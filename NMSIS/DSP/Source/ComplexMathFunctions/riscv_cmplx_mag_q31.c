/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mag_q31.c
 * Description:  Q31 complex magnitude
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

#include "dsp/complex_math_functions.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @addtogroup cmplx_mag
  @{
 */

/**
  @brief         Q31 complex magnitude.
  @param[in]     pSrc        points to input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function implements 1.31 by 1.31 multiplications and finally output is converted into 2.30 format.
                   Input down scaling is not required.
 */

void riscv_cmplx_mag_q31(
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t numSamples)
{
        unsigned long blkCnt;                          /* Loop counter */
        q31_t real, imag;                              /* Temporary input variables */
        q31_t acc0, acc1;                              /* Accumulators */

#if defined(RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  blkCnt = numSamples;                               /* Loop counter */
  size_t l;
  ptrdiff_t bstride = 8;
  vint32m4x2_t v_tuple;
  vint32m4_t v_R, v_I;
  vint32m4_t v_sum, v_res;
  vfloat32m4_t tmp00m4;

  for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l)
  {
    //vlseg2e32_v_i32m4(&v_R, &v_I, pSrc, l);
    v_tuple = __riscv_vlseg2e32_v_i32m4x2 (pSrc, l);
    v_R = __riscv_vget_v_i32m4x2_i32m4(v_tuple, 0);
    v_I = __riscv_vget_v_i32m4x2_i32m4(v_tuple, 1);
    pSrc += l * 2;
    v_R = __riscv_vnsra_wx_i32m4(__riscv_vwmul_vv_i64m8(v_R, v_R, l), 33, l);
    v_I = __riscv_vnsra_wx_i32m4(__riscv_vwmul_vv_i64m8(v_I, v_I, l), 33, l);
    v_sum = __riscv_vadd_vv_i32m4(v_R, v_I, l);
    tmp00m4 = __riscv_vfcvt_f_x_v_f32m4(v_sum, l);
    tmp00m4 = __riscv_vfdiv_vf_f32m4(tmp00m4, 0x80000000, l);
    tmp00m4 = __riscv_vfsqrt_v_f32m4(tmp00m4, l);
    tmp00m4 = __riscv_vfmul_vf_f32m4(tmp00m4, 0x80000000, l);
    v_res = __riscv_vfcvt_x_f_v_i32m4(tmp00m4, l);
    __riscv_vse32_v_i32m4(pDst, v_res, l);
    pDst += l;
  }
#else

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
  q63_t input, acc;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] = sqrt(A[0] * A[0] + A[1] * A[1]) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    input = read_q31x2_ia ((q31_t **) &pSrc);
#else
    real = *pSrc++;
    imag = *pSrc++;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    acc = __RV_DMSR33(input, input);
#else
    acc0 = (q31_t) (__MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */
#else
    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

    /* store result in 2.30 format in destination buffer. */
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    riscv_sqrt_q31((q31_t)acc + (q31_t)(acc >> 32), pDst++);
#else
    riscv_sqrt_q31(acc0 + acc1, pDst++);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    input = read_q31x2_ia ((q31_t **) &pSrc);
#else
    real = *pSrc++;
    imag = *pSrc++;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    acc = __RV_DMSR33(input, input);
#else
    acc0 = (q31_t) (__MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */
#else
    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    riscv_sqrt_q31((q31_t)acc + (q31_t)(acc >> 32), pDst++);
#else
    riscv_sqrt_q31(acc0 + acc1, pDst++);
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    input = read_q31x2_ia ((q31_t **) &pSrc);
#else
    real = *pSrc++;
    imag = *pSrc++;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    acc = __RV_DMSR33(input, input);
#else
    acc0 = (q31_t) (__MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */
#else
    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    riscv_sqrt_q31((q31_t)acc + (q31_t)(acc >> 32), pDst++);
#else
    riscv_sqrt_q31(acc0 + acc1, pDst++);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    input = read_q31x2_ia ((q31_t **) &pSrc);
#else
    real = *pSrc++;
    imag = *pSrc++;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    acc = __RV_DMSR33(input, input);
#else
    acc0 = (q31_t) (__MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */
#else
    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
     riscv_sqrt_q31((q31_t)acc + (q31_t)(acc >> 32), pDst++);
#else
    riscv_sqrt_q31(acc0 + acc1, pDst++);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = numSamples & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C[0] = sqrt(A[0] * A[0] + A[1] * A[1]) */

#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    input = read_q31x2_ia ((q31_t **) &pSrc);
#else
    real = *pSrc++;
    imag = *pSrc++;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    acc = __RV_DMSR33(input, input);
#else
    acc0 = (q31_t) (__MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */
#else
    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

    /* store result in 2.30 format in destination buffer. */
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32)
    riscv_sqrt_q31((q31_t)acc + (q31_t)(acc >> 32), pDst++);
#else
    riscv_sqrt_q31(acc0 + acc1, pDst++);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) && (__RISCV_XLEN == 32) */

    /* Decrement loop counter */
    blkCnt--;
  }

#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of cmplx_mag group
 */
