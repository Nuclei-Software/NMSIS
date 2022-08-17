/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_offset_q7.c
 * Description:  Q7 vector offset
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
  @addtogroup BasicOffset
  @{
 */

/**
  @brief         Adds a constant offset to a Q7 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     offset     is the offset to be added
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q7 range [0x80 0x7F] are saturated.
 */
void riscv_offset_q7(
  const q7_t * pSrc,
        q7_t offset,
        q7_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint8m8_t vx;

  for (; (l = vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
  {
    vx = vle8_v_i8m8(pSrc, l);
    pSrc += l;
    vse8_v_i8m8(pDst, vsadd_vx_i8m8(vx, offset, l), l);
    pDst += l;
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)
#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64 || defined (NUCLEI_DSP_N1)
  q63_t offset_packed;                           /* Offset packed to 32 bit */
  q7_t *offset_a = (q7_t*)&offset_packed;

  for (int i = 0; i < 8; i++)
  {
    offset_a[i]	 = offset;
  }


  /* Offset is packed to 64 bit in order to use SIMD64 for addition */
  offset_packed = *((q63_t *)offset_a);
  blkCnt = blockSize >> 3U;
#else
  q31_t offset_packed;
  offset_packed = (q31_t)__EXPD80(offset);
  blkCnt = blockSize >> 3U;
#endif /* __RISCV_XLEN == 64 */
#else
  blkCnt = blockSize >> 2U;
#endif /* RISCV_MATH_DSP */

  while (blkCnt > 0U)
  {
    /* C = A + offset */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    write_q7x8_ia (&pDst, __QADD8(read_q7x8_ia ((q7_t **) &pSrc), offset_packed));
#else
#ifdef NUCLEI_DSP_N1
    /* Add offset and store result in destination buffer (8 samples at a time). */
    write_q7x8_ia (&pDst, __RV_DKADD8(read_q7x8_ia((q7_t **)&pSrc), offset_packed));
#else
    write_q7x4_ia (&pDst, __QADD8(read_q7x4_ia((q7_t **)&pSrc), offset_packed));
    write_q7x4_ia (&pDst, __QADD8(read_q7x4_ia((q7_t **)&pSrc), offset_packed));
#endif /* NUCLEI_DSP_N1 */
#endif /* __RISCV_XLEN == 64 */
#else
    *pDst++ = (q7_t) __SSAT(*pSrc++ + offset, 8);
    *pDst++ = (q7_t) __SSAT(*pSrc++ + offset, 8);
    *pDst++ = (q7_t) __SSAT(*pSrc++ + offset, 8);
    *pDst++ = (q7_t) __SSAT(*pSrc++ + offset, 8);

#endif /* RISCV_MATH_DSP */

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
#if defined (RISCV_MATH_DSP)
  blkCnt = blockSize & 0x7U;
#else
  blkCnt = blockSize & 0x3U;
#endif

#else /* RISCV_MATH_LOOPUNROLL */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A + offset */

    /* Add offset and store result in destination buffer. */
    *pDst++ = (q7_t) __SSAT((q15_t) *pSrc++ + offset, 8);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicOffset group
 */
