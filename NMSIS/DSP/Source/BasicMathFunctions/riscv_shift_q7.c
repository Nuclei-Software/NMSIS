/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_shift_q7.c
 * Description:  Processing function for the Q7 Shifting
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
  @addtogroup BasicShift
  @{
 */

/**
  @brief         Shifts the elements of a Q7 vector a specified number of bits
  @param[in]     pSrc       points to the input vector
  @param[in]     shiftBits  number of bits to shift.  A positive value shifts left; a negative value shifts right.
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector

  @par           onditions for optimum performance
                   Input and output buffers should be aligned by 32-bit
  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q7 range [0x80 0x7F] are saturated.
 */

RISCV_DSP_ATTRIBUTE void riscv_shift_q7(
  const q7_t * pSrc,
        int8_t shiftBits,
        q7_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  uint8_t sign = (shiftBits & 0x80);             /* Sign of shiftBits */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint8m4_t vx;
  vint8m8_t vy;

  if (sign == 0U)
  {
    for (; (l = __riscv_vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l)
    {
      vx = __riscv_vle8_v_i8m4(pSrc, l);
      pSrc += l;
      __riscv_vse8_v_i8m4(pDst, __riscv_vnclip_wx_i8m4(__riscv_vsll_vx_i16m8(__riscv_vwadd_vx_i16m8(vx, 0, l), shiftBits, l), 0, __RISCV_VXRM_RNU, l), l);
      pDst += l;
    }
  }
  else
  {
    for (; (l = __riscv_vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
    {
      vy = __riscv_vle8_v_i8m8(pSrc, l);
      pSrc += l;
      __riscv_vse8_v_i8m8(pDst, __riscv_vsra_vx_i8m8(vy, -shiftBits, l), l);
      pDst += l;
    }
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP)
  blkCnt = blockSize >> 3U;
  while(blkCnt > 0U)
  {
#if __RISCV_XLEN == 64
    write_q7x8_ia(&pDst, __RV_KSLRA8(read_q7x8_ia((q7_t **)&pSrc), shiftBits));
#else
#ifdef NUCLEI_DSP_N1
    write_q7x8_ia(&pDst, __RV_DKSLRA8(read_q7x8_ia((q7_t **)&pSrc), shiftBits));
#else
    write_q7x4_ia(&pDst, __RV_KSLRA8(read_q7x4_ia((q7_t **)&pSrc), shiftBits));
    write_q7x4_ia(&pDst, __RV_KSLRA8(read_q7x4_ia((q7_t **)&pSrc), shiftBits));
#endif /* NUCLEI_DSP_N1 */
#endif /* __RISCV_XLEN == 64 */
    blkCnt--;
  }
  blkCnt = blockSize & 0x7U;

#else
  blkCnt = blockSize >> 2U;
  if (sign == 0U)
  {
    while (blkCnt > 0U)
    {
      /* C = A << shiftBits */

      *pDst++ = (q7_t) __SSAT(((q15_t)*pSrc++ << shiftBits), 8);
      *pDst++ = (q7_t) __SSAT(((q15_t)*pSrc++ << shiftBits), 8);
      *pDst++ = (q7_t) __SSAT(((q15_t)*pSrc++ << shiftBits), 8);
      *pDst++ = (q7_t) __SSAT(((q15_t)*pSrc++ << shiftBits), 8);
      blkCnt--;
    }
  }
  else
  {
    while (blkCnt > 0U)
    {
      /* C = A >> shiftBits */

      *pDst++ = (*pSrc++ >> -shiftBits);
      *pDst++ = (*pSrc++ >> -shiftBits);
      *pDst++ = (*pSrc++ >> -shiftBits);
      *pDst++ = (*pSrc++ >> -shiftBits);
      blkCnt--;
    }
  }
  blkCnt = blockSize & 0x3U;
#endif /* RISCV_MATH_DSP */

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  /* If the shift value is positive then do right shift else left shift */
  if (sign == 0U)
  {
    while (blkCnt > 0U)
    {
      /* C = A << shiftBits */

      /* Shift input and store result in destination buffer. */
      *pDst++ = (q7_t) __SSAT(((q15_t) *pSrc++ << shiftBits), 8);

      /* Decrement loop counter */
      blkCnt--;
    }
  }
  else
  {
    while (blkCnt > 0U)
    {
      /* C = A >> shiftBits */

      /* Shift input and store result in destination buffer. */
      *pDst++ = (*pSrc++ >> -shiftBits);

      /* Decrement loop counter */
      blkCnt--;
    }
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicShift group
 */
