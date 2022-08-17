/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_dot_prod_q7.c
 * Description:  Q7 dot product
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicDotProd
  @{
 */

/**
  @brief         Dot product of Q7 vectors.
  @param[in]     pSrcA      points to the first input vector
  @param[in]     pSrcB      points to the second input vector
  @param[in]     blockSize  number of samples in each vector
  @param[out]    result     output result returned here
  @return        none

  @par           Scaling and Overflow Behavior
                   The intermediate multiplications are in 1.7 x 1.7 = 2.14 format and these
                   results are added to an accumulator in 18.14 format.
                   Nonsaturating additions are used and there is no danger of wrap around as long as
                   the vectors are less than 2^18 elements long.
                   The return result is in 18.14 format.
 */

void riscv_dot_prod_q7(
  const q7_t * pSrcA,
  const q7_t * pSrcB,
        uint32_t blockSize,
        q31_t * result)
{
  uint32_t blkCnt;                               /* Loop counter */
  q31_t sum = 0;                                 /* Temporary return variable */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint8m4_t v_inA, v_inB;
  l = vsetvl_e32m1(1);
  vint32m1_t temp00 = vmv_v_x_i32m1(0, l);
  for (; (l = vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_inA = vle8_v_i8m4(pSrcA, l);
    pSrcA += l;
    v_inB = vle8_v_i8m4(pSrcB, l);
    pSrcB += l;
    temp00 = vwredsum_vs_i16m8_i32m1(temp00, vwmul_vv_i16m8(v_inA, v_inB, l), temp00, l);
  }
  sum = vmv_x_s_i32m1_i32(temp00);
#else

#if defined (RISCV_MATH_LOOPUNROLL)
  uint32_t remain_blkCnt;
  q63_t sum64 = 0;
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
  q63_t input1, input2;                          /* Temporary variables */
  /* Loop unrolling: Compute 8 outputs at a time */
  blkCnt = blockSize >> 3U;
  remain_blkCnt = blockSize & 0x7U;
#else
  blkCnt = blockSize >> 2U;
  remain_blkCnt = blockSize & 0x3U;
#endif /* RISCV_MATH_DSP && (NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */

  while (blkCnt > 0U)
  {
    /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    /* read 8 samples at a time from sourceA */
    input1 = read_q7x8_ia((q7_t **)&pSrcA);
    /* read 8 samples at a time from sourceB */
    input2 = read_q7x8_ia((q7_t **)&pSrcB);
    sum64 = __RV_SMAQA(sum64, input1, input2);
#else
#ifdef NUCLEI_DSP_N3
    /* read 8 samples at a time from sourceA */
    input1 = read_q7x8_ia((q7_t **)&pSrcA);
    /* read 8 samples at a time from sourceB */
    input2 = read_q7x8_ia((q7_t **)&pSrcB);
    sum64 = __ddsmaqa(sum64, input1, input2);
#else
    /* read 4 samples at a time from sourceA */
    input1 = read_q7x4_ia((q7_t **)&pSrcA);
    /* read 4 samples at a time from sourceB */
    input2 = read_q7x4_ia((q7_t **)&pSrcB);
    sum = __RV_SMAQA(sum, input1, input2);
#endif /* NUCLEI_DSP_N3 */
#endif /* __RISCV_XLEN == 64 */

#else
    sum += (q31_t) ((q15_t)*pSrcA++ * *pSrcB++);
    sum += (q31_t) ((q15_t)*pSrcA++ * *pSrcB++);
    sum += (q31_t) ((q15_t)*pSrcA++ * *pSrcB++);
    sum += (q31_t) ((q15_t)*pSrcA++ * *pSrcB++);
#endif /* RISCV_MATH_DSP */

    /* Decrement loop counter */
    blkCnt--;
  }
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
  sum += ((sum64 + (sum64 << 32u)) >> 32u);
#endif

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = remain_blkCnt;
#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */

    /* Calculate dot product and store result in a temporary buffer. */
//#if defined (RISCV_MATH_DSP)
//    sum  = __SMLAD(*pSrcA++, *pSrcB++, sum);
//#else
    sum += (q31_t) ((q15_t) *pSrcA++ * *pSrcB++);
//#endif

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store result in destination buffer in 18.14 format */
  *result = sum;
}

/**
  @} end of BasicDotProd group
 */
