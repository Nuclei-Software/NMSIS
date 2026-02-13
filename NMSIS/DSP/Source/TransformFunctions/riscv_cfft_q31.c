/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_q31.c
 * Description:  Combined Radix Decimation in Frequency CFFT fixed point processing function
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
extern void riscv_radix4_butterfly_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint32_t twidCoefModifier);

extern void riscv_radix4_butterfly_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint32_t twidCoefModifier);

extern void riscv_bitreversal_32(
        uint32_t * pSrc,
  const uint16_t bitRevLen,
  const uint16_t * pBitRevTable);

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef);

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef);
#endif

/**
  @addtogroup ComplexFFTQ31
  @{
 */

/**
  @brief         Processing function for the Q31 complex FFT.
  @param[in]     S               points to an instance of the fixed-point CFFT structure
  @param[in,out] p1              points to the complex data buffer. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output

@par             Input and Output formats for CFFT Q31

| CFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ---------: | ------------: | -------------: | ------------------------: |
| 16         | 1.31          | 5.27           | 4        
| 64         | 1.31          | 7.25           | 6      
| 256        | 1.31          | 9.23           | 8     
| 1024       | 1.31          | 11.21          | 10      

@par             Input and Output formats for CIFFT Q31

| CIFFT Size  | Input Format  | Output Format  | Number of bits to upscale |
| ----------: | ------------: | -------------: | ------------------------: |
| 16          | 1.31          | 5.27           | 0        
| 64          | 1.31          | 7.25           | 0      
| 256         | 1.31          | 9.23           | 0     
| 1024        | 1.31          | 11.21          | 0      


  @par RVV version
                     The rvv version has a different API.
                     The input and output buffers must be
                     different.
                     There is a temporary buffer.
                     The bit reverse flag is not more 
                     available in rvv version.

  @code
        void riscv_cfft_q31(
  const riscv_cfft_instance_q31 * S,
        const q31_t * src,
        q31_t * dst,
        q31_t *buffer,
        uint8_t ifftFlag
        )
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
  
 */
#if defined(RISCV_MATH_VECTOR) 

RISCV_DSP_ATTRIBUTE void riscv_cfft_q31(
  const riscv_cfft_instance_q31 * S,
        const q31_t * src,
        q31_t * dst,
        q31_t *buffer,
        uint8_t ifftFlag
        )
{
    const long N = S->fftLen;
    int32_t *buf[2] = {buffer, buffer + N * 2};
    int buf_idx = 0; // data in buf_idx

    const int32_t *ptwd_re = S->ptwd_re;
    const int32_t *ptwd_im = S->ptwd_im;

    size_t avl = N >> 1;
    const int32_t *px = src;
    int32_t *py = buf[buf_idx];
    do {
        size_t vl = __riscv_vsetvl_e32m2(avl);

        vint32m2x2_t v_tuple = __riscv_vlseg2e32_v_i32m2x2(px, vl);
        vint32m2_t va_re = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 0);
        vint32m2_t va_im = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 1);
        v_tuple = __riscv_vlseg2e32_v_i32m2x2(px + N, vl);
        vint32m2_t vb_re = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 0);
        vint32m2_t vb_im = __riscv_vget_v_i32m2x2_i32m2(v_tuple, 1);
        px += 2 * vl;

        if (ifftFlag) {
            va_im = __riscv_vneg_v_i32m2(va_im, vl);
            vb_im = __riscv_vneg_v_i32m2(vb_im, vl);
        }

        // scale down to prevent overflow
        va_re = __riscv_vsra_vx_i32m2(va_re, 1, vl);
        va_im = __riscv_vsra_vx_i32m2(va_im, 1, vl);
        vb_re = __riscv_vsra_vx_i32m2(vb_re, 1, vl);
        vb_im = __riscv_vsra_vx_i32m2(vb_im, 1, vl);

        vint32m2_t vre0 = __riscv_vsadd_vv_i32m2(va_re, vb_re, vl);
        vre0 = __riscv_vsra_vx_i32m2(vre0, 1, vl);
        vint32m2_t vim0 = __riscv_vsadd_vv_i32m2(va_im, vb_im, vl);
        vim0 = __riscv_vsra_vx_i32m2(vim0, 1, vl);

        vint32m2_t vtmp_re = __riscv_vssub_vv_i32m2(va_re, vb_re, vl);
        vtmp_re = __riscv_vsra_vx_i32m2(vtmp_re, 1, vl);
        vint32m2_t vtmp_im = __riscv_vssub_vv_i32m2(va_im, vb_im, vl);
        vtmp_im = __riscv_vsra_vx_i32m2(vtmp_im, 1, vl);

        vint32m2_t vtwd_re = __riscv_vle32_v_i32m2(ptwd_re, vl);
        ptwd_re += vl;
        vint32m2_t vtwd_im = __riscv_vle32_v_i32m2(ptwd_im, vl);
        ptwd_im += vl;

        vint32m2_t vre1 = __riscv_vssub_vv_i32m2(
            __riscv_vsmul_vv_i32m2(vtmp_re, vtwd_re, __RISCV_VXRM_RNU, vl),
            __riscv_vsmul_vv_i32m2(vtmp_im, vtwd_im, __RISCV_VXRM_RNU, vl), vl);
        vint32m2_t vim1 = __riscv_vsadd_vv_i32m2(
            __riscv_vsmul_vv_i32m2(vtmp_im, vtwd_re, __RISCV_VXRM_RNU, vl),
            __riscv_vsmul_vv_i32m2(vtmp_re, vtwd_im, __RISCV_VXRM_RNU, vl), vl);

        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vre0);
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vre1);
        __riscv_vsseg2e32_v_i32m2x2(py, v_tuple, vl);
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vim0);
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vim1);
        __riscv_vsseg2e32_v_i32m2x2(py + N, v_tuple, vl);
        py += 2 * vl;

        avl -= vl;
    } while (avl != 0);

    for (int a = N >> 1, stage = 0; a > 2; a >>= 1, stage++) {
        avl = N >> 1;
        const int32_t *px = buf[buf_idx];
        int32_t *py = buf[1 - buf_idx];
            do {
                size_t vl = __riscv_vsetvl_e32m2(avl);

                vint32m2_t va_re = __riscv_vle32_v_i32m2(px, vl);
                vint32m2_t va_im = __riscv_vle32_v_i32m2(px + N, vl);
                vint32m2_t vb_re = __riscv_vle32_v_i32m2(px + N / 2, vl);
                vint32m2_t vb_im = __riscv_vle32_v_i32m2(px + N * 3 / 2, vl);
                px += vl;

                vint32m2_t vre0 = __riscv_vadd_vv_i32m2(va_re, vb_re, vl);
                vre0 = __riscv_vsra_vx_i32m2(vre0, 1, vl);
                vint32m2_t vim0 = __riscv_vadd_vv_i32m2(va_im, vb_im, vl);
                vim0 = __riscv_vsra_vx_i32m2(vim0, 1, vl);

                vint32m2_t vtmp_re = __riscv_vsub_vv_i32m2(va_re, vb_re, vl);
                vtmp_re = __riscv_vsra_vx_i32m2(vtmp_re, 1, vl);
                vint32m2_t vtmp_im = __riscv_vsub_vv_i32m2(va_im, vb_im, vl);
                vtmp_im = __riscv_vsra_vx_i32m2(vtmp_im, 1, vl);

                vint32m2_t vtwd_re = __riscv_vle32_v_i32m2(ptwd_re, vl);
                ptwd_re += vl;
                vint32m2_t vtwd_im = __riscv_vle32_v_i32m2(ptwd_im, vl);
                ptwd_im += vl;

                vint32m2_t vre1 = __riscv_vssub_vv_i32m2(
                    __riscv_vsmul_vv_i32m2(vtmp_re, vtwd_re, __RISCV_VXRM_RNU,
                                           vl),
                    __riscv_vsmul_vv_i32m2(vtmp_im, vtwd_im, __RISCV_VXRM_RNU,
                                           vl),
                    vl);
                vint32m2_t vim1 = __riscv_vsadd_vv_i32m2(
                    __riscv_vsmul_vv_i32m2(vtmp_re, vtwd_im, __RISCV_VXRM_RNU,
                                           vl),
                    __riscv_vsmul_vv_i32m2(vtmp_im, vtwd_re, __RISCV_VXRM_RNU,
                                           vl),
                    vl);

                vint32m2x2_t v_tuple;
                v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vre0);
                v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vre1);
                __riscv_vsseg2e32_v_i32m2x2(py, v_tuple, vl);
                v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vim0);
                v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vim1);
                __riscv_vsseg2e32_v_i32m2x2(py + N, v_tuple, vl);
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
        size_t vl = __riscv_vsetvl_e32m2(avl);

        vint32m2_t va_re = __riscv_vle32_v_i32m2(px, vl);
        vint32m2_t va_im = __riscv_vle32_v_i32m2(px + N, vl);
        vint32m2_t vb_re = __riscv_vle32_v_i32m2(px + N / 2, vl);
        vint32m2_t vb_im = __riscv_vle32_v_i32m2(px + N * 3 / 2, vl);
        px += vl;

        vint32m2_t vre0 = __riscv_vadd_vv_i32m2(va_re, vb_re, vl);
        vint32m2_t vim0 = __riscv_vadd_vv_i32m2(va_im, vb_im, vl);

        vint32m2_t vre1 = __riscv_vsub_vv_i32m2(va_re, vb_re, vl);
        vint32m2_t vim1 = __riscv_vsub_vv_i32m2(va_im, vb_im, vl);

        if (ifftFlag) {
            vim0 = __riscv_vneg_v_i32m2(vim0, vl);
            vim1 = __riscv_vneg_v_i32m2(vim1, vl);
        }

        vuint16m1_t vidx = __riscv_vle16_v_u16m1(pidx, vl);
        vint32m2x2_t v_tuple;
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vre0);
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vim0);
        __riscv_vsoxseg2ei16_v_i32m2x2(py, vidx, v_tuple, vl);

        vidx = __riscv_vle16_v_u16m1(pidx + (N >> 1), vl);
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 0, vre1);
        v_tuple = __riscv_vset_v_i32m2_i32m2x2(v_tuple, 1, vim1);

        pidx += vl;
        __riscv_vsoxseg2ei16_v_i32m2x2(py, vidx, v_tuple, vl);

        avl -= vl;
    } while (avl != 0);
}

#else
RISCV_DSP_ATTRIBUTE void riscv_cfft_q31(
  const riscv_cfft_instance_q31 * S,
        q31_t * p1,
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
       riscv_radix4_butterfly_inverse_q31 ( p1, L, (q31_t*)S->pTwiddle, 1 );
       break;

     case 32:
     case 128:
     case 512:
     case 2048:
       riscv_cfft_radix4by2_inverse_q31 ( p1, L, S->pTwiddle );
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
       riscv_radix4_butterfly_q31 ( p1, L, (q31_t*)S->pTwiddle, 1 );
       break;

     case 32:
     case 128:
     case 512:
     case 2048:
       riscv_cfft_radix4by2_q31 ( p1, L, S->pTwiddle );
       break;
     }
  }

  if ( bitReverseFlag )
    riscv_bitreversal_32 ((uint32_t*) p1, S->bitRevLength, S->pBitRevTable);
}
#endif

/**
  @} end of ComplexFFTQ31 group
 */

#if !defined(RISCV_MATH_VECTOR)
RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef)
{
        uint32_t i, l;
        uint32_t n2;
        q31_t xt, yt, cosVal, sinVal;
        q31_t p0, p1;

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
  const q31_t *pC = pCoef;
        q31_t *pSi = pSrc;
        q31_t *pSl = pSrc + fftLen;
        q63_t T, S, R;
        q63_t coeff, out1, out2;
        q63_t temp0, temp1;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
  n2 = fftLen >> 1U;

  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_RADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_RADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_SUB32(T, S);

    write_q31x2_ia (&pSi, __RV_RADD32(T, S));

    out1 = __RV_KMDA32(coeff, R);
    out2 = __RV_SMXDS32(R, coeff);

    write_q31x2_ia (&pSl, __RV_PKTT32(out2, out1));
#else
#if defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_DRADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_DRADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_DSUB32(T, S);

    write_q31x2_ia (&pSi, __RV_DRADD32(T, S));

    out1 = __RV_DKMDA32(coeff, R);
    out2 = __RV_DSMXDS32(R, coeff);

    write_q31x2_ia (&pSl, __RV_DPKTT32(out2, out1));
#else
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt =          (pSrc[2 * i] >> 2U) - (pSrc[2 * l] >> 2U);
     pSrc[2 * i] = (pSrc[2 * i] >> 2U) + (pSrc[2 * l] >> 2U);

     yt =              (pSrc[2 * i + 1] >> 2U) - (pSrc[2 * l + 1] >> 2U);
     pSrc[2 * i + 1] = (pSrc[2 * l + 1] >> 2U) + (pSrc[2 * i + 1] >> 2U);

     mult_32x32_keep32_R(p0, xt, cosVal);
     mult_32x32_keep32_R(p1, yt, cosVal);
     multAcc_32x32_keep32_R(p0, yt, sinVal);
     multSub_32x32_keep32_R(p1, xt, sinVal);

     pSrc[2 * l]     = p0 << 1;
     pSrc[2 * l + 1] = p1 << 1;
#endif /* defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }


  /* first col */
  riscv_radix4_butterfly_q31 (pSrc,          n2, (q31_t*)pCoef, 2U);

  /* second col */
  riscv_radix4_butterfly_q31 (pSrc + fftLen, n2, (q31_t*)pCoef, 2U);

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
     temp0 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
#if defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3)
     temp0 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
     p0 = pSrc[4 * i + 0];
     p1 = pSrc[4 * i + 1];
     xt = pSrc[4 * i + 2];
     yt = pSrc[4 * i + 3];

     p0 <<= 1U;
     p1 <<= 1U;
     xt <<= 1U;
     yt <<= 1U;

     pSrc[4 * i + 0] = p0;
     pSrc[4 * i + 1] = p1;
     pSrc[4 * i + 2] = xt;
     pSrc[4 * i + 3] = yt;
#endif /* defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }

}

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix4by2_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef)
{
  uint32_t i, l;
  uint32_t n2;
  q31_t xt, yt, cosVal, sinVal;
  q31_t p0, p1;

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
  const q31_t *pC = pCoef;
        q31_t *pSi = pSrc;
        q31_t *pSl = pSrc + fftLen;
        q63_t T, S, R;
        q63_t coeff, out1, out2;
        q63_t temp0, temp1;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_RADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_RADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_SUB32(T, S);

    write_q31x2_ia (&pSi, __RV_RADD32(T, S));

    out1 = __RV_SMDRS32(coeff, R);
    out2 = __RV_KMXDA32(R, coeff);

    write_q31x2_ia (&pSl, __RV_PKTT32(out2, out1));
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_DRADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_DRADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_DSUB32(T, S);

    write_q31x2_ia (&pSi, __RV_DRADD32(T, S));

    out1 = __RV_DSMDRS32(coeff, R);
    out2 = __RV_DKMXDA32(R, coeff);

    write_q31x2_ia (&pSl, __RV_DPKTT32(out2, out1));
#else
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt =          (pSrc[2 * i] >> 2U) - (pSrc[2 * l] >> 2U);
     pSrc[2 * i] = (pSrc[2 * i] >> 2U) + (pSrc[2 * l] >> 2U);

     yt =              (pSrc[2 * i + 1] >> 2U) - (pSrc[2 * l + 1] >> 2U);
     pSrc[2 * i + 1] = (pSrc[2 * l + 1] >> 2U) + (pSrc[2 * i + 1] >> 2U);

     mult_32x32_keep32_R(p0, xt, cosVal);
     mult_32x32_keep32_R(p1, yt, cosVal);
     multSub_32x32_keep32_R(p0, yt, sinVal);
     multAcc_32x32_keep32_R(p1, xt, sinVal);

     pSrc[2 * l]     = p0 << 1U;
     pSrc[2 * l + 1] = p1 << 1U;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }

  /* first col */
  riscv_radix4_butterfly_inverse_q31( pSrc,          n2, (q31_t*)pCoef, 2U);

  /* second col */
  riscv_radix4_butterfly_inverse_q31( pSrc + fftLen, n2, (q31_t*)pCoef, 2U);

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
     temp0 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
     temp0 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
     p0 = pSrc[4 * i + 0];
     p1 = pSrc[4 * i + 1];
     xt = pSrc[4 * i + 2];
     yt = pSrc[4 * i + 3];

     p0 <<= 1U;
     p1 <<= 1U;
     xt <<= 1U;
     yt <<= 1U;

     pSrc[4 * i + 0] = p0;
     pSrc[4 * i + 1] = p1;
     pSrc[4 * i + 2] = xt;
     pSrc[4 * i + 3] = yt;
#endif /* defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }
}
#endif /* defined RISCV_MATH_VECTOR */
