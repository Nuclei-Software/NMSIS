/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_or_u16.c
 * Description:  uint16_t bitwise inclusive OR
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
  @defgroup Or Vector bitwise inclusive OR

  Compute the logical bitwise OR.

  There are separate functions for uint32_t, uint16_t, and uint8_t data types.
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
 */

RISCV_DSP_ATTRIBUTE void riscv_or_u16(
    const uint16_t * pSrcA,
    const uint16_t * pSrcB,
          uint16_t * pDst,
          uint32_t blockSize)
{
  uint32_t blkCnt;      /* Loop counter */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#if defined(RISCV_MATH_VECTOR)
  size_t l;
  vuint16m8_t vx, vy;

  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    vx = __riscv_vle16_v_u16m8(pSrcA, l);
    pSrcA += l;
    vy = __riscv_vle16_v_u16m8(pSrcB, l);
    pSrcB += l;
    __riscv_vse16_v_u16m8(pDst, __riscv_vor_vv_u16m8(vx, vy, l), l);
    pDst += l;
  }
#else

  const uint64_t * pSrcA_temp = (const uint64_t *)pSrcA;
  const uint64_t * pSrcB_temp = (const uint64_t *)pSrcB;
  uint64_t * pDst_temp = (uint64_t *)pDst;
  uint16_t *pDst_remain = NULL;
  /* Compute 4 outputs at a time */
  if (blkCnt = blockSize >> 2)
  {
    while (blkCnt > 0U)
    {
      *pDst_temp++ = (*pSrcA_temp++) | (*pSrcB_temp++);
      /* Decrement the loop counter */
      blkCnt--;
    }
  }
  /* Tail */
  if (blkCnt = blockSize & 0x3)
  {
    pSrcA = (const uint16_t *)pSrcA_temp;
    pSrcB = (const uint16_t *)pSrcB_temp;
    pDst_remain = (uint16_t *)pDst_temp;
    while (blkCnt > 0U)
    {
      *pDst_remain++ = (*pSrcA++) | (*pSrcB++);
      /* Decrement the loop counter */
      blkCnt--;
    }
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of Or group
 */
