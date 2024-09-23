/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rms_q31.c
 * Description:  Root Mean Square of the elements of a Q31 vector
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

#include "dsp/statistics_functions.h"

/**
  @ingroup groupStats
 */

/**
  @addtogroup RMS
  @{
 */

/**
  @brief         Root Mean Square of the elements of a Q31 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    root mean square value returned here

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The input is represented in 1.31 format, and intermediate multiplication
                   yields a 2.62 format.
                   The accumulator maintains full precision of the intermediate multiplication results,
                   but provides only a single guard bit.
                   There is no saturation on intermediate additions.
                   If the accumulator overflows, it wraps around and distorts the result.
                   In order to avoid overflows completely, the input signal must be scaled down by
                   log2(blockSize) bits, as a total of blockSize additions are performed internally.
                   Finally, the 2.62 accumulator is right shifted by 31 bits to yield a 1.31 format value.
 */
RISCV_DSP_ATTRIBUTE void riscv_rms_q31(
  const q31_t * pSrc,
        uint32_t blockSize,
        q31_t * pResult)
{
  uint32_t blkCnt;         /* Loop counter */
  uint64_t sum = 0;        /* Temporary result storage (can get never negative. changed type from q63 to uint64 */
  q31_t in;                /* Temporary variable to store input value */

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  blkCnt = blockSize;      /* Loop counter */
  size_t l;
  const q31_t * input = pSrc;
  vint32m4_t v_in;
  l = __riscv_vsetvl_e64m1(1);
  vint64m1_t v_sum = __riscv_vmv_s_x_i64m1(0, l); /* init v_sum data */
  for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle32_v_i32m4(input, l);
    input += l;
    v_sum = __riscv_vredsum_vs_i64m8_i64m1(__riscv_vwmul_vv_i64m8(v_in, v_in, l), v_sum, l);
  }
  sum += __riscv_vmv_x_s_i64m1_i64(v_sum);
#else

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A[0] * A[0] + A[1] * A[1] + ... + A[blockSize-1] * A[blockSize-1] */

    in = *pSrc++;
    /* Compute sum of squares and store result in a temporary variable, sum. */
    sum += ((q63_t)in * in);

    in = *pSrc++;
    sum += ((q63_t)in * in);

    in = *pSrc++;
    sum += ((q63_t)in * in);

    in = *pSrc++;
    sum += ((q63_t)in * in);


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

    in = *pSrc++;
    /* Compute sum of squares and store result in a temporary variable. */
    sum += ((q63_t)in * in);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
  /* Convert data in 2.62 to 1.31 by 31 right shifts and saturate */
  /* Compute Rms and store result in destination vector */
  riscv_sqrt_q31(clip_q63_to_q31((sum / (q63_t)blockSize) >> 31), pResult);
}

/**
  @} end of RMS group
 */
