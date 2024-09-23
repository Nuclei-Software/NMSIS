/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mag_squared_q31.c
 * Description:  Q31 complex magnitude squared
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
  @addtogroup cmplx_mag_squared
  @{
 */

/**
  @brief         Q31 complex magnitude squared.
  @param[in]     pSrc        points to input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function implements 1.31 by 1.31 multiplications and finally output is converted into 3.29 format.
                   Input down scaling is not required.
 */

RISCV_DSP_ATTRIBUTE void riscv_cmplx_mag_squared_q31(
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t numSamples)
{
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  uint32_t blkCnt = numSamples;                               /* Loop counter */
  size_t l;

  ptrdiff_t bstride = 8;
  vint32m4_t v_R, v_I;
  vint32m4_t vR2_m4, vI2_m4;
  vint32m4_t v_sum;
  for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_R = __riscv_vlse32_v_i32m4(pSrc, bstride, l);
    v_I = __riscv_vlse32_v_i32m4(pSrc + 1, bstride, l);
    pSrc += l * 2;
    vR2_m4 = __riscv_vnclip_wx_i32m4(__riscv_vwmul_vv_i64m8(v_R, v_R, l), 33, __RISCV_VXRM_RNU, l);
    vI2_m4 = __riscv_vnclip_wx_i32m4(__riscv_vwmul_vv_i64m8(v_I, v_I, l), 33, __RISCV_VXRM_RNU, l);
    v_sum = __riscv_vsadd_vv_i32m4(vR2_m4, vI2_m4, l);
    __riscv_vse32_v_i32m4(pDst, v_sum, l);
    pDst += l;
  }
#else

  unsigned long blkCnt;                          /* Loop counter */
  q31_t real, imag;                              /* Temporary input variables */
  q31_t acc0, acc1;                              /* Accumulators */

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32))
  q63_t in64;
  q63_t result;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)) */

#if defined (RISCV_MATH_LOOPUNROLL)
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] = (A[0] * A[0] + A[1] * A[1]) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)
    in64 = read_q31x2_ia ((q31_t **) &pSrc);
    result = __RV_DMSR33(in64, in64);
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (__RV_MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__RV_MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

    /* store the result in 3.29 format in the destination buffer. */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32))
    *pDst++ = (q31_t)result + (q31_t)(result >> 32);
#else
    *pDst++ = acc0 + acc1;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)
    in64 = read_q31x2_ia ((q31_t **) &pSrc);
    result = __RV_DMSR33(in64, in64);
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (__RV_MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__RV_MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32))
    *pDst++ = (q31_t)result + (q31_t)(result >> 32);
#else
    *pDst++ = acc0 + acc1;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)
    in64 = read_q31x2_ia ((q31_t **) &pSrc);
    result = __RV_DMSR33(in64, in64);
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (__RV_MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__RV_MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32))
    *pDst++ = (q31_t)result + (q31_t)(result >> 32);
#else
    *pDst++ = acc0 + acc1;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)
    in64 = read_q31x2_ia ((q31_t **) &pSrc);
    result = __RV_DMSR33(in64, in64);
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (__RV_MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__RV_MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32))
    *pDst++ = (q31_t)result + (q31_t)(result >> 32);
#else
    *pDst++ = acc0 + acc1;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)) */

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
    /* C[0] = (A[0] * A[0] + A[1] * A[1]) */

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)
    in64 = read_q31x2_ia ((q31_t **) &pSrc);
    result = __RV_DMSR33(in64, in64);
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (__RV_MULSR64(real, real) >> 33);
    acc1 = (q31_t) (__RV_MULSR64(imag, imag) >> 33);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#else
    real = *pSrc++;
    imag = *pSrc++;

    acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif /* defined (RISCV_MATH_DSP) */

    /* store result in 3.29 format in destination buffer. */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32))
    *pDst++ = (q31_t)result + (q31_t)(result >> 32);
#else
    *pDst++ = acc0 + acc1;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)) */

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
}


/**
  @} end of cmplx_mag_squared group
 */
