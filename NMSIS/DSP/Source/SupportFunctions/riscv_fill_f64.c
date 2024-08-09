/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fill_f64.c
 * Description:  Fills a constant value into a floating-point vector
 *
 * $Date:        13 September 2021
 * $Revision:    V1.10.0
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
  @brief         Fills a constant value into a floating-point vector.
  @param[in]     value      input value to be filled
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
 */
RISCV_DSP_ATTRIBUTE void riscv_fill_f64(
  float64_t value,
  float64_t * pDst,
  uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64))
  size_t l;
  vfloat64m8_t v_fill;
  l = __riscv_vsetvlmax_e64m8();
  v_fill = __riscv_vfmv_v_f_f64m8(value, l);
  for (; (l = __riscv_vsetvl_e64m8(blkCnt)) > 0; blkCnt -= l) {
    __riscv_vse64_v_f64m8 (pDst, v_fill, l);
    pDst += l;
  }
#else

  while (blkCnt > 0U)
  {
    /* C = value */

    /* Fill value in destination buffer */
    *pDst++ = value;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64)) */
}

/**
  @} end of Fill group
 */
