/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_max_no_idx_q15.c
 * Description:  Maximum value of a q15 vector without returning the index
 *
 * $Date:        16 November 2021
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

#include "dsp/statistics_functions.h"


/**
  @ingroup groupStats
 */

/**
  @addtogroup Max
  @{
 */

/**
  @brief         Maximum value of a q15 vector without index.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
 */

RISCV_DSP_ATTRIBUTE void riscv_max_no_idx_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult)
{
  q15_t maxVal1, out;       /* Temporary variables to store the output value. */
  unsigned long blkCnt;     /* loop counter */

#if defined(RISCV_MATH_VECTOR)
  size_t l;
  const q15_t *inputx = pSrc;
  vint16m8_t v_x;
  vint16m1_t v_tempa;
  blkCnt = blockSize;
  l = __riscv_vsetvl_e16m1(1);
  v_tempa = __riscv_vmv_s_x_i16m1(pSrc[0], l);
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_x = __riscv_vle16_v_i16m8(inputx, l);
    inputx += l;
    v_tempa = __riscv_vredmax_vs_i16m8_i16m1(v_x, v_tempa, l);
  }
  out = __riscv_vmv_x_s_i16m1_i16(v_tempa);
#else

  /* Load first input value that act as reference value for comparision */
  out = *pSrc;
  unsigned long i;

#ifdef RISCV_MATH_DSP
#if __RISCV_XLEN == 64
  q63_t in;
  q63_t max = read_q15x4((q15_t*)pSrc);
  q15_t max_q15[4];

  blkCnt = blockSize >> 2U;
#else
  q31_t in;
  q31_t max = read_q15x2((q15_t*)pSrc);
  q15_t max_q15[2];

  blkCnt = blockSize >> 1U;
#endif /* __RISCV_XLEN == 64 */

  while (blkCnt > 0U)
  {
#if __RISCV_XLEN == 64
    in = read_q15x4_ia((q15_t**)&pSrc);
#else
    in = read_q15x2_ia((q15_t**)&pSrc);
#endif /* __RISCV_XLEN == 64 */
    max = __RV_SMAX16(max, in);
    blkCnt--;
  }

  max_q15[0] = (q15_t)max;
  max_q15[1] = (q15_t)(max >> 16);
#if __RISCV_XLEN == 64
  max_q15[2] = (q15_t)(max >> 32);
  max_q15[3] = (q15_t)(max >> 48);
#endif /* __RISCV_XLEN == 64 */

  maxVal1 = max_q15[0];

#if __RISCV_XLEN == 64
  for (i = 1; i <= 3; i++)
  {
    if (maxVal1 < max_q15[i])
    {
      /* Update the maximum value0 */
      maxVal1 = max_q15[i];
    }
  }
#else
  if(maxVal1 < max_q15[1])
  {
    maxVal1 = max_q15[1];
  }
#endif /* __RISCV_XLEN == 64 */
  out = maxVal1;
#endif /* #ifdef RISCV_MATH_DSP */

#ifdef RISCV_MATH_DSP
#if __RISCV_XLEN == 64
  blkCnt = blockSize & 3U;
#else
  blkCnt = blockSize & 1U;
#endif /* __RISCV_XLEN == 64 */
#else
  blkCnt = blockSize;
#endif /* #ifdef RISCV_MATH_DSP */

  while (blkCnt > 0U)
  {
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal1 = *pSrc++;

    /* compare for the maximum value */
    if (out < maxVal1)
    {
      /* Update the maximum value */
      out = maxVal1;
    }
    /* Decrement the loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store the maximum value into destination pointer */
  *pResult = out;
}

/**
  @} end of Max group
 */
