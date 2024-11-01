/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_power_f16.c
 * Description:  Sum of the squares of the elements of a floating-point vector
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

#include "dsp/statistics_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupStats
 */



/**
  @addtogroup power
  @{
 */

/**
  @brief         Sum of the squares of the elements of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    sum of the squares value returned here
 */

RISCV_DSP_ATTRIBUTE void riscv_power_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult)
{
        _Float16 sum = 0.0f16;                          /* Temporary result storage */
        _Float16 in;                                  /* Temporary variable to store input value */
#if defined(RISCV_MATH_VECTOR)
  size_t blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  const float16_t *input = pSrc;
  vfloat16m8_t v_in;
  vfloat16m8_t vsum;
  l = __riscv_vsetvlmax_e16m8();
  vsum = __riscv_vfmv_v_f_f16m8(0.0f, l);
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle16_v_f16m8(input, l);
    input += l;
    vsum = __riscv_vfmacc_vv_f16m8(vsum, v_in, v_in, l);
  }
  l = __riscv_vsetvl_e16m8(1);
  vfloat16m1_t temp00m1 = __riscv_vfmv_v_f_f16m1(0.0f, l);
  l = __riscv_vsetvlmax_e16m8();
  temp00m1 = __riscv_vfredusum_vs_f16m8_f16m1(vsum, temp00m1, l);
  sum += __riscv_vfmv_f_s_f16m1_f16(temp00m1);

#else

#if defined (RISCV_MATH_LOOPUNROLL)
  uint32_t blkCnt;                               /* Loop counter */
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A[0] * A[0] + A[1] * A[1] + ... + A[blockSize-1] * A[blockSize-1] */

    /* Compute Power and store result in a temporary variable, sum. */
    in = *pSrc++;
    sum += in * in;

    in = *pSrc++;
    sum += in * in;

    in = *pSrc++;
    sum += in * in;

    in = *pSrc++;
    sum += in * in;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A[0] * A[0] + A[1] * A[1] + ... + A[blockSize-1] * A[blockSize-1] */

    /* Compute Power and store result in a temporary variable, sum. */
    in = *pSrc++;
    sum += in * in;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store result to destination */
  *pResult = sum;
}

/**
  @} end of power group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

