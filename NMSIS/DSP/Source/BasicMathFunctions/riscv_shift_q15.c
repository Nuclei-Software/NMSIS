/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_shift_q15.c
 * Description:  Shifts the elements of a Q15 vector by a specified number of bits
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
  @brief         Shifts the elements of a Q15 vector a specified number of bits
  @param[in]     pSrc       points to the input vector
  @param[in]     shiftBits  number of bits to shift.  A positive value shifts left; a negative value shifts right.
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q15 range [0x8000 0x7FFF] are saturated.
 */

RISCV_DSP_ATTRIBUTE void riscv_shift_q15(
  const q15_t * pSrc,
        int8_t shiftBits,
        q15_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  uint8_t sign = (shiftBits & 0x80);             /* Sign of shiftBits */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint16m4_t vx;
  vint16m8_t vy;
  /* If the shift value is positive then do right shift else left shift */
  if (sign == 0U)
  {
    for (; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
    {
      vx = __riscv_vle16_v_i16m4(pSrc, l);
      pSrc += l;
      __riscv_vse16_v_i16m4(pDst, __riscv_vnclip_wx_i16m4(__riscv_vsll_vx_i32m8(__riscv_vwadd_vx_i32m8(vx, 0, l), shiftBits, l), 0, __RISCV_VXRM_RNU, l), l);
      pDst += l;

    }
  }
  else
  {
    for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
    {
      vy = __riscv_vle16_v_i16m8(pSrc, l);
      pSrc += l;
      __riscv_vse16_v_i16m8(pDst, __riscv_vsra_vx_i16m8(vy, -shiftBits, l), l);
      pDst += l;

    }
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
#if defined (RISCV_MATH_DSP)
  while(blkCnt > 0U)
  {
#if __RISCV_XLEN == 64
    write_q15x4_ia(&pDst, __RV_KSLRA16(read_q15x4_ia((q15_t **)&pSrc), shiftBits));
#else
#ifdef NUCLEI_DSP_N1
    write_q15x4_ia(&pDst, __DKSLRA16(read_q15x4_ia((q15_t **)&pSrc), shiftBits));
#else
    write_q15x2_ia(&pDst, __RV_KSLRA16(read_q15x2_ia((q15_t **)&pSrc), shiftBits));
    write_q15x2_ia(&pDst, __RV_KSLRA16(read_q15x2_ia((q15_t **)&pSrc), shiftBits));
#endif /* NUCLEI_DSP_N1 */
#endif /* __RISCV_XLEN == 64 */
    blkCnt--;
  }

#else

  if (sign == 0U)
  {
    while (blkCnt > 0U)
    {
      /* C = A << shiftBits */
      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
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
#endif /* RISCV_MATH_DSP */

  blkCnt = blockSize & 0x3U;

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
      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);

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
