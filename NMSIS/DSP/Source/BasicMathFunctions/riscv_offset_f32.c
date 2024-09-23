/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_offset_f32.c
 * Description:  Floating-point vector offset
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
  @defgroup BasicOffset Vector Offset

  Adds a constant offset to each element of a vector.

  <pre>
      pDst[n] = pSrc[n] + offset,   0 <= n < blockSize.
  </pre>

  The functions support in-place computation allowing the source and
  destination pointers to reference the same memory buffer.
  There are separate functions for floating-point, Q7, Q15, and Q31 data types.
 */

/**
  @addtogroup BasicOffset
  @{
 */

/**
  @brief         Adds a constant offset to a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     offset     is the offset to be added
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
 */

RISCV_DSP_ATTRIBUTE void riscv_offset_f32(
  const float32_t * pSrc,
        float32_t offset,
        float32_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vfloat32m8_t vx;

  for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
    vx = __riscv_vle32_v_f32m8(pSrc, l);
    pSrc += l;
    __riscv_vse32_v_f32m8(pDst, __riscv_vfadd_vf_f32m8(vx, offset, l), l);
    pDst += l;
  }

#else

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A + offset */

    /* Add offset and store result in destination buffer. */
    *pDst++ = (*pSrc++) + offset;

    *pDst++ = (*pSrc++) + offset;

    *pDst++ = (*pSrc++) + offset;

    *pDst++ = (*pSrc++) + offset;

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
    /* C = A + offset */

    /* Add offset and store result in destination buffer. */
    *pDst++ = (*pSrc++) + offset;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicOffset group
 */
