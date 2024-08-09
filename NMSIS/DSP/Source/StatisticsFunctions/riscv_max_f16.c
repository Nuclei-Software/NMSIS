/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_max_f16.c
 * Description:  Maximum value of a floating-point vector
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

#include "dsp/statistics_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupStats
 */


/**
  @addtogroup Max
  @{
 */

/**
  @brief         Maximum value of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
  @param[out]    pIndex     index of maximum value returned here
 */

RISCV_DSP_ATTRIBUTE void riscv_max_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult,
        uint32_t * pIndex)
{
        float16_t maxVal, out;                         /* Temporary variables to store the output value. */
        uint32_t blkCnt, outIndex;                     /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
    float32_t max_temp;

    size_t l;
    const float16_t *inputx = pSrc;
    vfloat16m8_t v_x;
    vfloat16m1_t v_tempa;
    vbool2_t mask;
    unsigned long last_suf = 0, temp_index = 0;
    out = pSrc[0];
    outIndex = 0;

    blkCnt = blockSize;
    l = __riscv_vsetvl_e16m1(1);
    v_tempa = __riscv_vfmv_s_f_f16m1(out, l);
    for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
    {
        v_x = __riscv_vle16_v_f16m8(inputx, l);
        inputx += l;
        max_temp = __riscv_vfmv_f_s_f16m1_f16(__riscv_vfredmax_vs_f16m8_f16m1(v_x, v_tempa, l));
        if (max_temp > out) {
          out = max_temp;
          mask = __riscv_vmfeq_vf_f16m8_b2(v_x, max_temp, l);
          temp_index = __riscv_vfirst_m_b2(mask, l);
          outIndex = last_suf + temp_index;
        }
        last_suf += l;
    }
#else

#if defined (RISCV_MATH_LOOPUNROLL)
        uint32_t index;                                /* index of maximum value */
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
    /* Initialize maxVal to next consecutive values one by one */
    maxVal = *pSrc++;

    /* compare for the maximum value */
    if ((_Float16)out < (_Float16)maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = index + 1U;
    }

    maxVal = *pSrc++;
    if ((_Float16)out < (_Float16)maxVal)
    {
      out = maxVal;
      outIndex = index + 2U;
    }

    maxVal = *pSrc++;
    if ((_Float16)out < (_Float16)maxVal)
    {
      out = maxVal;
      outIndex = index + 3U;
    }

    maxVal = *pSrc++;
    if ((_Float16)out < (_Float16)maxVal)
    {
      out = maxVal;
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
    /* Initialize maxVal to the next consecutive values one by one */
    maxVal = *pSrc++;

    /* compare for the maximum value */
    if ((_Float16)out < (_Float16)maxVal)
    {
      /* Update the maximum value and it's index */
      out = maxVal;
      outIndex = blockSize - blkCnt;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store the maximum value and it's index into destination pointers */
  *pResult = out;
  *pIndex = outIndex;
}

/**
  @} end of Max group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

