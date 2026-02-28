/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rfft_q31.c
 * Description:  FFT & RIFFT Q31 process function
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
RISCV_DSP_ATTRIBUTE void riscv_split_rfft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier);

RISCV_DSP_ATTRIBUTE void riscv_split_rifft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier);
#endif
/**
  @addtogroup RealFFTQ31
  @{
 */

/**
  @brief         Processing function for the Q31 RFFT/RIFFT.
  @param[in]     S     points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     pSrc  points to input buffer (Source buffer is modified by this function)
  @param[out]    pDst  points to output buffer

  @par           Input an output formats
                   Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.
                   Hence the output format is different for different RFFT sizes.
                   The input and output formats for different RFFT sizes and number of bits to upscale are mentioned in the tables below for RFFT and RIFFT:
  @par             Input and Output formats for RFFT Q31

| RFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ---------: | ------------: | -------------: | ------------------------: |
| 32         | 1.31          | 6.26           | 5                         |
| 64         | 1.31          | 7.25           | 6                         |
| 128        | 1.31          | 8.24           | 7                         |
| 256        | 1.31          | 9.23           | 8                         |
| 512        | 1.31          | 10.22          | 9                         |
| 1024       | 1.31          | 11.21          | 10                        |
| 2048       | 1.31          | 12.20          | 11                        |
| 4096       | 1.31          | 13.19          | 12                        |
| 8192       | 1.31          | 14.18          | 13                        |
             
  @par             Input and Output formats for RIFFT Q31

| RIFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ----------: | ------------: | -------------: | ------------------------: |
| 32          | 1.31          | 6.26           | 0                         |
| 64          | 1.31          | 7.25           | 0                         |
| 128         | 1.31          | 8.24           | 0                         |
| 256         | 1.31          | 9.23           | 0                         |
| 512         | 1.31          | 10.22          | 0                         |
| 1024        | 1.31          | 11.21          | 0                         |
| 2048        | 1.31          | 12.20          | 0                         |
| 4096        | 1.31          | 13.19          | 0                         |
| 8192        | 1.31          | 14.18          | 0                         |

                   
  @par RVV implementation
       A temporary buffer is needed

  @code 
       void riscv_rfft_q31(
  const riscv_rfft_instance_q31 * S,
        const q31_t * pSrc,
        q31_t * pDst,
        q31_t *tmp,
        uint8_t ifftFlag
        )
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".

 */

#if defined(RISCV_MATH_VECTOR)


typedef struct {
    int32_t re;
    int32_t im;
} cplxq31;

RISCV_DSP_ATTRIBUTE void riscv_rfft_q31(
  const riscv_rfft_instance_q31 * S,
        const q31_t * pSrc,
        q31_t * pDst,
        q31_t *tmp,
        uint8_t ifftFlag
        )
{
    const long N = S->fftLenRFFT;
    if (ifftFlag) {
        size_t avl = N >> 1;
        const cplxq31 *px = (const cplxq31 *)pSrc;
        const cplxq31 *px_inv = (const cplxq31 *)(pSrc + N);
        const int32_t *ptwd_re = S->ptwd_re;
        const int32_t *ptwd_im = S->ptwd_im;
        int32_t *py = pDst;
        do {
            size_t vl = __riscv_vsetvl_e32m2(avl);

            vint32m2x2_t vx =
                __riscv_vlseg2e32_v_i32m2x2((const int32_t *)px, vl);
            px += vl;
            vint32m2_t vx_re = __riscv_vget_v_i32m2x2_i32m2(vx, 0);
            vx_re = __riscv_vsra_vx_i32m2(vx_re, 2, vl);
            vint32m2_t vx_im = __riscv_vget_v_i32m2x2_i32m2(vx, 1);
            vx_im = __riscv_vsra_vx_i32m2(vx_im, 2, vl);

            vint32m2x2_t vx_inv = __riscv_vlsseg2e32_v_i32m2x2(
                (const int32_t *)px_inv, -sizeof(cplxq31), vl);
            px_inv -= vl;
            vint32m2_t vx_inv_re = __riscv_vget_v_i32m2x2_i32m2(vx_inv, 0);
            vx_inv_re = __riscv_vsra_vx_i32m2(vx_inv_re, 2, vl);
            vint32m2_t vx_inv_im = __riscv_vget_v_i32m2x2_i32m2(vx_inv, 1);
            vx_inv_im = __riscv_vsra_vx_i32m2(vx_inv_im, 2, vl);
            vx_inv_im = __riscv_vneg_v_i32m2(vx_inv_im, vl);

            vint32m2_t vy_re = __riscv_vadd_vv_i32m2(vx_re, vx_inv_re, vl);
            vint32m2_t vy_im = __riscv_vadd_vv_i32m2(vx_im, vx_inv_im, vl);

            vint32m2_t vtmp_re = __riscv_vsub_vv_i32m2(vx_re, vx_inv_re, vl);
            vint32m2_t vtmp_im = __riscv_vsub_vv_i32m2(vx_im, vx_inv_im, vl);

            vint32m2_t vtwd_re = __riscv_vle32_v_i32m2(ptwd_im, vl);
            ptwd_im += vl;
            vint32m2_t vtwd_im = __riscv_vle32_v_i32m2(ptwd_re, vl);
            ptwd_re += vl;

            vy_re = __riscv_vadd_vv_i32m2(
                vy_re,
                __riscv_vsmul_vv_i32m2(vtmp_re, vtwd_re, __RISCV_VXRM_RNU, vl),
                vl);
            vy_re = __riscv_vsub_vv_i32m2(
                vy_re,
                __riscv_vsmul_vv_i32m2(vtmp_im, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);

            vy_im = __riscv_vadd_vv_i32m2(
                vy_im,
                __riscv_vsmul_vv_i32m2(vtmp_re, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);
            vy_im = __riscv_vadd_vv_i32m2(
                vy_im,
                __riscv_vsmul_vv_i32m2(vtmp_im, vtwd_re, __RISCV_VXRM_RNU, vl),
                vl);

            vint32m2x2_t v_tuple;
            v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vy_re);
            v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vy_im);
            __riscv_vsseg2e32_v_i32m2x2(py, v_tuple, vl);

            py += 2 * vl;
            avl -= vl;
        } while (avl != 0);

        riscv_cfft_q31(&S->Sint, pDst, pDst, tmp, ifftFlag);

        avl = N;
        py = pDst;
        do {
            size_t vl = __riscv_vsetvl_e32m4(avl);
            vint32m4_t vy = __riscv_vle32_v_i32m4(py, vl);
            vy = __riscv_vsll_vx_i32m4(vy, 1, vl);
            __riscv_vse32_v_i32m4(py, vy, vl);
            py += vl;
            avl -= vl;
        } while (avl != 0);

    } else {
        int32_t *y = tmp + 2 * N;
        riscv_cfft_q31(&S->Sint, pSrc, y, tmp, ifftFlag);

        // Y[0] = F[0] + G[0]
        const int32_t F0 = y[0];
        const int32_t G0 = y[1];
        pDst[0] = (F0 + G0) >> 1;
        pDst[1] = 0;
        pDst[N] = (F0 - G0) >> 1;
        pDst[N + 1] = 0;

        size_t avl = (N >> 1) - 1;
        const int32_t *py = y + 2;
        const int32_t *py_inv = y + N - 2;
        int32_t *pout = pDst + 2;
        const int32_t *ptwd_re = S->ptwd_re + 1;
        const int32_t *ptwd_im = S->ptwd_im + 1;
        do {
            size_t vl = __riscv_vsetvl_e32m2(avl);

            // load vx_re, vx_im
            vint32m2x2_t v_tuple = __riscv_vlseg2e32_v_i32m2x2(py, vl);
            py += vl * 2;
            vint32m2_t vy_re = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 0);
            vint32m2_t vy_im = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 1);

            v_tuple =
                __riscv_vlsseg2e32_v_i32m2x2(py_inv, -sizeof(int32_t) * 2, vl);
            py_inv -= vl * 2;
            vint32m2_t vy_inv_re = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 0);
            vint32m2_t vy_inv_im = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 1);

            // vFr
            vint32m2_t vFr_re = __riscv_vadd_vv_i32m2(vy_re, vy_inv_re, vl);
            vFr_re = __riscv_vsra_vx_i32m2(vFr_re, 2, vl);
            vint32m2_t vFr_im = __riscv_vsub_vv_i32m2(vy_im, vy_inv_im, vl);
            vFr_im = __riscv_vsra_vx_i32m2(vFr_im, 2, vl);

            // vGr
            vint32m2_t vGr_re = __riscv_vadd_vv_i32m2(vy_inv_im, vy_im, vl);
            vGr_re = __riscv_vsra_vx_i32m2(vGr_re, 2, vl);
            vint32m2_t vGr_im = __riscv_vsub_vv_i32m2(vy_inv_re, vy_re, vl);
            vGr_im = __riscv_vsra_vx_i32m2(vGr_im, 2, vl);

            vint32m2_t vtwd_re = __riscv_vle32_v_i32m2(ptwd_re, vl);
            ptwd_re += vl;
            vint32m2_t vtwd_im = __riscv_vle32_v_i32m2(ptwd_im, vl);
            ptwd_im += vl;

            vint32m2_t vout_re = __riscv_vssub_vv_i32m2(
                __riscv_vsmul_vv_i32m2(vGr_re, vtwd_re, __RISCV_VXRM_RNU, vl),
                __riscv_vsmul_vv_i32m2(vGr_im, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);
            vint32m2_t vout_im = __riscv_vsadd_vv_i32m2(
                __riscv_vsmul_vv_i32m2(vGr_im, vtwd_re, __RISCV_VXRM_RNU, vl),
                __riscv_vsmul_vv_i32m2(vGr_re, vtwd_im, __RISCV_VXRM_RNU, vl),
                vl);

            vout_re = __riscv_vadd_vv_i32m2(vFr_re, vout_re, vl);
            vout_im = __riscv_vadd_vv_i32m2(vFr_im, vout_im, vl);

            v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vout_re);
            v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vout_im);
            __riscv_vsseg2e32_v_i32m2x2(pout, v_tuple, vl);
            pout += 2 * vl;

            avl -= vl;
        } while (avl != 0);
    }
}
#else
RISCV_DSP_ATTRIBUTE void riscv_rfft_q31(
  const riscv_rfft_instance_q31 * S,
        q31_t * pSrc,
        q31_t * pDst)
{
  const riscv_cfft_instance_q31 *S_CFFT = S->pCfft;
        uint32_t L2 = S->fftLenReal >> 1U;

  /* Calculation of RIFFT of input */
  if (S->ifftFlagR == 1U)
  {
     /*  Real IFFT core process */
     riscv_split_rifft_q31 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);

     /* Complex IFFT process */
     riscv_cfft_q31 (S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

     riscv_shift_q31(pDst, 1, pDst, S->fftLenReal);
  }
  else
  {
     /* Calculation of RFFT of input */

     /* Complex FFT process */
     riscv_cfft_q31 (S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

     /*  Real FFT core process */
     riscv_split_rfft_q31 (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }

}

#endif
/**
  @} end of RealFFTQ31 group
 */

/**
  @brief         Core Real FFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
 */

#if !defined(RISCV_MATH_VECTOR)
RISCV_DSP_ATTRIBUTE void riscv_split_rfft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier)
{
        uint32_t i;                                    /* Loop Counter */
        q31_t outR, outI;                              /* Temporary variables for output */
  const q31_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q31_t CoefA1, CoefA2, CoefB1;                  /* Temporary variables for twiddle coefficients */
        q31_t *pOut1 = &pDst[2], *pOut2 = &pDst[4 * fftLen - 1];
        q31_t *pIn1 =  &pSrc[2], *pIn2 =  &pSrc[2 * fftLen - 1];

  /* Init coefficient pointers */
  pCoefA = &pATable[modifier * 2];
  pCoefB = &pBTable[modifier * 2];

  i = fftLen - 1U;

  while (i > 0U)
  {
     /*
       outR = (  pSrc[2 * i]             * pATable[2 * i]
               - pSrc[2 * i + 1]         * pATable[2 * i + 1]
               + pSrc[2 * n - 2 * i]     * pBTable[2 * i]
               + pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1]);

       outI = (  pIn[2 * i + 1]         * pATable[2 * i]
               + pIn[2 * i]             * pATable[2 * i + 1]
               + pIn[2 * n - 2 * i]     * pBTable[2 * i + 1]
               - pIn[2 * n - 2 * i + 1] * pBTable[2 * i]);
      */

     CoefA1 = *pCoefA++;
     CoefA2 = *pCoefA;

     /* outR = (pSrc[2 * i] * pATable[2 * i] */
     mult_32x32_keep32_R (outR, *pIn1, CoefA1);

     /* outI = pIn[2 * i] * pATable[2 * i + 1] */
     mult_32x32_keep32_R (outI, *pIn1++, CoefA2);

     /* - pSrc[2 * i + 1] * pATable[2 * i + 1] */
     multSub_32x32_keep32_R (outR, *pIn1, CoefA2);

     /* (pIn[2 * i + 1] * pATable[2 * i] */
     multAcc_32x32_keep32_R (outI, *pIn1++, CoefA1);

     /* pSrc[2 * n - 2 * i] * pBTable[2 * i]  */
     multSub_32x32_keep32_R (outR, *pIn2, CoefA2);
     CoefB1 = *pCoefB;

     /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] */
     multSub_32x32_keep32_R (outI, *pIn2--, CoefB1);

     /* pSrc[2 * n - 2 * i + 1] * pBTable[2 * i + 1] */
     multAcc_32x32_keep32_R (outR, *pIn2, CoefB1);

     /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
     multSub_32x32_keep32_R (outI, *pIn2--, CoefA2);

     /* write output */
     *pOut1++ = outR;
     *pOut1++ = outI;

     /* write complex conjugate output */
     *pOut2-- = -outI;
     *pOut2-- = outR;

     /* update coefficient pointer */
     pCoefB = pCoefB + (2 * modifier);
     pCoefA = pCoefA + (2 * modifier - 1);

     /* Decrement loop count */
     i--;
  }

  pDst[2 * fftLen]     = (pSrc[0] - pSrc[1]) >> 1U;
  pDst[2 * fftLen + 1] = 0;

  pDst[0] = (pSrc[0] + pSrc[1]) >> 1U;
  pDst[1] = 0;
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
 */

#if !defined(RISCV_MATH_VECTOR)
RISCV_DSP_ATTRIBUTE void riscv_split_rifft_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier)
{       
        q31_t outR, outI;                              /* Temporary variables for output */
  const q31_t *pCoefA, *pCoefB;                        /* Temporary pointers for twiddle factors */
        q31_t CoefA1, CoefA2, CoefB1;                  /* Temporary variables for twiddle coefficients */
        q31_t *pIn1 = &pSrc[0], *pIn2 = &pSrc[2 * fftLen + 1];

  pCoefA = &pATable[0];
  pCoefB = &pBTable[0];

  while (fftLen > 0U)
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

     CoefA1 = *pCoefA++;
     CoefA2 = *pCoefA;

     /* outR = (pIn[2 * i] * pATable[2 * i] */
     mult_32x32_keep32_R (outR, *pIn1, CoefA1);

     /* - pIn[2 * i] * pATable[2 * i + 1] */
     mult_32x32_keep32_R (outI, *pIn1++, -CoefA2);

     /* pIn[2 * i + 1] * pATable[2 * i + 1] */
     multAcc_32x32_keep32_R (outR, *pIn1, CoefA2);

     /* pIn[2 * i + 1] * pATable[2 * i] */
     multAcc_32x32_keep32_R (outI, *pIn1++, CoefA1);

     /* pIn[2 * n - 2 * i] * pBTable[2 * i] */
     multAcc_32x32_keep32_R (outR, *pIn2, CoefA2);
     CoefB1 = *pCoefB;

     /* pIn[2 * n - 2 * i] * pBTable[2 * i + 1] */
     multSub_32x32_keep32_R (outI, *pIn2--, CoefB1);

     /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i + 1] */
     multAcc_32x32_keep32_R (outR, *pIn2, CoefB1);

     /* pIn[2 * n - 2 * i + 1] * pBTable[2 * i] */
     multAcc_32x32_keep32_R (outI, *pIn2--, CoefA2);

     /* write output */
     *pDst++ = outR;
     *pDst++ = outI;

     /* update coefficient pointer */
     pCoefB = pCoefB + (modifier * 2);
     pCoefA = pCoefA + (modifier * 2 - 1);

     /* Decrement loop count */
     fftLen--;
  }

}

#endif /* #if !defined(RISCV_MATH_VECTOR) */
