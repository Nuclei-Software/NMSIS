/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_shift_q15.c
 * Description:  Shifts the elements of a Q15 vector by a specified number of bits
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: RISC-V cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
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

#include "riscv_math.h"

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
  @return        none

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q15 range [0x8000 0x7FFF] are saturated.
 */

void riscv_shift_q15(
  const q15_t * pSrc,
        int8_t shiftBits,
        q15_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
        uint8_t sign = (shiftBits & 0x80);             /* Sign of shiftBits */

#if defined (RISCV_MATH_LOOPUNROLL)

//#if defined (RISCV_MATH_DSP)
//  q15_t in1, in2;                                /* Temporary input variables */
//#endif

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
  while(blkCnt > 0U) {
 #ifdef RISCV_DSP64
	  write_q15x4_ia(&pDst, __DKSLRA16(read_q15x4_ia((q15_t **)&pSrc), shiftBits));
 #else
	  write_q15x2_ia(&pDst, __KSLRA16(read_q15x2_ia((q15_t **)&pSrc), shiftBits));
	  write_q15x2_ia(&pDst, __KSLRA16(read_q15x2_ia((q15_t **)&pSrc), shiftBits));
 #endif
	  blkCnt--;
  }

  /* If the shift value is positive then do right shift else left shift */
//  if (sign == 0U)
//  {
//    while (blkCnt > 0U)
//    {
//      /* C = A << shiftBits */
//
//#if defined (RISCV_MATH_DSP)
//      /* read 2 samples from source */
//      in1 = *pSrc++;
//      in2 = *pSrc++;
//
//      /* Shift the inputs and then store the results in the destination buffer. */
//#ifndef RISCV_MATH_BIG_ENDIAN
//      write_q15x2_ia (&pDst, __PKHBT(__SSAT((in1 << shiftBits), 16),
//                                     __SSAT((in2 << shiftBits), 16), 16));
//#else
//      write_q15x2_ia (&pDst, __PKHBT(__SSAT((in2 << shiftBits), 16),
//                                      __SSAT((in1 << shiftBits), 16), 16));
//#endif /* #ifndef RISCV_MATH_BIG_ENDIAN */
//
//      /* read 2 samples from source */
//      in1 = *pSrc++;
//      in2 = *pSrc++;
//
//      write_q15x2_ia (&pDst, __PKHBT(__SSAT((in1 << shiftBits), 16),
//                                     __SSAT((in2 << shiftBits), 16), 16));
//
//#else
//      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
//      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
//      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
//      *pDst++ = __SSAT(((q31_t) *pSrc++ << shiftBits), 16);
//#endif
//
//      /* Decrement loop counter */
//      blkCnt--;
//    }
//  }
//  else
//  {
//    while (blkCnt > 0U)
//    {
//      /* C = A >> shiftBits */
//
//#if defined (RISCV_MATH_DSP)
//      /* read 2 samples from source */
//      in1 = *pSrc++;
//      in2 = *pSrc++;
//
//      /* Shift the inputs and then store the results in the destination buffer. */
//#ifndef RISCV_MATH_BIG_ENDIAN
//      write_q15x2_ia (&pDst, __PKHBT((in1 >> -shiftBits),
//                                     (in2 >> -shiftBits), 16));
//#else
//      write_q15x2_ia (&pDst, __PKHBT((in2 >> -shiftBits),
//                                     (in1 >> -shiftBits), 16));
//#endif /* #ifndef RISCV_MATH_BIG_ENDIAN */
//
//      /* read 2 samples from source */
//      in1 = *pSrc++;
//      in2 = *pSrc++;
//
//#ifndef RISCV_MATH_BIG_ENDIAN
//      write_q15x2_ia (&pDst, __PKHBT((in1 >> -shiftBits),
//                                     (in2 >> -shiftBits), 16));
//#else
//      write_q15x2_ia (&pDst, __PKHBT((in2 >> -shiftBits),
//                                     (in1 >> -shiftBits), 16));
//#endif /* #ifndef RISCV_MATH_BIG_ENDIAN */
//
//#else
//      *pDst++ = (*pSrc++ >> -shiftBits);
//      *pDst++ = (*pSrc++ >> -shiftBits);
//      *pDst++ = (*pSrc++ >> -shiftBits);
//      *pDst++ = (*pSrc++ >> -shiftBits);
//#endif
//
//      /* Decrement loop counter */
//      blkCnt--;
//    }
//  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;

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

}

/**
  @} end of BasicShift group
 */
