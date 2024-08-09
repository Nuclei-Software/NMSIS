/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_max_no_idx_q31.c
 * Description:  Maximum value of a q31 vector without returning the index
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
  @brief         Maximum value of a q31 vector without index.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
 */

RISCV_DSP_ATTRIBUTE void riscv_max_no_idx_q31(
  const q31_t * pSrc,
  uint32_t blockSize,
  q31_t * pResult)
{
  q31_t maxVal1, out;       /* Temporary variables to store the output value. */
  unsigned long blkCnt;     /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  size_t l;
  const q31_t *inputx = pSrc;
  vint32m8_t v_x;
  vint32m1_t v_tempa;
  blkCnt = blockSize;
  l = __riscv_vsetvl_e32m1(1);
  v_tempa = __riscv_vmv_s_x_i32m1(pSrc[0], l);
  for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_x = __riscv_vle32_v_i32m8(inputx, l);
    inputx += l;
    v_tempa = __riscv_vredmax_vs_i32m8_i32m1(v_x, v_tempa, l);
  }
  out = __riscv_vmv_x_s_i32m1_i32(v_tempa);
#else
  /* Load first input value that act as reference value for comparision */
  out = *pSrc;

#if defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
  q63_t in64;
  q63_t max64 = read_q31x2((q31_t*)pSrc);
  q31_t max_q31[2];

  blkCnt = blockSize >> 1U;
  while (blkCnt > 0U)
  {
    in64 = read_q31x2_ia((q31_t**)&pSrc);
    max64 = __RV_SMAX32(max64, in64);
    blkCnt--;
  }

  max_q31[0] = (q31_t)max64;
  max_q31[1] = (q31_t)(max64 >> 32);

  maxVal1 = max_q31[0];

  if(maxVal1 < max_q31[1])
  {
    maxVal1 = max_q31[1];
  }
  out = maxVal1;

#endif /* defined(RISCV_MATH_DSP) && __RISCV_XLEN == 64 */

#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
  blkCnt = blockSize & 1U;
#else
  blkCnt = blockSize;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

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
    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store the maximum value into destination pointer */
  *pResult = out;
}

/**
  @} end of Max group
 */
