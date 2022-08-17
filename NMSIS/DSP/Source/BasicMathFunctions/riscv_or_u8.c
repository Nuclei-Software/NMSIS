/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_or_u8.c
 * Description:  uint8_t bitwise inclusive OR
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
  @addtogroup Or
  @{
 */

/**
  @brief         Compute the logical bitwise OR of two fixed-point vectors.
  @param[in]     pSrcA      points to input vector A
  @param[in]     pSrcB      points to input vector B
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void riscv_or_u8(
    const uint8_t * pSrcA,
    const uint8_t * pSrcB,
          uint8_t * pDst,
          uint32_t blockSize)
{
  uint32_t blkCnt;      /* Loop counter */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#if defined(RISCV_MATH_VECTOR)
  size_t l;
  vuint8m8_t vx, vy;

  for (; (l = vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
  {
    vx = vle8_v_u8m8(pSrcA, l);
    pSrcA += l;
    vy = vle8_v_u8m8(pSrcB, l);
    pSrcB += l;
    vse8_v_u8m8(pDst, vor_vv_u8m8(vx, vy, l), l);
    pDst += l;
  }
#else

  const uint64_t * pSrcA_temp = (const uint64_t *)pSrcA;
  const uint64_t * pSrcB_temp = (const uint64_t *)pSrcB;
  uint64_t * pDst_temp = (uint64_t *)pDst;
  if (blkCnt = blockSize >> 3)
  {
      while (blkCnt > 0U)
      {
          *pDst_temp++ = (*pSrcA_temp++) | (*pSrcB_temp++);
          /* Decrement the loop counter */
          blkCnt--;
      }
  }

  if (blkCnt = blockSize & 0x7U)
  {
      pSrcA = (uint8_t *)(pSrcA_temp - 7);
      pSrcB = (uint8_t *)(pSrcB_temp - 7);
  }

  while (blkCnt > 0U)
  {
      *pDst++ = (*pSrcA++) | (*pSrcB++);
      /* Decrement the loop counter */
      blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}
/**
  @} end of Or group
 */
