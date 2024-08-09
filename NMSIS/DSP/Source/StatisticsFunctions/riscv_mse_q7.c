/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mse_q7.c
 * Description:  Mean square error between two Q7 vectors
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
  @defgroup MSE Mean Square Error

  Calculates the mean square error between two vectors.

 */

/**
  @addtogroup MSE
  @{
 */

/**
  @brief         Mean square error between two Q7 vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    pResult     mean square error
 */
RISCV_DSP_ATTRIBUTE void riscv_mse_q7(
  const q7_t * pSrcA,
  const q7_t * pSrcB,
        uint32_t blockSize,
        q7_t * pResult)
{
        uint32_t blkCnt;                               /* Loop counter */
        q31_t sum = 0;                                 /* Temporary result storage */
        q7_t inA,inB;                                       /* Temporary variable to store input value */

#if defined(RISCV_MATH_VECTOR)
    blkCnt = blockSize;
    size_t l;
    const q7_t *pInA = pSrcA;
    const q7_t *pInB = pSrcB;
    vint8m4_t v_inA, v_inB, v_subVal;
    vint16m8_t v_mul;
    l = __riscv_vsetvl_e32m1(1);
    vint32m1_t v_sum = __riscv_vmv_s_x_i32m1(0, l); /* init v_sum data */
    for (; (l = __riscv_vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
        v_inA = __riscv_vle8_v_i8m4(pInA, l);
        v_inA = __riscv_vsra_vx_i8m4(v_inA, 1, l);
        pInA += l;
        v_inB = __riscv_vle8_v_i8m4(pInB, l);
        v_inB = __riscv_vsra_vx_i8m4(v_inB, 1, l);
        pInB += l;
        v_subVal = __riscv_vssub_vv_i8m4(v_inA, v_inB, l);
        v_mul = __riscv_vwmul_vv_i16m8(v_subVal, v_subVal, l);
        v_sum = __riscv_vwredsum_vs_i16m8_i32m1(v_mul, v_sum, l);

    }
    sum += __riscv_vmv_x_s_i32m1_i32(v_sum);
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
    inA1 = read_q7x4_ia((q7_t **)&pSrcA);
    inB1 = read_q7x4_ia((q7_t **)&pSrcB);
    inA1 = __RV_SRAI8(inA1, 1);
    inB1 = __RV_SRAI8(inB1, 1);
    inA2 = __RV_KSUB8(inA1, inB1);
    sum  = __RV_SMAQA(sum, inA2, inA2);
#else
    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q7_t) __SSAT((q15_t) inA - (q15_t)inB, 8);
    sum += ((q15_t) inA * inA);

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q7_t) __SSAT((q15_t) inA - (q15_t)inB, 8);
    sum += ((q15_t) inA * inA);

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q7_t) __SSAT((q15_t) inA - (q15_t)inB, 8);
    sum += ((q15_t) inA * inA);

    inA = *pSrcA++ >> 1;
    inB = *pSrcB++ >> 1;
    inA = (q7_t) __SSAT((q15_t) inA - (q15_t)inB, 8);
    sum += ((q15_t) inA * inA);
#endif/* defined (RISCV_MATH_DSP) */

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

    inA = (q7_t) __SSAT((q15_t) inA - (q15_t)inB, 8);
    sum += ((q15_t) inA * inA);

    /* Decrement loop counter */
    blkCnt--;
  }

#endif /* defined(RISCV_MATH_VECTOR) */

  /* Store result in q7 format */
  *pResult = (q7_t) __SSAT((q15_t) (sum / blockSize)>>5, 8);;
}

/**
  @} end of MSE group
 */
