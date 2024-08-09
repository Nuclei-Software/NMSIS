/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_dot_prod_q31.c
 * Description:  Q31 complex dot product
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
  @brief         Q31 complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The intermediate 1.31 by 1.31 multiplications are performed with 64-bit precision and then shifted to 16.48 format.
                   The internal real and imaginary accumulators are in 16.48 format and provide 15 guard bits.
                   Additions are nonsaturating and no overflow will occur as long as <code>numSamples</code> is less than 32768.
                   The return results <code>realResult</code> and <code>imagResult</code> are in 16.48 format.
                   Input down scaling is not required.
 */

RISCV_DSP_ATTRIBUTE void riscv_cmplx_dot_prod_q31(
  const q31_t * pSrcA,
  const q31_t * pSrcB,
        uint32_t numSamples,
        q63_t * realResult,
        q63_t * imagResult)
{
  uint32_t blkCnt;                        /* Loop counter */
  q63_t real_sum = 0, imag_sum = 0;       /* Temporary result variables */

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  blkCnt = numSamples;                    /* Loop counter */
  size_t l;
  vint32m2x2_t v_tupleA, v_tupleB;
  vint32m2_t v_R1, v_R2, v_I1, v_I2;
  vint64m4_t v_RR, v_II, v_RI, v_IR;

  l = __riscv_vsetvl_e64m1(1);
  vint64m1_t v_temp, v_temp1;
  v_temp = __riscv_vmv_s_x_i64m1(0, l);
  v_temp1 = __riscv_vmv_v_v_i64m1(v_temp, l);
  /* Note the total number of V registers to avoid saturation */
  for (; (l = __riscv_vsetvl_e32m2(blkCnt)) > 0; blkCnt -= l)
  {
    v_tupleA = __riscv_vlseg2e32_v_i32m2x2 (pSrcA, l);
    v_R1 = __riscv_vget_v_i32m2x2_i32m2(v_tupleA, 0);
    v_I1 = __riscv_vget_v_i32m2x2_i32m2(v_tupleA, 1);
    v_tupleB = __riscv_vlseg2e32_v_i32m2x2 (pSrcB, l);
    v_R2 = __riscv_vget_v_i32m2x2_i32m2(v_tupleB, 0);
    v_I2 = __riscv_vget_v_i32m2x2_i32m2(v_tupleB, 1);

    v_RR = __riscv_vsra_vx_i64m4(__riscv_vwmul_vv_i64m4(v_R1, v_R2, l), 14, l);
    v_II = __riscv_vsra_vx_i64m4(__riscv_vwmul_vv_i64m4(v_I1, v_I2, l), 14, l);
    v_temp = __riscv_vredsum_vs_i64m4_i64m1(__riscv_vsub_vv_i64m4(v_RR, v_II, l), v_temp, l);
    v_RI = __riscv_vsra_vx_i64m4(__riscv_vwmul_vv_i64m4(v_R1, v_I2, l), 14, l);
    v_IR = __riscv_vsra_vx_i64m4(__riscv_vwmul_vv_i64m4(v_I1, v_R2, l), 14, l);
    v_temp1 = __riscv_vredsum_vs_i64m4_i64m1(__riscv_vadd_vv_i64m4(v_RI, v_IR, l), v_temp1, l);

    pSrcA += l * 2;
    pSrcB += l * 2;
  }
  real_sum += __riscv_vmv_x_s_i64m1_i64(v_temp);
  imag_sum += __riscv_vmv_x_s_i64m1_i64(v_temp1);
#else
  q31_t a0, b0, c0, d0;

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
  q63_t inA64, inB64;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += (__RV_SMBB32(inA64, inB64) >> 14);
    real_sum -= (__RV_SMTT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inB64, inA64) >> 14);

    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += (__RV_SMBB32(inA64, inB64) >> 14);
    real_sum -= (__RV_SMTT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inB64, inA64) >> 14);

    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += (__RV_SMBB32(inA64, inB64) >> 14);
    real_sum -= (__RV_SMTT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inB64, inA64) >> 14);

    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += (__RV_SMBB32(inA64, inB64) >> 14);
    real_sum -= (__RV_SMTT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inA64, inB64) >> 14);
    imag_sum += (__RV_SMBT32(inB64, inA64) >> 14);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += __RV_DSMBB32_SRA14(inA64, inB64);
    real_sum -= __RV_DSMTT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inB64, inA64);

    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += __RV_DSMBB32_SRA14(inA64, inB64);
    real_sum -= __RV_DSMTT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inB64, inA64);

    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += __RV_DSMBB32_SRA14(inA64, inB64);
    real_sum -= __RV_DSMTT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inB64, inA64);

    inA64 = read_q31x2_ia ((q31_t **) &pSrcA);
    inB64 = read_q31x2_ia ((q31_t **) &pSrcB);
    real_sum += __RV_DSMBB32_SRA14(inA64, inB64);
    real_sum -= __RV_DSMTT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inA64, inB64);
    imag_sum += __RV_DSMBT32_SRA14(inB64, inA64);
#else
    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += ((q63_t)a0 * c0) >> 14;
    imag_sum += ((q63_t)a0 * d0) >> 14;
    real_sum -= ((q63_t)b0 * d0) >> 14;
    imag_sum += ((q63_t)b0 * c0) >> 14;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += ((q63_t)a0 * c0) >> 14;
    imag_sum += ((q63_t)a0 * d0) >> 14;
    real_sum -= ((q63_t)b0 * d0) >> 14;
    imag_sum += ((q63_t)b0 * c0) >> 14;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += ((q63_t)a0 * c0) >> 14;
    imag_sum += ((q63_t)a0 * d0) >> 14;
    real_sum -= ((q63_t)b0 * d0) >> 14;
    imag_sum += ((q63_t)b0 * c0) >> 14;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += ((q63_t)a0 * c0) >> 14;
    imag_sum += ((q63_t)a0 * d0) >> 14;
    real_sum -= ((q63_t)b0 * d0) >> 14;
    imag_sum += ((q63_t)b0 * c0) >> 14;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
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

    real_sum += ((q63_t)a0 * c0) >> 14;
    imag_sum += ((q63_t)a0 * d0) >> 14;
    real_sum -= ((q63_t)b0 * d0) >> 14;
    imag_sum += ((q63_t)b0 * c0) >> 14;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
  /* Store real and imaginary result in 16.48 format  */
  *realResult = real_sum;
  *imagResult = imag_sum;
}

/**
  @} end of cmplx_dot_prod group
 */
