/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_max_no_idx_f32.c
 * Description:  Maximum value of a floating-point vector without returning the index
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
  @addtogroup Max
  @{
 */

/**
  @brief         Maximum value of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
 */


RISCV_DSP_ATTRIBUTE void riscv_max_no_idx_f32(
    const float32_t *pSrc,
    uint32_t   blockSize,
    float32_t *pResult)
{
   float32_t   maxValue = F32_MIN;
   float32_t   newVal;

#if defined(RISCV_MATH_VECTOR)
  uint32_t blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vfloat32m8_t v_in;
  l = __riscv_vsetvl_e32m1(1);
  vfloat32m1_t v_max = __riscv_vfmv_s_f_f32m1(maxValue, l);
  for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
    v_in = __riscv_vle32_v_f32m8(pSrc, l);
    pSrc += l;
    v_max = __riscv_vfredmax_vs_f32m8_f32m1(v_in, v_max, l);
  }
  maxValue = __riscv_vfmv_f_s_f32m1_f32(v_max);
#else

   while (blockSize > 0U)
   {
       newVal = *pSrc++;

       /* compare for the maximum value */
       if (maxValue < newVal)
       {
           /* Update the maximum value and it's index */
           maxValue = newVal;
       }

       blockSize --;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
   *pResult = maxValue;
}


/**
  @} end of Max group
 */
