
/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_braycurtis_distance_f32.c
 * Description:  Bray-Curtis distance between two vectors
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

#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>



/**
  @addtogroup braycurtis
  @{
 */


/**
 * @brief        Bray-Curtis distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */
RISCV_DSP_ATTRIBUTE float32_t riscv_braycurtis_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize)
{
   float32_t accumDiff=0.0f, accumSum=0.0f, tmpA, tmpB;

#if defined(RISCV_MATH_VECTOR)
   uint32_t blkCnt = blockSize;                               /* Loop counter */
   size_t l;
   vfloat32m8_t v_x, v_y;
   vfloat32m8_t v_at, v_bt;
   vfloat32m1_t v_sumdiff, v_sum;
   l = __riscv_vsetvl_e32m1(1);
   v_sumdiff = __riscv_vfmv_v_f_f32m1(0.0, l);
   v_sum = __riscv_vmv_v_v_f32m1(v_sumdiff, l);
   for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
      v_x = __riscv_vle32_v_f32m8(pA, l);
      pA += l;
      v_y = __riscv_vle32_v_f32m8(pB, l);
      pB += l;
      v_at = __riscv_vfsub_vv_f32m8(v_x, v_y, l);
      v_sumdiff = __riscv_vfredusum_vs_f32m8_f32m1(__riscv_vfabs_v_f32m8(v_at, l), v_sumdiff, l);
      v_bt = __riscv_vfadd_vv_f32m8(v_x, v_y, l);
      v_sum = __riscv_vfredusum_vs_f32m8_f32m1(__riscv_vfabs_v_f32m8(v_bt, l), v_sum, l);
   }
   accumDiff += __riscv_vfmv_f_s_f32m1_f32(v_sumdiff);
   accumSum += __riscv_vfmv_f_s_f32m1_f32(v_sum);
#else
   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      accumDiff += fabsf(tmpA - tmpB);
      accumSum += fabsf(tmpA + tmpB);
      blockSize --;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
   /*

   It is assumed that accumSum is not zero. Since it is the sum of several absolute
   values it would imply that all of them are zero. It is very unlikely for long vectors.

   */
   return(accumDiff / accumSum);
}


/**
 * @} end of braycurtis group
 */

