/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_add_f64.c
 * Description:  Floating-point vector addition
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicAdd
  @{
 */

/**
  @brief         Floating-point vector addition.
  @param[in]     pSrcA      points to first input vector
  @param[in]     pSrcB      points to second input vector
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void riscv_add_f64(
  const float64_t * pSrcA,
  const float64_t * pSrcB,
        float64_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64))
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vfloat64m8_t vx, vy;

  for (; (l = vsetvl_e64m8(blkCnt)) > 0; blkCnt -= l) {
    vx = vle64_v_f64m8(pSrcA, l);
    vy = vle64_v_f64m8(pSrcB, l);
    pSrcA += l;
    pSrcB += l;
    vse64_v_f64m8(pDst, vfadd_vv_f64m8(vy, vx, l), l);
    pDst += l;
  }

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

  while (blkCnt > 0U)
  {
    /* C = A + B */

    /* Add and store result in destination buffer. */
    *pDst++ = (*pSrcA++) + (*pSrcB++);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /*defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) && (defined (__riscv_flen) && (__riscv_flen == 64)) */

}

/**
  @} end of BasicAdd group
 */
