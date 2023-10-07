/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmin_q15.c
 * Description:  Minimum value of absolute values of a Q15 vector
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
  @addtogroup AbsMin
  @{
 */

/**
  @brief         Minimum value of absolute values of a Q15 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
  @param[out]    pIndex     index of minimum value returned here
  @return        none
 */
#if defined(RISCV_MATH_DSP) && !defined(RISCV_MATH_VECTOR)
void riscv_absmin_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult,
        uint32_t * pIndex)
{
        q15_t cur_absmin, out;                            /* Temporary variables to store the output value. */
        q63_t cur;
        unsigned long blkCnt, outIndex;            /* Loop counter */
        int index;                                 /* index of maximum value */

  /* Initialize index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */

  /* Initialize index of extrema value. */
  out = *pSrc;
  out = (out > 0) ? out : (q15_t)__QSUB16(0, out);

  index = -1;
  blkCnt = blockSize >> 2U;
  while (blkCnt > 0U)
  {

    cur = read_q15x4_ia((q15_t**)&pSrc);
#if (__RISCV_XLEN == 64)
    cur = __RV_KABS16(cur);
#else
#if defined (NUCLEI_DSP_N1)
    cur = __dkabs16(cur);
#else
    cur = (q63_t)__RV_KABS16((uint32_t)(cur >> 32)) << 32 | (uint32_t)__RV_KABS16((uint32_t)cur);
#endif /* defined (NUCLEI_DSP_N1) */
#endif /* (__RISCV_XLEN == 64) */

    /* Initialize cur_absmin to next consecutive values one by one */
    cur_absmin = (q15_t)cur;                                                    /* num1 */
    if (cur_absmin < out)
    {
      out = cur_absmin;
      outIndex = index + 1U;
    }

    cur_absmin = (q15_t)(cur >> 16U);                                           /* num2 */
    if (cur_absmin < out)
    {
      out = cur_absmin;
      outIndex = index + 2U;
    }

    cur_absmin = (q15_t)(cur >> 32U);                                           /* num3 */
    if (cur_absmin < out)
    {
      out = cur_absmin;
      outIndex = index + 3U;
    }

    cur_absmin = (q15_t)(cur >> 48U);                                           /* num4 */
    if (cur_absmin < out)
    {
      out = cur_absmin;
      outIndex = index + 4U;
    }
    index += 4U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 3U;
  while (blkCnt > 0U)                                                                                       \
  {                                                                                                         \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = (cur_absmin > 0) ? cur_absmin : (q15_t)__QSUB16(0, cur_absmin);                                                                 \
    if (cur_absmin < out)                                                                         \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
      outIndex = blockSize - blkCnt;                                                                        \
    }                                                                                                       \
                                                                                                            \
    /* Decrement loop counter */                                                                            \
    blkCnt--;                                                                                               \
  }                                                                                                         \
                                                                                                            \
  /* Store the extrema value and it's index into destination pointers */                                    \
  *pResult = out;                                                                                           \
  *pIndex = outIndex;
}
#else
void riscv_absmin_q15(
  const q15_t * pSrc,
        uint32_t blockSize,
        q15_t * pResult,
        uint32_t * pIndex)
{
  q15_t minVal, out;              /* Temporary variables to store the output value. */
  unsigned long blkCnt, outIndex; /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;
  size_t l;
  vint16m8_t v_x, v_zero;
  vint16m1_t v_temp;
  unsigned long last_suf = 0, temp_index = 0;
  const q15_t *pData = pSrc;
  out = 0x7fff;
  outIndex = 0;
  l = __riscv_vsetvlmax_e16m8();
  v_zero = __riscv_vmv_v_x_i16m8(0, l);
  l = __riscv_vsetvlmax_e16m1();
  v_temp = __riscv_vmv_v_x_i16m1(out, l);

  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_x = __riscv_vle16_v_i16m8(pData, l);
    pData += l;
    vbool2_t mask = __riscv_vmslt_vx_i16m8_b2(v_x, 0, l);
    v_x = __riscv_vssub_vv_i16m8_m(mask, v_zero, v_x, l);
    minVal =
        __riscv_vmv_x_s_i16m1_i16(__riscv_vredmin_vs_i16m8_i16m1(v_x, v_temp, l));
    if (minVal < out)
    {
      out = minVal;
      mask = __riscv_vmseq_vx_i16m8_b2(v_x, minVal, l);
      temp_index = __riscv_vfirst_m_b2(mask, l);
      outIndex = last_suf + temp_index;
    }
    last_suf += l;
  }
#else

  /* Initialise index value to zero. */
  outIndex = 0U;
  /* Load first input value that act as reference value for comparision */
  out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
  pSrc++;

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q15_t) 0x8000) ? 0x7fff : -*pSrc);
    pSrc++;

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
#endif /* defined(RISCV_MATH_DSP) */
/**
  @} end of AbsMin group
 */
