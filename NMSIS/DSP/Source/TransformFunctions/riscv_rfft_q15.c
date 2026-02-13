/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rfft_q15.c
 * Description:  RFFT & RIFFT Q15 process function
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "dsp/transform_functions.h"

/* ----------------------------------------------------------------------
 * Internal functions prototypes
 * -------------------------------------------------------------------- */

#if !defined(RISCV_MATH_VECTOR) 

RISCV_DSP_ATTRIBUTE void riscv_split_rfft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier);

RISCV_DSP_ATTRIBUTE void riscv_split_rifft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier);
#endif

/**
  @addtogroup RealFFTQ15
  @{
 */

/**
  @brief         Processing function for the Q15 RFFT/RIFFT.
  @param[in]     S     points to an instance of the Q15 RFFT/RIFFT structure
  @param[in]     pSrc  points to input buffer (Source buffer is modified by this function.)
  @param[out]    pDst  points to output buffer

  @par           Input an output formats
                   Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.
                   Hence the output format is different for different RFFT sizes.
                   The input and output formats for different RFFT sizes and number of bits to upscale are mentioned in the tables below for RFFT and RIFFT:
  @par             Input and Output formats for RFFT Q15

| RFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ---------: | ------------: | -------------: | ------------------------: |
| 32         | 1.15          | 6.10           | 5                         |
| 64         | 1.15          | 7.9            | 6                         |
| 128        | 1.15          | 8.8            | 7                         |
| 256        | 1.15          | 9.7            | 8                         |
| 512        | 1.15          | 10.6           | 9                         |
| 1024       | 1.15          | 11.5           | 10                        |
| 2048       | 1.15          | 12.4           | 11                        |
| 4096       | 1.15          | 13.3           | 12                        |
| 8192       | 1.15          | 14.2           | 13                        |
             
  @par             Input and Output formats for RIFFT Q15

| RIFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ----------: | ------------: | -------------: | ------------------------: |
| 32          | 1.15          | 6.10           | 0                         |
| 64          | 1.15          | 7.9            | 0                         |
| 128         | 1.15          | 8.8            | 0                         |
| 256         | 1.15          | 9.7            | 0                         |
| 512         | 1.15          | 10.6           | 0                         |
| 1024        | 1.15          | 11.5           | 0                         |
| 2048        | 1.15          | 12.4           | 0                         |
| 4096        | 1.15          | 13.3           | 0                         |
| 8192        | 1.15          | 14.2           | 0                         |
  

  
  @par RVV implementation
       A temporary buffer is required

  @code 
       void riscv_rfft_q15(
  const riscv_rfft_instance_q15 * S,
        const q15_t * pSrc,
        q15_t * pDst,
        q15_t *tmp,
        uint8_t ifftFlag
        )
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".  

 */

#if defined(RISCV_MATH_VECTOR) 


typedef struct {
    int16_t re;
    int16_t im;
} cplx16;

RISCV_DSP_ATTRIBUTE void riscv_rfft_q15(
  const riscv_rfft_instance_q15 * S,
        const q15_t * pSrc,
        q15_t * pDst,
        q15_t *tmp,
        uint8_t ifftFlag
        )
{
    const long N = S->fftLenRFFT;
    if (ifftFlag) {
        size_t avl = N >> 1;
        const cplx16 *px = (const cplx16 *)pSrc;
        const cplx16 *px_inv = (const cplx16 *)(pSrc + N);
        const int16_t *ptwd_re = S->ptwd_re;
        const int16_t *ptwd_im = S->ptwd_im;
        int16_t *py = pDst;
        do {
            size_t vl = __riscv_vsetvl_e16m2(avl);

            vint16m2x2_t vx =
                __riscv_vlseg2e16_v_i16m2x2((const int16_t *)px, vl);
            px += vl;
            vint16m2_t vx_re = __riscv_vget_v_i16m2x2_i16m2(vx, 0);
            vx_re = __riscv_vsra_vx_i16m2(vx_re, 2, vl);
            vint16m2_t vx_im = __riscv_vget_v_i16m2x2_i16m2(vx, 1);
            vx_im = __riscv_vsra_vx_i16m2(vx_im, 2, vl);

            vint16m2x2_t vx_inv = __riscv_vlsseg2e16_v_i16m2x2(
                (const int16_t *)px_inv, -sizeof(cplx16), vl);
            px_inv -= vl;
            vint16m2_t vx_inv_re = __riscv_vget_v_i16m2x2_i16m2(vx_inv, 0);
            vx_inv_re = __riscv_vsra_vx_i16m2(vx_inv_re, 2, vl);
            vint16m2_t vx_inv_im = __riscv_vget_v_i16m2x2_i16m2(vx_inv, 1);
            vx_inv_im = __riscv_vsra_vx_i16m2(vx_inv_im, 2, vl);
            vx_inv_im = __riscv_vneg_v_i16m2(vx_inv_im, vl);

            vint16m2_t vy_re = __riscv_vadd_vv_i16m2(vx_re, vx_inv_re, vl);
            vint16m2_t vy_im = __riscv_vadd_vv_i16m2(vx_im, vx_inv_im, vl);

            vint16m2_t vtmp_re = __riscv_vsub_vv_i16m2(vx_re, vx_inv_re, vl);
            vint16m2_t vtmp_im = __riscv_vsub_vv_i16m2(vx_im, vx_inv_im, vl);

            vint16m2_t vtwd_re = __riscv_vle16_v_i16m2(ptwd_im, vl);
            ptwd_im += vl;
            vint16m2_t vtwd_im = __riscv_vle16_v_i16m2(ptwd_re, vl);
            ptwd_re += vl;

            vy_re = __riscv_vadd_vv_i16m2(
                vy_re,
                __riscv_vsmul_vv_i16m2(vtmp_re, vtwd_re, __RISCV_VXRM_RNU, vl),
                vl);
            vy_re = __riscv_vsub_vv_i16m2(
                vy_re,
                __riscv_vsmul_vv_i16m2(vtmp_im, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);

            vy_im = __riscv_vadd_vv_i16m2(
                vy_im,
                __riscv_vsmul_vv_i16m2(vtmp_re, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);
            vy_im = __riscv_vadd_vv_i16m2(
                vy_im,
                __riscv_vsmul_vv_i16m2(vtmp_im, vtwd_re, __RISCV_VXRM_RNU, vl),
                vl);

            vint16m2x2_t v_tuple;
            v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vy_re);
            v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vy_im);
            __riscv_vsseg2e16_v_i16m2x2(py, v_tuple, vl);

            py += 2 * vl;
            avl -= vl;
        } while (avl != 0);

        riscv_cfft_q15(&S->Sint, pDst, pDst, tmp, ifftFlag);

        avl = N;
        py = pDst;
        do {
            size_t vl = __riscv_vsetvl_e16m4(avl);
            vint16m4_t vy = __riscv_vle16_v_i16m4(py, vl);
            vy = __riscv_vsll_vx_i16m4(vy, 1, vl);
            __riscv_vse16_v_i16m4(py, vy, vl);
            py += vl;
            avl -= vl;
        } while (avl != 0);

    } else {
        int16_t *y = tmp + 2 * N;
        riscv_cfft_q15(&S->Sint, pSrc, y, tmp, ifftFlag);

        // Y[0] = F[0] + G[0]
        const int16_t F0 = y[0];
        const int16_t G0 = y[1];
        pDst[0] = (F0 + G0) >> 1;
        pDst[1] = 0;
        pDst[N] = (F0 - G0) >> 1;
        pDst[N + 1] = 0;

        size_t avl = (N >> 1) - 1;
        const int16_t *py = y + 2;
        const int16_t *py_inv = y + N - 2;
        int16_t *pout = pDst + 2;
        const int16_t *ptwd_re = S->ptwd_re + 1;
        const int16_t *ptwd_im = S->ptwd_im + 1;
        do {
            size_t vl = __riscv_vsetvl_e16m2(avl);

            // load vx_re, vx_im
            vint16m2x2_t v_tuple = __riscv_vlseg2e16_v_i16m2x2(py, vl);
            py += vl * 2;
            vint16m2_t vy_re = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 0);
            vint16m2_t vy_im = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 1);

            v_tuple =
                __riscv_vlsseg2e16_v_i16m2x2(py_inv, -sizeof(int16_t) * 2, vl);
            py_inv -= vl * 2;
            vint16m2_t vy_inv_re = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 0);
            vint16m2_t vy_inv_im = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 1);

            // vFr
            vint16m2_t vFr_re = __riscv_vadd_vv_i16m2(vy_re, vy_inv_re, vl);
            vFr_re = __riscv_vsra_vx_i16m2(vFr_re, 2, vl);
            vint16m2_t vFr_im = __riscv_vsub_vv_i16m2(vy_im, vy_inv_im, vl);
            vFr_im = __riscv_vsra_vx_i16m2(vFr_im, 2, vl);

            // vGr
            vint16m2_t vGr_re = __riscv_vadd_vv_i16m2(vy_inv_im, vy_im, vl);
            vGr_re = __riscv_vsra_vx_i16m2(vGr_re, 2, vl);
            vint16m2_t vGr_im = __riscv_vsub_vv_i16m2(vy_inv_re, vy_re, vl);
            vGr_im = __riscv_vsra_vx_i16m2(vGr_im, 2, vl);

            vint16m2_t vtwd_re = __riscv_vle16_v_i16m2(ptwd_re, vl);
            ptwd_re += vl;
            vint16m2_t vtwd_im = __riscv_vle16_v_i16m2(ptwd_im, vl);
            ptwd_im += vl;

            vint16m2_t vout_re = __riscv_vssub_vv_i16m2(
                __riscv_vsmul_vv_i16m2(vGr_re, vtwd_re, __RISCV_VXRM_RNU, vl),
                __riscv_vsmul_vv_i16m2(vGr_im, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);
            vint16m2_t vout_im = __riscv_vsadd_vv_i16m2(
                __riscv_vsmul_vv_i16m2(vGr_im, vtwd_re, __RISCV_VXRM_RNU, vl),
                __riscv_vsmul_vv_i16m2(vGr_re, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);

            vout_re = __riscv_vadd_vv_i16m2(vFr_re, vout_re, vl);
            vout_im = __riscv_vadd_vv_i16m2(vFr_im, vout_im, vl);

            v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vout_re);
            v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vout_im);
            __riscv_vsseg2e16_v_i16m2x2(pout, v_tuple, vl);
            pout += 2 * vl;

            avl -= vl;
        } while (avl != 0);
    }
}
#else
RISCV_DSP_ATTRIBUTE void riscv_rfft_q15(
  const riscv_rfft_instance_q15 * S,
        q15_t * pSrc,
        q15_t * pDst)
{
  const riscv_cfft_instance_q15 *S_CFFT = S->pCfft;
        uint32_t L2 = S->fftLenReal >> 1U;

  /* Calculation of RIFFT of input */
  if (S->ifftFlagR == 1U)
  {
     /*  Real IFFT core process */
     riscv_split_rifft_q15 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);

     /* Complex IFFT process */
     riscv_cfft_q15 (S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

     riscv_shift_q15(pDst, 1, pDst, S->fftLenReal);
  }
  else
  {
     /* Calculation of RFFT of input */

     /* Complex FFT process */
     riscv_cfft_q15 (S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

     /*  Real FFT core process */
     riscv_split_rfft_q15 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }

}
#endif

/**
  @} end of RealFFTQ15 group
 */

/**
  @brief         Core Real FFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table

  @par
                   The function implements a Real FFT
 */

#if !defined(RISCV_MATH_VECTOR) 
RISCV_DSP_ATTRIBUTE void riscv_split_rfft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{       
        uint32_t i;                                    /* Loop Counter */
        q31_t outR, outI;                              /* Temporary variables for output */
  const q15_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q15_t *pSrc1, *pSrc2;
#if defined (RISCV_MATH_DSP)
        q15_t *pD1, *pD2;
#endif

  /* Init coefficient pointers */
  pCoefA = &pATable[modifier * 2];
  pCoefB = &pBTable[modifier * 2];

  pSrc1 = &pSrc[2];
  pSrc2 = &pSrc[(2U * fftLen) - 2U];

#if defined (RISCV_MATH_DSP)

    i = 1U;
    pD1 = pDst + 2;
    pD2 = pDst + (4U * fftLen) - 2;

    for (i = fftLen - 1; i > 0; i--)
    {
        /*
          outR = (  pSrc[2 * i]             * pATable[2 * i]
                  - pSrc[2 * i + 1]         * pATable[2 * i + 1]
                  + pSrc[2 * n - 2 * i]     * pBTable[2 * i]
                  + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

          outI = (  pIn[2 * i + 1]         * pATable[2 * i]
                  + pIn[2 * i]             * pATable[2 * i + 1]
                  + pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
                  - pIn[2 * n - 2 * i + 1] * pBTable[2 * i])
         */


        /* pSrc[2 * i] * pATable[2 * i] - pSrc[2 * i + 1] * pATable[2 * i + 1] */
        outR = __SMUSD(read_q15x2 (pSrc1), read_q15x2((q15_t *) pCoefA));

        /* pSrc[2 * n - 2 * i] * pBTable[2 * i] + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]) */
        outR = __SMLAD(read_q15x2 (pSrc2), read_q15x2((q15_t *) pCoefB), outR) >> 16U;

        /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] - pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
        outI = __SMUSDX(read_q15x2_da (&pSrc2), read_q15x2((q15_t *) pCoefB));

        /* (pIn[2 * i + 1] * pATable[2 * i] + pIn[2 * i] * pATable[2 * i + 1] */
        outI = __SMLADX(read_q15x2_ia (&pSrc1), read_q15x2 ((q15_t *) pCoefA), outI);

        /* write output */
        *pD1++ = (q15_t) outR;
        *pD1++ = outI >> 16U;

        /* write complex conjugate output */
        pD2[0] = (q15_t) outR;
        pD2[1] = -(outI >> 16U);
        pD2 -= 2;

        /* update coefficient pointer */
        pCoefB = pCoefB + (2U * modifier);
        pCoefA = pCoefA + (2U * modifier);
    }

    pDst[2U * fftLen]      = (pSrc[0] - pSrc[1]) >> 1U;
    pDst[2U * fftLen + 1U] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1U;
    pDst[1] = 0;

#else

    i = 1U;

    while (i < fftLen)
    {
        /*
          outR = (  pSrc[2 * i]             * pATable[2 * i]
                  - pSrc[2 * i + 1]         * pATable[2 * i + 1]
                  + pSrc[2 * n - 2 * i]     * pBTable[2 * i]
                  + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);
        */

        outR = *pSrc1 * *pCoefA;
        outR = outR - (*(pSrc1 + 1) * *(pCoefA + 1));
        outR = outR + (*pSrc2 * *pCoefB);
        outR = (outR + (*(pSrc2 + 1) * *(pCoefB + 1))) >> 16;

        /*
          outI = (  pIn[2 * i + 1]         * pATable[2 * i]
                  + pIn[2 * i]             * pATable[2 * i + 1]
                  + pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
                  - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
        */

        outI = *pSrc2 * *(pCoefB + 1);
        outI = outI - (*(pSrc2 + 1) * *pCoefB);
        outI = outI + (*(pSrc1 + 1) * *pCoefA);
        outI = outI + (*pSrc1 * *(pCoefA + 1));

        /* update input pointers */
        pSrc1 += 2U;
        pSrc2 -= 2U;

        /* write output */
        pDst[2U * i] = (q15_t) outR;
        pDst[2U * i + 1U] = outI >> 16U;

        /* write complex conjugate output */
        pDst[(4U * fftLen) - (2U * i)] = (q15_t) outR;
        pDst[((4U * fftLen) - (2U * i)) + 1U] = -(outI >> 16U);

        /* update coefficient pointer */
        pCoefB = pCoefB + (2U * modifier);
        pCoefA = pCoefA + (2U * modifier);

        i++;
    }

    pDst[2U * fftLen] = (pSrc[0] - pSrc[1]) >> 1;
    pDst[2U * fftLen + 1U] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1;
    pDst[1] = 0;

#endif /* #if defined (RISCV_MATH_DSP) */

}
#endif /* #if !defined(RISCV_MATH_VECTOR) */

/**
  @brief         Core Real IFFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table

  @par
                   The function implements a Real IFFT
 */

#if !defined(RISCV_MATH_VECTOR) 
RISCV_DSP_ATTRIBUTE void riscv_split_rifft_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{
        uint32_t i;                                    /* Loop Counter */
        q31_t outR, outI;                              /* Temporary variables for output */
  const q15_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q15_t *pSrc1, *pSrc2;
        q15_t *pDst1 = &pDst[0];

  pCoefA = &pATable[0];
  pCoefB = &pBTable[0];

  pSrc1 = &pSrc[0];
  pSrc2 = &pSrc[2 * fftLen];

  i = fftLen;
  while (i > 0U)
  {
      /*
        outR = (  pIn[2 * i]             * pATable[2 * i]
                + pIn[2 * i + 1]         * pATable[2 * i + 1]
                + pIn[2 * n - 2 * i]     * pBTable[2 * i]
                - pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

        outI = (  pIn[2 * i + 1]         * pATable[2 * i]
                - pIn[2 * i]             * pATable[2 * i + 1]
                - pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
                - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
       */

#if defined (RISCV_MATH_DSP)

      /* pIn[2 * n - 2 * i] * pBTable[2 * i] - pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1]) */
      outR = __SMUSD(read_q15x2(pSrc2), read_q15x2((q15_t *) pCoefB));

      /* pIn[2 * i] * pATable[2 * i] + pIn[2 * i + 1] * pATable[2 * i + 1] + pIn[2 * n - 2 * i] * pBTable[2 * i] */
      outR = __SMLAD(read_q15x2(pSrc1), read_q15x2 ((q15_t *) pCoefA), outR) >> 16U;

      /* -pIn[2 * n - 2 * i] * pBTable[2 * i + 1] + pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
      outI = __SMUADX(read_q15x2_da (&pSrc2), read_q15x2((q15_t *) pCoefB));

      /* pIn[2 * i + 1] * pATable[2 * i] - pIn[2 * i] * pATable[2 * i + 1] */
      outI = __SMLSDX(read_q15x2 ((q15_t *) pCoefA), read_q15x2_ia (&pSrc1), -outI);

      /* write output */
      write_q15x2_ia (&pDst1, __PKHBT(outR, (outI >> 16U), 16));


#else  /* #if defined (RISCV_MATH_DSP) */

      outR = *pSrc2 * *pCoefB;
      outR = outR - (*(pSrc2 + 1) * *(pCoefB + 1));
      outR = outR + (*pSrc1 * *pCoefA);
      outR = (outR + (*(pSrc1 + 1) * *(pCoefA + 1))) >> 16;

      outI = *(pSrc1 + 1) * *pCoefA;
      outI = outI - (*pSrc1 * *(pCoefA + 1));
      outI = outI - (*pSrc2 * *(pCoefB + 1));
      outI = outI - (*(pSrc2 + 1) * *(pCoefB));

      /* update input pointers */
      pSrc1 += 2U;
      pSrc2 -= 2U;

      /* write output */
      *pDst1++ = (q15_t) outR;
      *pDst1++ = (q15_t) (outI >> 16);

#endif /* #if defined (RISCV_MATH_DSP) */

      /* update coefficient pointer */
      pCoefB = pCoefB + (2 * modifier);
      pCoefA = pCoefA + (2 * modifier);

      i--;
  }

}
#endif /* #if !defined(RISCV_MATH_VECTOR) */
