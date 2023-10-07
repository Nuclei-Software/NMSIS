/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fill_q7.c
 * Description:  Fills a constant value into a Q7 vector
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
  @brief         Fills a constant value into a Q7 vector.
  @param[in]     value      input value to be filled
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */
void riscv_fill_q7(
  q7_t value,
  q7_t * pDst,
  uint32_t blockSize)
{

#if defined(RISCV_MATH_VECTOR)
  uint32_t blkCnt = blockSize & 0xFFFFFFF0;        /* Loop counter */
  size_t l;
  l = __riscv_vsetvlmax_e8m8();
  vint8m8_t value_v = __riscv_vmv_v_x_i8m8(value, l);
  for (; (l = __riscv_vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
  {
    __riscv_vse8_v_i8m8 (pDst, value_v, l);
    pDst += l;
  }
  blkCnt = blockSize & 0xF;
#else

  uint32_t blkCnt = blockSize;          /* Loop counter */

#if defined (RISCV_MATH_LOOPUNROLL)
  q31_t packedValue;                             /* value packed to 32 bits */
#if defined (RISCV_MATH_DSP)
#if (__RISCV_XLEN == 64)
  q63_t packedValue64;                  /* value packed to 64 bits */
  blkCnt = blockSize >> 3U;
  packedValue = __PACKq7(value, value, value, value);
  packedValue64 = __RV_PKBB32(packedValue, packedValue);

  while (blkCnt > 0U)
  {
    /* fill 8 samples at a time */
    write_q7x8_ia(&pDst, packedValue64);
    blkCnt--;
  }
  blkCnt = blockSize & 0x7U;
#else

  packedValue = __RV_EXPD80((q31_t)value);
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = value */

    /* fill 4 samples at a time */
    write_q7x4_ia(&pDst, packedValue);
    blkCnt--;
  }
  blkCnt = blockSize & 0x3U;
#endif /* __RISCV_XLEN == 64 */
#else
  blkCnt = blockSize >> 2U;
  packedValue = __PACKq7(value, value, value, value);
  while (blkCnt > 0U)
  {
    /* fill 4 samples at a time */
    write_q7x4_ia(&pDst, packedValue);
    blkCnt--;
  }
  blkCnt = blockSize & 0x3U;
#endif /* defined (RISCV_MATH_DSP) */

#else

  blkCnt = blockSize;
#endif /* RISCV_MATH_LOOPUNROLL */
#endif /* defined(RISCV_MATH_VECTOR) */

  while (blkCnt > 0U)
  {
    /* C = value */

    /* Fill value in destination buffer */
    *pDst++ = value;

    /* Decrement loop counter */
    blkCnt--;
  }
}

/**
  @} end of Fill group
 */
