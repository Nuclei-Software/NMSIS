
/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_braycurtis_distance_f16.c
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

#include "dsp/distance_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

#include <limits.h>
#include <math.h>

/**
 * @ingroup groupDistance
 */

/**
 * @defgroup FloatDist Float Distances
 *
 * Distances between two vectors of float values.
 */

/**
  @ingroup FloatDist
 */

/**
  @defgroup braycurtis Bray-Curtis distance

  Bray-Curtis distance between two vectors
 */

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

float16_t riscv_braycurtis_distance_f16(const float16_t *pA,const float16_t *pB, uint32_t blockSize)
{
   _Float16 accumDiff=0.0f16, accumSum=0.0f16, tmpA, tmpB;

#if defined(RISCV_MATH_VECTOR)
   uint32_t blkCnt = blockSize;                               /* Loop counter */
   size_t l;
   vfloat16m8_t v_x, v_y;
   vfloat16m8_t v_at, v_bt;
   vfloat16m1_t v_sumdiff, v_sum;
   l = __riscv_vsetvl_e16m1(1);
   v_sumdiff = __riscv_vfmv_v_f_f16m1(0.0, l);
   v_sum = __riscv_vmv_v_v_f16m1(v_sumdiff, l);
   for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
      v_x = __riscv_vle16_v_f16m8(pA, l);
      pA += l;
      v_y = __riscv_vle16_v_f16m8(pB, l);
      pB += l;
      v_at = __riscv_vfsub_vv_f16m8(v_x, v_y, l);
      v_sumdiff = __riscv_vfredusum_vs_f16m8_f16m1(__riscv_vfabs_v_f16m8(v_at, l), v_sumdiff, l);
      v_bt = __riscv_vfadd_vv_f16m8(v_x, v_y, l);
      v_sum = __riscv_vfredusum_vs_f16m8_f16m1(__riscv_vfabs_v_f16m8(v_bt, l), v_sum, l);
   }
   accumDiff += __riscv_vfmv_f_s_f16m1_f16(v_sumdiff);
   accumSum += __riscv_vfmv_f_s_f16m1_f16(v_sum);
#else
   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      accumDiff += (_Float16)fabsf((float32_t)((_Float16)tmpA - (_Float16)tmpB));
      accumSum += (_Float16)fabsf((float32_t)((_Float16)tmpA + (_Float16)tmpB));
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



#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

