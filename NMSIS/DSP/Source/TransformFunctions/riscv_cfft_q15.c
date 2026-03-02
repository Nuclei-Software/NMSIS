/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_q15.c
 * Description:  Combined Radix Decimation in Q15 Frequency CFFT processing function
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

#if !defined(RISCV_MATH_VECTOR)

extern void riscv_radix4_butterfly_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef,
        uint32_t twidCoefModifier);

extern void riscv_radix4_butterfly_inverse_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef,
        uint32_t twidCoefModifier);

extern void riscv_bitreversal_16(
        uint16_t * pSrc,
  const uint16_t bitRevLen,
  const uint16_t * pBitRevTable);

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef);

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_inverse_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef);

#endif

/**
  @addtogroup ComplexFFTQ15
  @{
 */

/**
  @brief         Processing function for Q15 complex FFT.
  @param[in]     S               points to an instance of Q15 CFFT structure
  @param[in,out] p1              points to the complex data buffer. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
 
  @par             Input and Output formats for CFFT Q15

| CFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ---------: | ------------: | -------------: | ------------------------: |
| 16         | 1.15          | 5.11           | 4        
| 64         | 1.15          | 7.9            | 6      
| 256        | 1.15          | 9.7            | 8     
| 1024       | 1.15          | 11.5           | 10      

  @par             Input and Output formats for CIFFT Q15

| CIFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ----------: | ------------: | -------------: | ------------------------: |
| 16          | 1.15          | 5.11           | 0        
| 64          | 1.15          | 7.9            | 0      
| 256         | 1.15          | 9.7            | 0     
| 1024        | 1.15          | 11.5           | 0     

  @par RVV version
                     The rvv version has a different API.
                     The input and output buffers must be
                     different.
                     There is a temporary buffer.
                     The bit reverse flag is not more 
                     available in rvv version.

  @code
        void riscv_cfft_q15(
  const riscv_cfft_instance_q15 * S,
        const q15_t * src,
        q15_t * dst,
        q15_t *buffer,
        uint8_t ifftFlag)
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".

 */

#if defined(RISCV_MATH_VECTOR)

RISCV_DSP_ATTRIBUTE void riscv_cfft_q15(
  const riscv_cfft_instance_q15 * S,
        const q15_t * src,
        q15_t * dst,
        q15_t *buffer,
        uint8_t ifftFlag)
{
    const long N = S->fftLen;
    int16_t *buf[2] = {buffer, buffer + N * 2};
    int buf_idx = 0; // data in buf_idx

    const int16_t *ptwd_re = S->ptwd_re;
    const int16_t *ptwd_im = S->ptwd_im;

    int32_t avl = N >> 1;
    size_t vl;
    const int16_t *px = src;
    int16_t *py = buf[buf_idx];
    do {
        vl = __riscv_vsetvl_e16m2(avl);

        vint16m2x2_t v_tuple = __riscv_vlseg2e16_v_i16m2x2(px, vl);
        vint16m2_t va_re = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 0);
        vint16m2_t va_im = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 1);
        v_tuple = __riscv_vlseg2e16_v_i16m2x2(px + N, vl);
        vint16m2_t vb_re = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 0);
        vint16m2_t vb_im = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 1);
        px += 2 * vl;

        if (ifftFlag) {
            va_im = __riscv_vneg_v_i16m2(va_im, vl);
            vb_im = __riscv_vneg_v_i16m2(vb_im, vl);
        }

        // scale down to prevent overflow
        va_re = __riscv_vsra_vx_i16m2(va_re, 1, vl);
        va_im = __riscv_vsra_vx_i16m2(va_im, 1, vl);
        vb_re = __riscv_vsra_vx_i16m2(vb_re, 1, vl);
        vb_im = __riscv_vsra_vx_i16m2(vb_im, 1, vl);

        vint16m2_t vre0 = __riscv_vsadd_vv_i16m2(va_re, vb_re, vl);
        vre0 = __riscv_vsra_vx_i16m2(vre0, 1, vl);
        vint16m2_t vim0 = __riscv_vsadd_vv_i16m2(va_im, vb_im, vl);
        vim0 = __riscv_vsra_vx_i16m2(vim0, 1, vl);

        vint16m2_t vtmp_re = __riscv_vssub_vv_i16m2(va_re, vb_re, vl);
        vtmp_re = __riscv_vsra_vx_i16m2(vtmp_re, 1, vl);
        vint16m2_t vtmp_im = __riscv_vssub_vv_i16m2(va_im, vb_im, vl);
        vtmp_im = __riscv_vsra_vx_i16m2(vtmp_im, 1, vl);

        vint16m2_t vtwd_re = __riscv_vle16_v_i16m2(ptwd_re, vl);
        ptwd_re += vl;
        vint16m2_t vtwd_im = __riscv_vle16_v_i16m2(ptwd_im, vl);
        ptwd_im += vl;

        vint16m2_t vre1 = __riscv_vssub_vv_i16m2(
            __riscv_vsmul_vv_i16m2(vtmp_re, vtwd_re, __RISCV_VXRM_RNU, vl),
            __riscv_vsmul_vv_i16m2(vtmp_im, vtwd_im, __RISCV_VXRM_RNU, vl), vl);
        vint16m2_t vim1 = __riscv_vsadd_vv_i16m2(
            __riscv_vsmul_vv_i16m2(vtmp_im, vtwd_re, __RISCV_VXRM_RNU, vl),
            __riscv_vsmul_vv_i16m2(vtmp_re, vtwd_im, __RISCV_VXRM_RNU, vl), vl);

        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vre0);
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vre1);
        __riscv_vsseg2e16_v_i16m2x2(py, v_tuple, vl);
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vim0);
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vim1);
        __riscv_vsseg2e16_v_i16m2x2(py + N, v_tuple, vl);
        py += 2 * vl;

        avl -= vl;
    } while (avl != 0);

    for (int a = N >> 1, stage = 0; a > 2; a >>= 1, stage++) {
        avl = N >> 1;
        const int16_t *px = buf[buf_idx];
        int16_t *py = buf[1 - buf_idx];
            do {
                size_t vl = __riscv_vsetvl_e16m2(avl);

                vint16m2_t va_re = __riscv_vle16_v_i16m2(px, vl);
                vint16m2_t va_im = __riscv_vle16_v_i16m2(px + N, vl);
                vint16m2_t vb_re = __riscv_vle16_v_i16m2(px + N / 2, vl);
                vint16m2_t vb_im = __riscv_vle16_v_i16m2(px + N * 3 / 2, vl);
                px += vl;

                vint16m2_t vre0 = __riscv_vadd_vv_i16m2(va_re, vb_re, vl);
                vre0 = __riscv_vsra_vx_i16m2(vre0, 1, vl);
                vint16m2_t vim0 = __riscv_vadd_vv_i16m2(va_im, vb_im, vl);
                vim0 = __riscv_vsra_vx_i16m2(vim0, 1, vl);

                vint16m2_t vtmp_re = __riscv_vsub_vv_i16m2(va_re, vb_re, vl);
                vtmp_re = __riscv_vsra_vx_i16m2(vtmp_re, 1, vl);
                vint16m2_t vtmp_im = __riscv_vsub_vv_i16m2(va_im, vb_im, vl);
                vtmp_im = __riscv_vsra_vx_i16m2(vtmp_im, 1, vl);

                vint16m2_t vtwd_re = __riscv_vle16_v_i16m2(ptwd_re, vl);
                ptwd_re += vl;
                vint16m2_t vtwd_im = __riscv_vle16_v_i16m2(ptwd_im, vl);
                ptwd_im += vl;

                vint16m2_t vre1 = __riscv_vssub_vv_i16m2(
                    __riscv_vsmul_vv_i16m2(vtmp_re, vtwd_re, __RISCV_VXRM_RNU,
                                           vl),
                    __riscv_vsmul_vv_i16m2(vtmp_im, vtwd_im, __RISCV_VXRM_RNU,
                                           vl),
                    vl);
                vint16m2_t vim1 = __riscv_vsadd_vv_i16m2(
                    __riscv_vsmul_vv_i16m2(vtmp_re, vtwd_im, __RISCV_VXRM_RNU,
                                           vl),
                    __riscv_vsmul_vv_i16m2(vtmp_im, vtwd_re, __RISCV_VXRM_RNU,
                                           vl),
                    vl);

                vint16m2x2_t v_tuple;
                v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vre0);
                v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vre1);
                __riscv_vsseg2e16_v_i16m2x2(py, v_tuple, vl);

                v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vim0);
                v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vim1);
                __riscv_vsseg2e16_v_i16m2x2(py + N, v_tuple, vl);
                py += 2 * vl;

                avl -= vl;
            } while (avl != 0);
        buf_idx = 1 - buf_idx;
    }

    avl = N >> 1;
    px = buf[buf_idx];
    py = dst;
    const uint16_t *pidx = S->pBitRevTable;
    do {
        size_t vl = __riscv_vsetvl_e16m2(avl);

        vint16m2_t va_re = __riscv_vle16_v_i16m2(px, vl);
        vint16m2_t va_im = __riscv_vle16_v_i16m2(px + N, vl);
        vint16m2_t vb_re = __riscv_vle16_v_i16m2(px + N / 2, vl);
        vint16m2_t vb_im = __riscv_vle16_v_i16m2(px + N * 3 / 2, vl);
        px += vl;

        vint16m2_t vre0 = __riscv_vadd_vv_i16m2(va_re, vb_re, vl);
        vint16m2_t vim0 = __riscv_vadd_vv_i16m2(va_im, vb_im, vl);

        vint16m2_t vre1 = __riscv_vsub_vv_i16m2(va_re, vb_re, vl);
        vint16m2_t vim1 = __riscv_vsub_vv_i16m2(va_im, vb_im, vl);

        if (ifftFlag) {
            vim0 = __riscv_vneg_v_i16m2(vim0, vl);
            vim1 = __riscv_vneg_v_i16m2(vim1, vl);
        }

        vuint16m2_t vidx = __riscv_vle16_v_u16m2(pidx, vl);
        vint16m2x2_t v_tuple;
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vre0);
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vim0);
        __riscv_vsoxseg2ei16_v_i16m2x2(py, vidx, v_tuple, vl);

        vidx = __riscv_vle16_v_u16m2(pidx + (N >> 1), vl);
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 0, vre1);
        v_tuple = __riscv_vset_v_i16m2_i16m2x2(v_tuple, 1, vim1);

        pidx += vl;
        __riscv_vsoxseg2ei16_v_i16m2x2(py, vidx, v_tuple, vl);

        avl -= vl;
    } while (avl != 0);
}

#else
RISCV_DSP_ATTRIBUTE void riscv_cfft_q15(
  const riscv_cfft_instance_q15 * S,
        q15_t * p1,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag)
{
  uint32_t L = S->fftLen;

  if (ifftFlag == 1U)
  {
     switch (L)
     {
     case 16:
     case 64:
     case 256:
     case 1024:
     case 4096:
       riscv_radix4_butterfly_inverse_q15 ( p1, L, (q15_t*)S->pTwiddle, 1 );
       break;

     case 32:
     case 128:
     case 512:
     case 2048:
       riscv_cfft_radix4by2_inverse_q15 ( p1, L, S->pTwiddle );
       break;
     }
  }
  else
  {
     switch (L)
     {
     case 16:
     case 64:
     case 256:
     case 1024:
     case 4096:
       riscv_radix4_butterfly_q15  ( p1, L, (q15_t*)S->pTwiddle, 1 );
       break;

     case 32:
     case 128:
     case 512:
     case 2048:
       riscv_cfft_radix4by2_q15  ( p1, L, S->pTwiddle );
       break;
     }
  }

  if ( bitReverseFlag )
    riscv_bitreversal_16 ((uint16_t*) p1, S->bitRevLength, S->pBitRevTable);
}

#endif

/**
  @} end of ComplexFFTQ15 group
 */

#if !defined(RISCV_MATH_VECTOR)

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef)
{
        uint32_t i;
        uint32_t n2;
        q15_t p0, p1, p2, p3;
#if defined (RISCV_MATH_DSP)
        q31_t T, S, R;
        q31_t coeff, out1, out2;
  const q15_t *pC = pCoef;
        q15_t *pSi = pSrc;
        q15_t *pSl = pSrc + fftLen;
#if __RISCV_XLEN == 64
        q15_t xt, yt, cosVal, sinVal;
        uint32_t n264;
        q63_t T64, S64, R64;
        q63_t coeff64, out164, out264;
        uint32_t l;
#endif /* __RISCV_XLEN == 64 */
#else
        uint32_t l;
        q15_t xt, yt, cosVal, sinVal;
#endif

  n2 = fftLen >> 1U;

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
  n264 = n2 >> 1U;
  for (i = n264; i > 0; i--)
  {
      coeff64 = read_q15x4_ia ((q15_t **) &pC);

      T64 = read_q15x4 (pSi);
      T64 = __SHADD16(T64, 0); /* this is just a SIMD arithmetic shift right by 1 */

      S64 = read_q15x4 (pSl);
      S64 = __SHADD16(S64, 0); /* this is just a SIMD arithmetic shift right by 1 */

      R64 = __QSUB16(T64, S64);

      write_q15x4_ia (&pSi, __SHADD16(T64, S64));

      out164 = __SMUAD(coeff64, R64) >> 16U;
      out264 = __SMUSDX(coeff64, R64);

      write_q15x4_ia (&pSl, (q63_t) (((out264) & 0xFFFF0000FFFF0000) | (out164 & 0x0000FFFF0000FFFF)));
  }
  for (i = n2 & 1; i > 0; i--)
#else
  for (i = n2; i > 0; i--)
#endif /* __RISCV_XLEN == 64 */
  {
      coeff = read_q15x2_ia ((q15_t **) &pC);

      T = read_q15x2 (pSi);
      T = __SHADD16(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

      S = read_q15x2 (pSl);
      S = __SHADD16(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

      R = __QSUB16(T, S);

      write_q15x2_ia (&pSi, __SHADD16(T, S));

      out1 = __SMUAD(coeff, R) >> 16U;
      out2 = __SMUSDX(coeff, R);

      write_q15x2_ia (&pSl, (q31_t)__PKHBT( out1, out2, 0 ) );
  }

#else /* #if defined (RISCV_MATH_DSP) */

  for (i = 0; i < n2; i++)
  {
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt =           (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
     pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

     yt =               (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
     pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1U) + (pSrc[2 * i + 1] >> 1U)) >> 1U;

     pSrc[2 * l]     = (((int16_t) (((q31_t) xt * cosVal) >> 16U)) +
                        ((int16_t) (((q31_t) yt * sinVal) >> 16U))  );

     pSrc[2 * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16U)) -
                        ((int16_t) (((q31_t) xt * sinVal) >> 16U))   );
  }

#endif /* #if defined (RISCV_MATH_DSP) */

  /* first col */
  riscv_radix4_butterfly_q15( pSrc,          n2, (q15_t*)pCoef, 2U);

  /* second col */
  riscv_radix4_butterfly_q15( pSrc + fftLen, n2, (q15_t*)pCoef, 2U);

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
     p0 = pSrc[4 * i + 0];
     p1 = pSrc[4 * i + 1];
     p2 = pSrc[4 * i + 2];
     p3 = pSrc[4 * i + 3];

     p0 <<= 1U;
     p1 <<= 1U;
     p2 <<= 1U;
     p3 <<= 1U;

     pSrc[4 * i + 0] = p0;
     pSrc[4 * i + 1] = p1;
     pSrc[4 * i + 2] = p2;
     pSrc[4 * i + 3] = p3;
  }

}

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_inverse_q15(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pCoef)
{
        uint32_t i;
        uint32_t n2;
        q15_t p0, p1, p2, p3;
#if defined (RISCV_MATH_DSP)
        q31_t T, S, R;
        q31_t coeff, out1, out2;
  const q15_t *pC = pCoef;
        q15_t *pSi = pSrc;
        q15_t *pSl = pSrc + fftLen;
#if __RISCV_XLEN == 64
        q15_t xt, yt, cosVal, sinVal;
        uint32_t n264;
        q63_t T64, S64, R64;
        q63_t coeff64, out164, out264;
        uint32_t l;
#endif /* __RISCV_XLEN == 64 */
#else
        uint32_t l;
        q15_t xt, yt, cosVal, sinVal;
#endif

  n2 = fftLen >> 1U;

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
  n264 = n2 >> 1U;
  for (i = n264; i > 0; i--)
  {
      coeff64 = read_q15x4_ia ((q15_t **) &pC);

      T64 = read_q15x4 (pSi);
      T64 = __SHADD16(T64, 0); /* this is just a SIMD arithmetic shift right by 1 */

      S64 = read_q15x4 (pSl);
      S64 = __SHADD16(S64, 0); /* this is just a SIMD arithmetic shift right by 1 */

      R64 = __QSUB16(T64, S64);

      write_q15x4_ia (&pSi, __SHADD16(T64, S64));

      out164 = __SMUSD(coeff64, R64) >> 16U;
      out264 = __SMUADX(coeff64, R64);

      write_q15x4_ia (&pSl, (q63_t) ((out264) & 0xFFFF0000FFFF0000) | (out164 & 0x0000FFFF0000FFFF));
  }
  for (i = n2 & 1; i > 0; i--)
#else
  for (i = n2; i > 0; i--)
#endif /* __RISCV_XLEN == 64 */
  {
     coeff = read_q15x2_ia ((q15_t **) &pC);

     T = read_q15x2 (pSi);
     T = __SHADD16(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

     S = read_q15x2 (pSl);
     S = __SHADD16(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

     R = __QSUB16(T, S);

     write_q15x2_ia (&pSi, __SHADD16(T, S));

     out1 = __SMUSD(coeff, R) >> 16U;
     out2 = __SMUADX(coeff, R);

     write_q15x2_ia (&pSl, (q31_t)__PKHBT( out1, out2, 0 ));
  }

#else /* #if defined (RISCV_MATH_DSP) */

  for (i = 0; i < n2; i++)
  {
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt =           (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
     pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

     yt =               (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
     pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1U) + (pSrc[2 * i + 1] >> 1U)) >> 1U;

     pSrc[2 * l]      = (((int16_t) (((q31_t) xt * cosVal) >> 16U)) -
                         ((int16_t) (((q31_t) yt * sinVal) >> 16U))  );

     pSrc[2 * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16U)) +
                        ((int16_t) (((q31_t) xt * sinVal) >> 16U))  );
  }

#endif /* #if defined (RISCV_MATH_DSP) */

  /* first col */
  riscv_radix4_butterfly_inverse_q15( pSrc,          n2, (q15_t*)pCoef, 2U);

  /* second col */
  riscv_radix4_butterfly_inverse_q15( pSrc + fftLen, n2, (q15_t*)pCoef, 2U);

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
     p0 = pSrc[4 * i + 0];
     p1 = pSrc[4 * i + 1];
     p2 = pSrc[4 * i + 2];
     p3 = pSrc[4 * i + 3];

     p0 <<= 1U;
     p1 <<= 1U;
     p2 <<= 1U;
     p3 <<= 1U;

     pSrc[4 * i + 0] = p0;
     pSrc[4 * i + 1] = p1;
     pSrc[4 * i + 2] = p2;
     pSrc[4 * i + 3] = p3;
  }
}
#endif /* defined RISCV_MATH_VECTOR */
