/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmin_no_idx_q7.c
 * Description:  Minimum value of absolute values of a Q7 vector
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
  @addtogroup AbsMin
  @{
 */

/**
  @brief         Minimum value of absolute values of a Q7 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
 */
#if defined(RISCV_MATH_DSP) && !defined(RISCV_MATH_VECTOR)
RISCV_DSP_ATTRIBUTE void riscv_absmin_no_idx_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult)
{
        q7_t cur_absmin, out;                     /* Temporary variables to store the output value. */\
        uint32_t blkCnt;                     /* Loop counter */                                   \
                                                                                                            \
                                                                                           \
  /* Load first input value that act as reference value for comparision */                                  \
  out = *pSrc++;                                                                                            \
  out = (out > 0) ? out : (q7_t)__QSUB8(0, out);                                                                           \
                                                                                                            \
  /* Loop unrolling: Compute 4 outputs at a time */                                                         \
  blkCnt = (blockSize - 1U) >> 2U;                                                                          \
                                                                                                            \
  while (blkCnt > 0U)                                                                                       \
  {                                                                                                         \
    /* Initialize cur_absmin to next consecutive values one by one */                                         \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = (cur_absmin > 0) ? cur_absmin : (q7_t)__QSUB8(0, cur_absmin);                                                                \
    /* compare for the extrema value */                                                                     \
    if (cur_absmin < out)                                                                         \
    {                                                                                                       \
      /* Update the extrema value and it's index */                                                         \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = (cur_absmin > 0) ? cur_absmin : (q7_t)__QSUB8(0, cur_absmin);                                                                \
    if (cur_absmin < out)                                                                         \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = (cur_absmin > 0) ? cur_absmin : (q7_t)__QSUB8(0, cur_absmin);                                                                \
    if (cur_absmin < out)                                                                          \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = (cur_absmin > 0) ? cur_absmin : (q7_t)__QSUB8(0, cur_absmin);                                                                 \
    if (cur_absmin < out)                                                                          \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
                                                                                                            \
    /* Decrement loop counter */                                                                            \
    blkCnt--;                                                                                               \
  }                                                                                                         \
                                                                                                            \
  /* Loop unrolling: Compute remaining outputs */                                                           \
  blkCnt = (blockSize - 1U) & 3U;                                                                           \
                                                                                                            \
                                                                                                            \
  while (blkCnt > 0U)                                                                                       \
  {                                                                                                         \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = (cur_absmin > 0) ? cur_absmin : (q7_t)__QSUB8(0, cur_absmin);                                                                 \
    if (cur_absmin < out)                                                                         \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    /* Decrement loop counter */                                                                            \
    blkCnt--;                                                                                               \
  }                                                                                                         \
                                                                                                            \
  /* Store the extrema value and it's index into destination pointers */                                    \
  *pResult = out;                                                                                           \
}
#else
void riscv_absmin_no_idx_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult)
{
        q7_t minVal, out;                              /* Temporary variables to store the output value. */
        uint32_t blkCnt;                     /* Loop counter */


  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
#if defined(RISCV_MATH_VECTOR)
    blkCnt = blockSize;
    size_t l;
    vint8m8_t v_x, v_zero;
    vint8m1_t v_temp;
    const q7_t *pData = pSrc;
    l = __riscv_vsetvlmax_e8m8();
    v_zero = __riscv_vmv_v_x_i8m8(0, l);
    l = __riscv_vsetvlmax_e8m1();
    v_temp = __riscv_vmv_v_x_i8m1(out, l);

    for (; (l = __riscv_vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = __riscv_vle8_v_i8m8(pData, l);
        pData += l;
        vbool1_t mask = __riscv_vmslt_vx_i8m8_b1(v_x, 0, l);
        v_x = __riscv_vssub_vv_i8m8_m(mask, v_zero, v_x, l);
        v_temp = __riscv_vredmin_vs_i8m8_i8m1(v_x, v_temp, l);
    }
    out = __riscv_vmv_x_s_i8m1_i8(v_temp);
#else
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
    pSrc++;

    /* compare for the minimum value */
    if (out > minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* #if defined(RISCV_MATH_VECTOR) */
  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
}
#endif /* defined(RISCV_MATH_DSP) */
/**
  @} end of AbsMin group
 */
