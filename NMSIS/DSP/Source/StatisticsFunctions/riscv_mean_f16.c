/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mean_f16.c
 * Description:  Mean value of a floating-point vector
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
  @defgroup mean Mean

  Calculates the mean of the input vector. Mean is defined as the average of the elements in the vector.
  The underlying algorithm is used:

  <pre>
      Result = (pSrc[0] + pSrc[1] + pSrc[2] + ... + pSrc[blockSize-1]) / blockSize;
  </pre>

  There are separate functions for floating-point, Q31, Q15, and Q7 data types.
 */

/**
  @addtogroup mean
  @{
 */

/**
  @brief         Mean value of a floating-point vector.
  @param[in]     pSrc       points to the input vector.
  @param[in]     blockSize  number of samples in input vector.
  @param[out]    pResult    mean value returned here.
 */

RISCV_DSP_ATTRIBUTE void riscv_mean_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult)
{
        float16_t sum = 0.0f;                          /* Temporary result storage */

#if defined(RISCV_MATH_VECTOR)
  size_t blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  const float16_t *input = pSrc;
  vfloat16m8_t v_in;
  l = __riscv_vsetvlmax_e16m8();
  vfloat16m8_t vsum = __riscv_vfmv_v_f_f16m8(0.0f, l);
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
    v_in = __riscv_vle16_v_f16m8(input, l);
    input += l;
    vsum = __riscv_vfadd_vv_f16m8(v_in, vsum, l);
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
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += (_Float16)*pSrc++;

    sum += (_Float16)*pSrc++;

    sum += (_Float16)*pSrc++;

    sum += (_Float16)*pSrc++;

    /* Decrement the loop counter */
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
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += (_Float16)*pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) / blockSize  */
  /* Store result to destination */
  *pResult = ((_Float16)sum / (_Float16)blockSize);
}

/**
  @} end of mean group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

