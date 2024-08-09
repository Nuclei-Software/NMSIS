/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_min_no_idx_f16.c
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

#include "dsp/statistics_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


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
 */


RISCV_DSP_ATTRIBUTE void riscv_min_no_idx_f16(
    const float16_t *pSrc,
    uint32_t   blockSize,
    float16_t *pResult)
{
   float16_t minValue = F16_MAX;
   float16_t newVal;

#if defined(RISCV_MATH_VECTOR)
    int32_t blkCnt = blockSize; /* Loop counter */
    size_t l;
    vfloat16m8_t v_in;
    l = __riscv_vsetvl_e16m1(1);
    vfloat16m1_t v_min = __riscv_vfmv_s_f_f16m1(minValue, l);
    for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
        v_in = __riscv_vle16_v_f16m8(pSrc, l);
        pSrc += l;
        v_min = __riscv_vfredmin_vs_f16m8_f16m1(v_in, v_min, l);
    }
    minValue = __riscv_vfmv_f_s_f16m1_f16(v_min);
#else

   while (blockSize > 0U)
   {
       newVal = *pSrc++;

       /* compare for the minimum value */
       if ((_Float16)minValue > (_Float16)newVal)
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

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
