/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mse_f32.c
 * Description:  Floating point mean square error
 *
 * $Date:        05 April 2022
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
  @brief         Mean square error between two floating point vectors.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     blockSize   number of samples in input vector
  @param[out]    pResult      mean square error
  @return        none
 */









void riscv_mse_f32(
    const float32_t * pSrcA,
    const float32_t * pSrcB,
    uint32_t    blockSize,
    float32_t * pResult)

{
  uint32_t blkCnt;                               /* Loop counter */
  float32_t inA, inB;
  float32_t sum = 0.0f;                          /* Temporary return variable */

#if defined(RISCV_MATH_VECTOR)
    size_t l;
    const float32_t *pInA = pSrcA;
    const float32_t *pInB = pSrcB;
    vfloat32m8_t v_inA, v_inB, v_subVal;
    vfloat32m8_t v_mul;
    l = __riscv_vsetvl_e32m1(1);
    vfloat32m1_t v_sum = __riscv_vfmv_s_f_f32m1(0, l);
    blkCnt = blockSize;
    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        v_inA = __riscv_vle32_v_f32m8(pInA, l);
        pInA += l;
        v_inB = __riscv_vle32_v_f32m8(pInB, l);
        pInB += l;
        v_subVal = __riscv_vfsub_vv_f32m8(v_inA, v_inB, l);
        v_mul = __riscv_vfmul_vv_f32m8(v_subVal, v_subVal, l);
        v_sum = __riscv_vfredusum_vs_f32m8_f32m1(v_mul, v_sum, l);
    }
    sum += __riscv_vfmv_f_s_f32m1_f32(v_sum);
#else

#if defined (RISCV_MATH_LOOPUNROLL)
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = (blockSize) >> 2;

  /* First part of the processing with loop unrolling. Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while (blkCnt > 0U)
  {

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = inA - inB;
    sum += inA * inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = inA - inB;
    sum += inA * inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = inA - inB;
    sum += inA * inA;

    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = inA - inB;
    sum += inA * inA;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = (blockSize) & 3;
#else
  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;
#endif

  while (blkCnt > 0U)
  {
    inA = *pSrcA++;
    inB = *pSrcB++;
    inA = inA - inB;
    sum += inA * inA;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */

  /* Store result in destination buffer */
  *pResult = sum / blockSize;
}

/**
  @} end of MSE group
 */
