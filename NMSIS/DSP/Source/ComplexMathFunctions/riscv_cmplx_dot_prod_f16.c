/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_dot_prod_f16.c
 * Description:  Floating-point complex dot product
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

#include "dsp/complex_math_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupCmplxMath
 */


/**
  @addtogroup cmplx_dot_prod
  @{
 */

/**
  @brief         Floating-point complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here
  @return        none
 */

void riscv_cmplx_dot_prod_f16(
  const float16_t * pSrcA,
  const float16_t * pSrcB,
        uint32_t numSamples,
        float16_t * realResult,
        float16_t * imagResult)
{
        uint32_t blkCnt;                               /* Loop counter */
        _Float16 real_sum = 0.0f, imag_sum = 0.0f;    /* Temporary result variables */
        _Float16 a0,b0,c0,d0;
#if defined(RISCV_MATH_VECTOR)
  blkCnt = numSamples;                               /* Loop counter */
  size_t l;
  vfloat16m4x2_t v_tupleA, v_tupleB;
  vfloat16m4_t v_R1, v_R2, v_I1, v_I2;
  vfloat16m4_t v_RR, v_II, v_RI, v_IR;
  l = __riscv_vsetvl_e16m1(1);
  vfloat16m1_t v_temp, v_temp1;
  v_temp = __riscv_vfmv_v_f_f16m1(0, l);
  v_temp1 = __riscv_vmv_v_v_f16m1(v_temp, l);
  /* Note the total number of V registers to avoid saturation */
  for (; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_tupleA = __riscv_vlseg2e16_v_f16m4x2 (pSrcA, l);
    v_R1 = __riscv_vget_v_f16m4x2_f16m4(v_tupleA, 0);
    v_I1 = __riscv_vget_v_f16m4x2_f16m4(v_tupleA, 1);
    v_tupleB = __riscv_vlseg2e16_v_f16m4x2 (pSrcB, l);
    v_R2 = __riscv_vget_v_f16m4x2_f16m4(v_tupleB, 0);
    v_I2 = __riscv_vget_v_f16m4x2_f16m4(v_tupleB, 1);

    v_RR = __riscv_vfmul_vv_f16m4(v_R1, v_R2, l);
    v_II = __riscv_vfmul_vv_f16m4(v_I1, v_I2, l);
    v_temp = __riscv_vfredusum_vs_f16m4_f16m1( __riscv_vfsub_vv_f16m4(v_RR, v_II, l), v_temp, l);
    v_RI = __riscv_vfmul_vv_f16m4(v_R1, v_I2, l);
    v_IR = __riscv_vfmul_vv_f16m4(v_I1, v_R2, l);
    v_temp1 = __riscv_vfredusum_vs_f16m4_f16m1(__riscv_vfadd_vv_f16m4(v_RI, v_IR, l), v_temp1, l);

    pSrcA += l * 2;
    pSrcB += l * 2;
  }
  real_sum += __riscv_vfmv_f_s_f16m1_f16(v_temp);
  imag_sum += __riscv_vfmv_f_s_f16m1_f16(v_temp1);
#else

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += a0 * c0;
    imag_sum += a0 * d0;
    real_sum -= b0 * d0;
    imag_sum += b0 * c0;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += a0 * c0;
    imag_sum += a0 * d0;
    real_sum -= b0 * d0;
    imag_sum += b0 * c0;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += a0 * c0;
    imag_sum += a0 * d0;
    real_sum -= b0 * d0;
    imag_sum += b0 * c0;

    a0 = *pSrcA++;
    b0 = *pSrcA++;
    c0 = *pSrcB++;
    d0 = *pSrcB++;

    real_sum += a0 * c0;
    imag_sum += a0 * d0;
    real_sum -= b0 * d0;
    imag_sum += b0 * c0;

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

    real_sum += a0 * c0;
    imag_sum += a0 * d0;
    real_sum -= b0 * d0;
    imag_sum += b0 * c0;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store real and imaginary result in destination buffer. */
  *realResult = real_sum;
  *imagResult = imag_sum;
}

/**
  @} end of cmplx_dot_prod group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
