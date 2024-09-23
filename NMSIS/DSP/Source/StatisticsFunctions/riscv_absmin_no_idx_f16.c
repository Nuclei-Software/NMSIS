/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmin_no_idx_f16.c
 * Description:  Minimum value of absolute values of a floating-point vector
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

#include "dsp/statistics_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)




/**
  @ingroup groupStats
 */

/**
  @addtogroup AbsMin
  @{
 */

/**
  @brief         Minimum value of absolute values of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
 */

#if defined(RISCV_MATH_LOOPUNROLL) && !defined(RISCV_MATH_VECTOR)
RISCV_DSP_ATTRIBUTE void riscv_absmin_no_idx_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult)
{
        float16_t cur_absmin, out;                     /* Temporary variables to store the output value. */\
        uint32_t blkCnt;                     /* Loop counter */                                   \
                                                                                                            \
                                                                                          \
  /* Load first input value that act as reference value for comparision */                                  \
  out = *pSrc++;                                                                                            \
  out = ((_Float16)out > 0.0f16) ? out : -(_Float16)out;                                                                             \
                                                                                              \
                                                                                                            \
  /* Loop unrolling: Compute 4 outputs at a time */                                                         \
  blkCnt = (blockSize - 1U) >> 2U;                                                                          \
                                                                                                            \
  while (blkCnt > 0U)                                                                                       \
  {                                                                                                         \
    /* Initialize cur_absmin to next consecutive values one by one */                                         \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = ((_Float16)cur_absmin > 0.0f16) ? cur_absmin : -(_Float16)cur_absmin;                                                                 \
    /* compare for the extrema value */                                                                     \
    if ((_Float16)cur_absmin < (_Float16)out)                                                                         \
    {                                                                                                       \
      /* Update the extrema value and it's index */                                                         \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = ((_Float16)cur_absmin > 0.0f16) ? cur_absmin : -(_Float16)cur_absmin;                                                                 \
    if ((_Float16)cur_absmin < (_Float16)out)                                                                         \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = ((_Float16)cur_absmin > 0.0f16) ? cur_absmin : -(_Float16)cur_absmin;                                                                 \
    if ((_Float16)cur_absmin < (_Float16)out)                                                                          \
    {                                                                                                       \
      out = cur_absmin;                                                                                       \
    }                                                                                                       \
                                                                                                            \
    cur_absmin = *pSrc++;                                                                                     \
    cur_absmin = ((_Float16)cur_absmin > 0.0f16) ? cur_absmin : -(_Float16)cur_absmin;                                                                 \
    if ((_Float16)cur_absmin < (_Float16)out)                                                                          \
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
    cur_absmin = ((_Float16)cur_absmin > 0.0f16) ? cur_absmin : -(_Float16)cur_absmin;                                                                 \
    if ((_Float16)cur_absmin < (_Float16)out)                                                                         \
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
void riscv_absmin_no_idx_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult)
{
        float16_t minVal, out;                         /* Temporary variables to store the output value. */
        uint32_t blkCnt;                     /* Loop counter */



  /* Load first input value that act as reference value for comparision */
  out = (_Float16)fabsf((float32_t)*pSrc++);

#if defined(RISCV_MATH_VECTOR)
    blkCnt = blockSize;
    size_t l;
    vfloat16m8_t v_x;
    vfloat16m1_t v_temp;
    const float16_t *pIN = pSrc;
    l = __riscv_vsetvl_e16m1(1);
    v_temp = __riscv_vfmv_v_f_f16m1(out, l);
    for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = __riscv_vle16_v_f16m8(pIN, l);
        pIN += l;
        v_x = __riscv_vfsgnjx_vv_f16m8(v_x, v_x, l);
        v_temp = __riscv_vfredmin_vs_f16m8_f16m1(v_x, v_temp, l);
    }
    out = __riscv_vfmv_f_s_f16m1_f16(v_temp);
#else

  /* Initialize blkCnt with number of samples */
  blkCnt = (blockSize - 1U);

  while (blkCnt > 0U)
  {
    /* Initialize minVal to the next consecutive values one by one */
    minVal = (_Float16)fabsf((float32_t)*pSrc++);

    /* compare for the minimum value */
    if ((_Float16)out > (_Float16)minVal)
    {
      /* Update the minimum value and it's index */
      out = minVal;
    }

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store the minimum value and it's index into destination pointers */
  *pResult = out;
}
#endif /* defined(RISCV_MATH_LOOPUNROLL) */
/**
  @} end of AbsMin group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
