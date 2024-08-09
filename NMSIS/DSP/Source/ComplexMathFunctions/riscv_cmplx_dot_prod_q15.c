/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_dot_prod_q15.c
 * Description:  Processing function for the Q15 Complex Dot product
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
  @addtogroup cmplx_dot_prod
  @{
 */

/**
  @brief         Q15 complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned her

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The intermediate 1.15 by 1.15 multiplications are performed with full precision and yield a 2.30 result.
                   These are accumulated in a 64-bit accumulator with 34.30 precision.
                   As a final step, the accumulators are converted to 8.24 format.
                   The return results <code>realResult</code> and <code>imagResult</code> are in 8.24 format.
 */

RISCV_DSP_ATTRIBUTE void riscv_cmplx_dot_prod_q15(
  const q15_t * pSrcA,
  const q15_t * pSrcB,
        uint32_t numSamples,
        q31_t * realResult,
        q31_t * imagResult)
{
  unsigned long blkCnt;                   /* Loop counter */
  q63_t real_sum = 0, imag_sum = 0;       /* Temporary result variables */

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  blkCnt = numSamples;                   /* Loop counter */
  size_t l;
  vint16m2x2_t v_tupleA, v_tupleB;
  vint16m2_t v_R1, v_R2, v_I1, v_I2;
  vint32m4_t v_RR, v_II, v_RI, v_IR;
  l = __riscv_vsetvl_e64m1(1);
  vint64m1_t v_temp, v_temp1;
  v_temp = __riscv_vmv_s_x_i64m1(0, l);
  v_temp1 = __riscv_vmv_v_v_i64m1(v_temp, l);
  /* Note the total number of V registers to avoid saturation */
  for (; (l = __riscv_vsetvl_e16m2(blkCnt)) > 0; blkCnt -= l)
  {
    v_tupleA = __riscv_vlseg2e16_v_i16m2x2(pSrcA, l);
    v_R1 = __riscv_vget_v_i16m2x2_i16m2(v_tupleA, 0);
    v_I1 = __riscv_vget_v_i16m2x2_i16m2(v_tupleA, 1);
    v_tupleB = __riscv_vlseg2e16_v_i16m2x2(pSrcB, l);
    v_R2 = __riscv_vget_v_i16m2x2_i16m2(v_tupleB, 0);
    v_I2 = __riscv_vget_v_i16m2x2_i16m2(v_tupleB, 1);

    v_RR = __riscv_vwmul_vv_i32m4(v_R1, v_R2, l);
    v_II = __riscv_vwmul_vv_i32m4(v_I1, v_I2, l);
    v_temp = __riscv_vwredsum_vs_i32m4_i64m1(__riscv_vsub_vv_i32m4(v_RR, v_II, l), v_temp, l);
    v_RI = __riscv_vwmul_vv_i32m4(v_R1, v_I2, l);
    v_IR = __riscv_vwmul_vv_i32m4(v_I1, v_R2, l);
    v_temp1 = __riscv_vwredsum_vs_i32m4_i64m1(__riscv_vadd_vv_i32m4(v_RI, v_IR, l), v_temp1, l);

    pSrcA += l * 2;
    pSrcB += l * 2;
  }
  real_sum += __riscv_vmv_x_s_i64m1_i64(v_temp);
  imag_sum += __riscv_vmv_x_s_i64m1_i64(v_temp1);
#else
  q15_t a0, b0, c0, d0;

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
  q63_t inA, inB;
#else
  q31_t inA, inB;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
#if defined (RISCV_MATH_DSP)
#if (__RISCV_XLEN == 64)
    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);
    real_sum = __RV_SMALDRS(real_sum, inA, inB);
    imag_sum = __SMLALDX(inA, inB, imag_sum);

    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);
    real_sum = __RV_SMALDRS(real_sum, inA, inB);
    imag_sum = __SMLALDX(inA, inB, imag_sum);
#else
#if defined (NUCLEI_DSP_N3)
    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);
    real_sum = __RV_DSMALDRS(real_sum, inA, inB);
    imag_sum = __RV_DSMALXDA(imag_sum, inA, inB);

    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);
    real_sum = __RV_DSMALDRS(real_sum, inA, inB);
    imag_sum = __RV_DSMALXDA(imag_sum, inA, inB);
#else
    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    real_sum += __RV_SMALDRS(0, inA, inB);
    imag_sum += __SMLALDX(inA, inB, 0);

    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    real_sum += __RV_SMALDRS(0, inA, inB);
    imag_sum += __SMLALDX(inA, inB, 0);

    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    real_sum += __RV_SMALDRS(0, inA, inB);
    imag_sum += __SMLALDX(inA,inB, 0);

    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    real_sum += __RV_SMALDRS(0, inA, inB);
    imag_sum += __SMLALDX(inA, inB, 0);

#endif /* defined (NUCLEI_DSP_N3) */
#endif /* (__RISCV_XLEN == 64) */

#else
    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += (q31_t)a0 * c0;
    imag_sum += (q31_t)a0 * d0;
    real_sum -= (q31_t)b0 * d0;
    imag_sum += (q31_t)b0 * c0;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += (q31_t)a0 * c0;
    imag_sum += (q31_t)a0 * d0;
    real_sum -= (q31_t)b0 * d0;
    imag_sum += (q31_t)b0 * c0;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += (q31_t)a0 * c0;
    imag_sum += (q31_t)a0 * d0;
    real_sum -= (q31_t)b0 * d0;
    imag_sum += (q31_t)b0 * c0;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += (q31_t)a0 * c0;
    imag_sum += (q31_t)a0 * d0;
    real_sum -= (q31_t)b0 * d0;
    imag_sum += (q31_t)b0 * c0;

#endif /* defined (RISCV_MATH_DSP) */
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
    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

#if defined(RISCV_MATH_DSP)
    real_sum = __SMALBB(real_sum, a0, c0);
    imag_sum = __SMALBB(imag_sum, a0, d0);
    real_sum = __SMSLDA(real_sum, b0, d0);
    imag_sum = __SMALBB(imag_sum, b0, c0);
#else
    real_sum += (q31_t)a0 * c0;
    imag_sum += (q31_t)a0 * d0;
    real_sum -= (q31_t)b0 * d0;
    imag_sum += (q31_t)b0 * c0;
#endif /* defined(RISCV_MATH_DSP) */

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
  /* Store real and imaginary result in 8.24 format  */
  /* Convert real data in 34.30 to 8.24 by 6 right shifts */
  *realResult = (q31_t) (real_sum >> 6);
  /* Convert imaginary data in 34.30 to 8.24 by 6 right shifts */
  *imagResult = (q31_t) (imag_sum >> 6);
}

/**
  @} end of cmplx_dot_prod group
 */
