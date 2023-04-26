/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_min_no_idx_f32.c
 * Description:  Minimum value of a floating-point vector without returning the index
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
  @addtogroup Min
  @{
 */

/**
  @brief         Minimum value of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
  @return        none
 */


void riscv_min_no_idx_f32(
    const float32_t *pSrc,
    uint32_t blockSize,
    float32_t *pResult)
{
   float32_t minValue = F32_MAX;
   float32_t newVal;

#if defined(RISCV_MATH_VECTOR)
    int32_t blkCnt = blockSize; /* Loop counter */
    size_t l;
    vfloat32m8_t v_in;
    l = vsetvl_e32m1(1);
    vfloat32m1_t v_min = vfmv_s_f_f32m1(v_min, minValue, l);
    for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        v_in = vle32_v_f32m8(pSrc, l);
        pSrc += l;
        v_min = vfredmin_vs_f32m8_f32m1(v_min, v_in, v_min, l);
    }
    minValue = vfmv_f_s_f32m1_f32(v_min);
#else

   while (blockSize > 0U)
   {
       newVal = *pSrc++;

       /* compare for the minimum value */
       if (minValue > newVal)
       {
           /* Update the minimum value and it's index */
           minValue = newVal;
       }

       blockSize --;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
   *pResult = minValue;
}

/**
  @} end of Min group
 */
