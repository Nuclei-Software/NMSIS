/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_not_u16.c
 * Description:  uint16_t bitwise NOT
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
  @defgroup Not Vector bitwise NOT

  Compute the logical bitwise NOT.

  There are separate functions for uint32_t, uint16_t, and uint8_t data types.
 */

/**
  @addtogroup Not
  @{
 */

/**
  @brief         Compute the logical bitwise NOT of a fixed-point vector.
  @param[in]     pSrc       points to input vector
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void riscv_not_u16(
    const uint16_t * pSrc,
          uint16_t * pDst,
          uint32_t blockSize)
{
#if defined(RISCV_MATH_VECTOR)
  uint32_t blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vuint16m8_t vx;

  for (; (l = vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
    vx = vle16_v_u16m8(pSrc, l);
    pSrc += l;
    vse16_v_u16m8(pDst,vnot_v_u16m8(vx, l), l);
    pDst += l;
  }
#else
    uint32_t blkCnt;      /* Loop counter */

#if defined (RISCV_DSP64) || (__RISCV_XLEN == 64)

    uint64_t * pSrc_temp = pSrc;
    uint64_t * pDst_temp = pDst;
    if(blkCnt = blockSize >> 2)
    {
        while (blkCnt > 0U)
        {
            *pDst_temp++ = ~(*pSrc_temp++);

            /* Decrement the loop counter */
            blkCnt--;
        }
    }
    if(blkCnt = blockSize%4)
    {
        pSrc = (uint8_t * )(pSrc_temp-3);
    }

#else
    uint32_t * pSrc_temp = pSrc;
    uint32_t * pDst_temp = pDst;
    if(blkCnt = blockSize >> 1)
    {
        while (blkCnt > 0U)
        {
            *pDst_temp++ = ~(*pSrc_temp++);

            /* Decrement the loop counter */
            blkCnt--;
        }
    }
    if(blkCnt = blockSize%2)
    {
        pSrc = (uint8_t * )(pSrc_temp-1);
    }
#endif

    while (blkCnt > 0U)
    {
        *pDst++ = ~(*pSrc++);

        /* Decrement the loop counter */
        blkCnt--;
    }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of Not group
 */