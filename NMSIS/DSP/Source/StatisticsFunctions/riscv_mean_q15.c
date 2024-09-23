/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mean_q15.c
 * Description:  Mean value of a Q15 vector
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
  @addtogroup mean
  @{
 */

/**
  @brief         Mean value of a Q15 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    mean value returned here

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 32-bit internal accumulator.
                   The input is represented in 1.15 format and is accumulated in a 32-bit
                   accumulator in 17.15 format.
                   There is no risk of internal overflow with this approach, and the
                   full precision of intermediate result is preserved.
                   Finally, the accumulator is truncated to yield a result of 1.15 format.
 */

RISCV_DSP_ATTRIBUTE void riscv_mean_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult)
{
  unsigned long blkCnt;                          /* Loop counter */
  q31_t sum = 0;                                 /* Temporary result storage */
#if defined (RISCV_MATH_DSP) && ((__RISCV_XLEN == 64) || defined (NUCLEI_DSP_N3))
  q63_t in64, sum64 = 0;
#else
  q31_t in;
#endif /* defined (RISCV_MATH_DSP) && ((__RISCV_XLEN == 64) || defined (NUCLEI_DSP_N3)) */


#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;          /* Loop counter */
  size_t l;
  const q15_t *input = pSrc;
  vint16m8_t v_in;
  l = __riscv_vsetvl_e32m1(1);
  vint32m1_t v_sum = __riscv_vmv_s_x_i32m1(0, l); /* init v_sum data */
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle16_v_i16m8(input, l);
    input += l;
    v_sum = __riscv_vwredsum_vs_i16m8_i32m1(v_in, v_sum, l);
  }
  sum += __riscv_vmv_x_s_i32m1_i32(v_sum);
#else
#if defined (RISCV_MATH_LOOPUNROLL)
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
#ifdef RISCV_MATH_DSP
#if __RISCV_XLEN == 64
    in64 = read_q15x4_ia((q15_t **)&pSrc);
    sum64 = __RV_SMALDA(sum64, in64, 0x0001000100010001);
#else
#if defined (NUCLEI_DSP_N3)
    in64 = read_q15x4_ia((q15_t **)&pSrc);
    sum64 = __RV_DSMALDA(sum64, in64, 0x0001000100010001);
#else
    in = read_q15x2_ia((q15_t **)&pSrc);
    sum = __RV_SMALDA(sum, in, 0x00010001);

    in = read_q15x2_ia((q15_t **)&pSrc);
    sum = __RV_SMALDA(sum, in, 0x00010001);

#endif /* defined(NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    in = read_q15x2_ia((q15_t **)&pSrc);
    sum += ((in << 16U) >> 16U);
    sum += (in >> 16U);

    in = read_q15x2_ia((q15_t **)&pSrc);
    sum += ((in << 16U) >> 16U);
    sum += (in >> 16U);
#endif /* #ifdef RISCV_MATH_DSP */
    /* Decrement the loop counter */
    blkCnt--;
  }
#if defined (RISCV_MATH_DSP) && ((__RISCV_XLEN == 64) || defined (NUCLEI_DSP_N3))
  sum = (q31_t)sum64 + (q31_t)(sum64 >> 32);
#endif /* defined (RISCV_MATH_DSP) && ((__RISCV_XLEN == 64) || defined (NUCLEI_DSP_N3)) */
  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;
#else
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) / blockSize  */
  /* Store result to destination */
  *pResult = (q15_t)(sum / (int32_t)blockSize);
}

/**
  @} end of mean group
 */
