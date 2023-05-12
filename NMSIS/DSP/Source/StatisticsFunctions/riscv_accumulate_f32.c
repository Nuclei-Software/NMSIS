/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_accumulate_f32.c
 * Description:  Sum value of a floating-point vector
 *
 * $Date:        14 July 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: RISC-V and Cortex-A cores
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
 @addtogroup Accumulation
 @{
 */

/**
 @brief         Accumulation value of a floating-point vector.
 @param[in]     pSrc       points to the input vector.
 @param[in]     blockSize  number of samples in input vector.
 @param[out]    pResult    sum of values in input vector.
 @return        none
 */

void riscv_accumulate_f32(
                        const float32_t * pSrc,
                        uint32_t blockSize,
                        float32_t * pResult)
{
  uint32_t blkCnt;                               /* Loop counter */
  float32_t sum = 0.0f;                          /* Temporary result storage */
  
#if defined (RISCV_MATH_LOOPUNROLL)
  
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
  
  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */
    sum += *pSrc++;
    
    sum += *pSrc++;
    
    sum += *pSrc++;
    
    sum += *pSrc++;
    
    /* Decrement the loop counter */
    blkCnt--;
  }
  
  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;
  
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
  
  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1])  */
  /* Store result to destination */
  *pResult = sum ;
}
/**
 @} end of Accumulation group
 */
