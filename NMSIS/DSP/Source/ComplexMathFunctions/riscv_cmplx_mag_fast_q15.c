/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mag_fast_q15.c
 * Description:  Q15 complex magnitude
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
  @brief         Q15 complex magnitude.
  @param[in]     pSrc        points to input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function implements 1.15 by 1.15 multiplications and finally output is converted into 2.14 format.
                   Fast functions are less accurate. This function will tend to clamp to 0
                   the too small values. So sqrt(x*x) = x will not always be true.
 */
RISCV_DSP_ATTRIBUTE void riscv_cmplx_mag_fast_q15(
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = numSamples;                               /* Loop counter */
  size_t l;
  vint16m2x2_t v_tuple;
  vint16m2_t v_R, v_I, v_res;
  vint32m4_t v_R2, v_I2, v_sum, v_sum2;              /* Note: Here, the sum used is int32, which may not be same as the scalar version */
  vfloat32m4_t tmp00m4;
  for (; (l = __riscv_vsetvl_e16m2(blkCnt)) > 0; blkCnt -= l)
  {
    v_tuple = __riscv_vlseg2e16_v_i16m2x2(pSrc, l);
    v_R = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 0);
    v_I = __riscv_vget_v_i16m2x2_i16m2(v_tuple, 1);
    pSrc += l * 2;
    v_R2 = __riscv_vwmul_vv_i32m4(v_R, v_R, l);
    v_I2 = __riscv_vwmul_vv_i32m4(v_I, v_I, l);
    v_sum = __riscv_vsadd_vv_i32m4(v_R2, v_I2, l);
    v_sum2 = __riscv_vsra_vx_i32m4(v_sum, 17, l);

    tmp00m4 = __riscv_vfcvt_f_x_v_f32m4(v_sum2, l);
    tmp00m4 = __riscv_vfmul_vf_f32m4(tmp00m4, 1.0 / 0x8000, l);
    tmp00m4 = __riscv_vfsqrt_v_f32m4(tmp00m4, l);
    tmp00m4 = __riscv_vfmul_vf_f32m4(tmp00m4, 0x8000, l);
    v_sum2 = __riscv_vfcvt_x_f_v_i32m4(tmp00m4, l);

    v_res = __riscv_vnsra_wx_i16m2(v_sum2, 0, l);
    __riscv_vse16_v_i16m2(pDst, v_res, l);
    pDst += l;
  }
#else
#if defined (RISCV_MATH_DSP)
        q31_t in;
        q31_t acc0;                                    /* Accumulators */
#else
       q15_t real, imag;                              /* Temporary input variables */
       q31_t acc0, acc1;                              /* Accumulators */
#endif

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] = sqrt(A[0] * A[0] + A[1] * A[1]) */

#if defined (RISCV_MATH_DSP)
    in = read_q15x2_ia((q15_t **)&pSrc);
    acc0 = __SMUAD(in, in);
    /* store result in 2.14 format in destination buffer. */
    riscv_sqrt_q15((q15_t) (acc0 >> 17), pDst++);

    in = read_q15x2_ia((q15_t **)&pSrc);
    acc0 = __SMUAD(in, in);
    riscv_sqrt_q15((q15_t) (acc0 >> 17), pDst++);

    in = read_q15x2_ia((q15_t **)&pSrc);
    acc0 = __SMUAD(in, in);
    riscv_sqrt_q15((q15_t) (acc0 >> 17), pDst++);

    in = read_q15x2_ia ((q15_t **)&pSrc);
    acc0 = __SMUAD(in, in);
    riscv_sqrt_q15((q15_t) (acc0 >> 17), pDst++);
#else
    real = *pSrc++;
    imag = *pSrc++;
    acc0 = ((q31_t) real * real);
    acc1 = ((q31_t) imag * imag);

    /* store result in 2.14 format in destination buffer. */
    riscv_sqrt_q15((q15_t) (((q63_t) acc0 + acc1) >> 17), pDst++);

    real = *pSrc++;
    imag = *pSrc++;
    acc0 = ((q31_t) real * real);
    acc1 = ((q31_t) imag * imag);
    riscv_sqrt_q15((q15_t) (((q63_t) acc0 + acc1) >> 17), pDst++);

    real = *pSrc++;
    imag = *pSrc++;
    acc0 = ((q31_t) real * real);
    acc1 = ((q31_t) imag * imag);
    riscv_sqrt_q15((q15_t) (((q63_t) acc0 + acc1) >> 17), pDst++);

    real = *pSrc++;
    imag = *pSrc++;
    acc0 = ((q31_t) real * real);
    acc1 = ((q31_t) imag * imag);
    riscv_sqrt_q15((q15_t) (((q63_t) acc0 + acc1) >> 17), pDst++);
#endif /* #if defined (RISCV_MATH_DSP) */

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

#if defined (RISCV_MATH_DSP)
    in = read_q15x2_ia ((q15_t **)&pSrc);
    acc0 = __SMUAD(in, in);

    /* store result in 2.14 format in destination buffer. */
    riscv_sqrt_q15((q15_t) (acc0 >> 17), pDst++);
#else
    real = *pSrc++;
    imag = *pSrc++;
    acc0 = ((q31_t) real * real);
    acc1 = ((q31_t) imag * imag);

    /* store result in 2.14 format in destination buffer. */
    riscv_sqrt_q15((q15_t) (((q63_t) acc0 + acc1) >> 17), pDst++);
#endif

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of cmplx_mag group
 */
