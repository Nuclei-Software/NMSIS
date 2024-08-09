/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_shift_q31.c
 * Description:  Shifts the elements of a Q31 vector by a specified number of bits
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
  @defgroup BasicShift Vector Shift

  Shifts the elements of a fixed-point vector by a specified number of bits.
  There are separate functions for Q7, Q15, and Q31 data types.
  The underlying algorithm used is:

  <pre>
      pDst[n] = pSrc[n] << shift,   0 <= n < blockSize.
  </pre>

  If <code>shift</code> is positive then the elements of the vector are shifted to the left.
  If <code>shift</code> is negative then the elements of the vector are shifted to the right.

  The functions support in-place computation allowing the source and destination
  pointers to reference the same memory buffer.
 */

/**
  @addtogroup BasicShift
  @{
 */

/**
  @brief         Shifts the elements of a Q31 vector a specified number of bits.
  @param[in]     pSrc       points to the input vector
  @param[in]     shiftBits  number of bits to shift.  A positive value shifts left; a negative value shifts right.
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in the vector

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q31 range [0x80000000 0x7FFFFFFF] are saturated.
 */

RISCV_DSP_ATTRIBUTE void riscv_shift_q31(
  const q31_t * pSrc,
        int8_t shiftBits,
        q31_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  uint8_t sign = (shiftBits & 0x80);             /* Sign of shiftBits */

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint32m4_t vx;
  vint32m8_t vy;
  if (sign == 0U)
  {
    for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l)
    {
      vx = __riscv_vle32_v_i32m4(pSrc, l);
      pSrc += l;
      __riscv_vse32_v_i32m4(pDst, __riscv_vnclip_wx_i32m4(__riscv_vsll_vx_i64m8(__riscv_vwadd_vx_i64m8(vx, 0, l), shiftBits, l), 0, __RISCV_VXRM_RNU, l), l);
      pDst += l;
    }
  }
  else
  {
    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
    {
      vy = __riscv_vle32_v_i32m8(pSrc, l);
      pSrc += l;
      __riscv_vse32_v_i32m8(pDst, __riscv_vsra_vx_i32m8(vy, -shiftBits, l), l);
      pDst += l;
    }
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)

  q31_t in, out;                                 /* Temporary variables */

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
  while (blkCnt > 0U)
  {
    write_q31x2_ia(&pDst, __RV_KSLRA32(read_q31x2_ia((q31_t **)&pSrc), shiftBits));
    write_q31x2_ia(&pDst, __RV_KSLRA32(read_q31x2_ia((q31_t **)&pSrc), shiftBits));
    blkCnt--;
  }
#else
#ifdef NUCLEI_DSP_N2
  while (blkCnt > 0U)
  {
    write_q31x2_ia(&pDst, __dkslra32(read_q31x2_ia((q31_t **)&pSrc), shiftBits));
    write_q31x2_ia(&pDst, __dkslra32(read_q31x2_ia((q31_t **)&pSrc), shiftBits));
    blkCnt--;
  }
#else
  /* If the shift value is positive then do right shift else left shift */
  while (blkCnt > 0U)
  {
    *pDst++ = __RV_KSLRAW(*pSrc++, shiftBits);
    *pDst++ = __RV_KSLRAW(*pSrc++, shiftBits);
    *pDst++ = __RV_KSLRAW(*pSrc++, shiftBits);
    *pDst++ = __RV_KSLRAW(*pSrc++, shiftBits);
    blkCnt--;
  }
#endif /* NUCLEI_DSP_N2 */
#endif /* __RISCV_XLEN == 64 */

#else

  if (sign == 0U)
  {
    while (blkCnt > 0U)
    {
      /* C = A << shiftBits */

      /* Shift input and store result in destination buffer. */
      in = *pSrc++;
      out = in << shiftBits;
      if (in != (out >> shiftBits))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pDst++ = out;

      in = *pSrc++;
      out = in << shiftBits;
      if (in != (out >> shiftBits))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pDst++ = out;

      in = *pSrc++;
      out = in << shiftBits;
      if (in != (out >> shiftBits))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pDst++ = out;

      in = *pSrc++;
      out = in << shiftBits;
      if (in != (out >> shiftBits))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pDst++ = out;

      /* Decrement loop counter */
      blkCnt--;
    }
  }
  else
  {
    while (blkCnt > 0U)
    {
      /* C = A >> shiftBits */

      /* Shift input and store results in destination buffer. */
      *pDst++ = (*pSrc++ >> -shiftBits);
      *pDst++ = (*pSrc++ >> -shiftBits);
      *pDst++ = (*pSrc++ >> -shiftBits);
      *pDst++ = (*pSrc++ >> -shiftBits);

      /* Decrement loop counter */
      blkCnt--;
    }
  }
#endif /* #if defined (RISCV_MATH_DSP) */
  /* Loop unrolling: Compute remaining outputs */
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
      *pDst++ = clip_q63_to_q31((q63_t) *pSrc++ << shiftBits);

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
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
}

/**
  @} end of BasicShift group
 */
