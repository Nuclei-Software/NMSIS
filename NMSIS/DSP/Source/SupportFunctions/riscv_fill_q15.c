/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fill_q15.c
 * Description:  Fills a constant value into a Q15 vector
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

#include "dsp/support_functions.h"

/**
  @ingroup groupSupport
 */

/**
  @addtogroup Fill
  @{
 */

/**
  @brief         Fills a constant value into a Q15 vector.
  @param[in]     value      input value to be filled
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */
void riscv_fill_q15(
  q15_t value,
  q15_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                       /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                    /* Loop counter */
  size_t l;
  vint16m8_t v_fill;
  l = vsetvlmax_e16m8();
  v_fill = vmv_v_x_i16m8(value, l);
  for (; (l = vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    vse16_v_i16m8 (pDst, v_fill, l);
    pDst += l;
  }
#else
#if defined (RISCV_MATH_LOOPUNROLL)
  q31_t packedValue;                             /* value packed to 32 bits */

  /* Packing two 16 bit values to 32 bit value in order to use SIMD */
  //packedValue = __PKHBT(value, value, 16U);
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
  q63_t packedValue64;                       /* value packed to 64 bits */
  packedValue = __PKBB16(value, value);
  packedValue64 = __RV_PKBB32(packedValue,packedValue);
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = value */

    /* fill 4 times 2 samples at a time */
    write_q15x4_ia(&pDst, packedValue64);
    blkCnt--;
  }
#else
  packedValue = __PKHBT(value, value, 16U);
  blkCnt = blockSize >> 2U;
    while (blkCnt > 0U)
  {
    /* C = value */

    /* fill 2 times 2 samples at a time */
    write_q15x2_ia(&pDst, packedValue);
    write_q15x2_ia(&pDst, packedValue);
    blkCnt--;
  }
#endif /* RISCV_MATH_DSP && __RISCV_XLEN == 64 */

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = value */

    /* Fill value in destination buffer */
    *pDst++ = value;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of Fill group
 */
