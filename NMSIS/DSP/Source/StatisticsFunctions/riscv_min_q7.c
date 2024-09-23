/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_min_q7.c
 * Description:  Minimum value of a Q7 vector
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

#include "dsp/statistics_functions.h"

/**
  @ingroup groupStats
 */


/**
  @addtogroup Min
  @{
 */

/**
  @brief         Minimum value of a Q7 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
  @param[out]    pIndex     index of minimum value returned here
 */
RISCV_DSP_ATTRIBUTE void riscv_min_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult,
        uint32_t * pIndex)
{
        q7_t minVal, out;                              /* Temporary variables to store the output value. */
        uint32_t blkCnt, outIndex;                     /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
    int8_t min_temp;
    size_t l;
    const q7_t *inputx = pSrc;
    vint8m8_t v_x;
    vint8m1_t v_tempa;
    unsigned long last_suf = 0, temp_index = 0;
    vbool1_t mask;
    out = pSrc[0];
    outIndex = 0;
    blkCnt = blockSize;
    l = __riscv_vsetvl_e8m1(1);
    v_tempa = __riscv_vmv_s_x_i8m1(pSrc[0], l);
    for (; (l = __riscv_vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
    {
        v_x = __riscv_vle8_v_i8m8(inputx, l);
        inputx += l;
        min_temp = __riscv_vmv_x_s_i8m1_i8(__riscv_vredmin_vs_i8m8_i8m1(v_x, v_tempa, l));
        if (min_temp < out){
          out = min_temp;
          mask = __riscv_vmseq_vx_i8m8_b1(v_x, min_temp, l);
          temp_index = __riscv_vfirst_m_b1(mask, l);
          outIndex = last_suf + temp_index;
        }
        last_suf += l;
    }
#else

#if defined (RISCV_MATH_LOOPUNROLL)
        uint32_t index;                                /* index of minimum value */
#endif

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = *pSrc++;

#if defined (RISCV_MATH_LOOPUNROLL)
  /* Initialise index of maximum value. */
  index = 0U;

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = (blockSize - 1U) >> 2U;

  while (blkCnt > 0U)
  {
    /* Initialize minVal to next consecutive values one by one */
    minVal = *pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
      outIndex = index + 1U;
    }

    minVal = *pSrc++;
    if (out > minVal)
    {
      out = minVal;
      outIndex = index + 2U;
    }

    minVal = *pSrc++;
    if (out > minVal)
    {
      out = minVal;
      outIndex = index + 3U;
    }

    minVal = *pSrc++;
    if (out > minVal)
    {
      out = minVal;
      outIndex = index + 4U;
    }

    index += 4U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = (blockSize - 1U) & 3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = *pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

/**
  @} end of Min group
 */
