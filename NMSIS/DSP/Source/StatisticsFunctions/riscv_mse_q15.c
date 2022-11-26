/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mse_q15.c
 * Description:  Mean square error between two Q15 vectors
 *
 * $Date:        04 April 2022
 * $Revision:    V1.10.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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
  @addtogroup MSE
  @{
 */

/**
  @brief         Mean square error between two Q15 vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    pResult     mean square error
  @return        none
 */
void riscv_mse_q15(
  const q15_t * pSrcA,
  const q15_t * pSrcB,
        uint32_t blockSize,
        q15_t * pResult)
{
  uint32_t blkCnt;                       /* Loop counter */
  q63_t sum = 0;                         /* Temporary result storage */
  q15_t inA, inB;                        /* Temporary variable to store input value */

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  blkCnt = blockSize;
  size_t l;
  const q15_t *pInA = pSrcA;
  const q15_t *pInB = pSrcB;
  vint16m4_t v_inA, v_inB, v_subVal;
  vint32m8_t v_mul;
  l = vsetvl_e64m1(1);
  vint64m1_t v_sum = vmv_s_x_i64m1(v_sum, 0, l); /* init v_sum data */

  for (; (l = vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_inA = vle16_v_i16m4(pInA, l);
    v_inA = vsra_vx_i16m4(v_inA, 1, l);
    pInA += l;
    v_inB = vle16_v_i16m4(pInB, l);
    v_inB = vsra_vx_i16m4(v_inB, 1, l);
    pInB += l;
    v_subVal = vssub_vv_i16m4(v_inA, v_inB, l);
    v_mul = vwmul_vv_i32m8(v_subVal, v_subVal, l);
    v_sum = vwredsum_vs_i32m8_i64m1(v_sum, v_mul, v_sum, l);
  }
  sum += vmv_x_s_i64m1_i64(v_sum);
#else

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP)
  q31_t inA1, inB1, inA2;
#endif /* defined (RISCV_MATH_DSP) */
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {

#if defined (RISCV_MATH_DSP)
    inA1 = read_q15x2_ia((q15_t **)&pSrcA);
    inB1 = read_q15x2_ia((q15_t **)&pSrcB);
    inA1 = __RV_SRAI16(inA1, 1);
    inB1 = __RV_SRAI16(inB1, 1);
    inA2 = __RV_KSUB16(inA1, inB1);
    sum  = __RV_SMALDA(sum, inA2, inA2);

    inA1 = read_q15x2_ia((q15_t **)&pSrcA);
    inB1 = read_q15x2_ia((q15_t **)&pSrcB);
    inA1 = __RV_SRAI16(inA1, 1);
    inB1 = __RV_SRAI16(inB1, 1);
    inA2 = __RV_KSUB16(inA1, inB1);
    sum  = __RV_SMALDA(sum, inA2, inA2);
#else
    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q15_t)__SSAT(((q31_t)inA - (q31_t)inB), 16);
    sum += (q63_t)((q31_t)inA * inA);

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q15_t)__SSAT(((q31_t)inA - (q31_t)inB), 16);
    sum += (q63_t)((q31_t)inA * inA);

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q15_t)__SSAT(((q31_t)inA - (q31_t)inB), 16);
    sum += (q63_t)((q31_t)inA * inA);

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q15_t)__SSAT(((q31_t)inA - (q31_t)inB), 16);
    sum += (q63_t)((q31_t)inA * inA);
#endif /* defined (RISCV_MATH_DSP) */

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

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q15_t) __SSAT(((q31_t)inA - (q31_t)inB), 16);
    sum += (q63_t)((q31_t)inA * inA);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */

  /* Store result in q15 format */
  *pResult = (q15_t) __SSAT((q31_t)(sum / blockSize) >> 13, 16);
}

/**
  @} end of MSE group
 */
