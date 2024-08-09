
/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_chebyshev_distance_f16.c
 * Description:  Chebyshev distance between two vectors
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
  @ingroup FloatDist
 */

/**
  @defgroup Chebyshev Chebyshev distance

  Chebyshev distance
 */

/**
  @addtogroup Chebyshev
  @{
 */


/**
 * @brief        Chebyshev distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */

RISCV_DSP_ATTRIBUTE float16_t riscv_chebyshev_distance_f16(const float16_t *pA,const float16_t *pB, uint32_t blockSize)
{
   _Float16 diff=0.0f, maxVal, tmpA, tmpB;

#if defined(RISCV_MATH_VECTOR)
   uint32_t blkCnt = blockSize;                               /* Loop counter */
   float32_t max_temp;
   size_t l;
   vfloat16m8_t v_x, v_y;
   vfloat16m8_t v_at;
   vfloat16m1_t v_temp;
   l = __riscv_vsetvl_e16m1(1);
   v_temp = __riscv_vfsub_vv_f16m1(v_temp, v_temp, l);
   for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
      v_x = __riscv_vle16_v_f16m8(pA, l);
      pA += l;
      v_y = __riscv_vle16_v_f16m8(pB, l);
      pB += l;
      v_at = __riscv_vfsub_vv_f16m8(v_x, v_y, l);
      v_at = __riscv_vfabs_v_f16m8(v_at, l);
      v_temp = __riscv_vfredmax_vs_f16m8_f16m1(v_at, v_temp, l);
   }
   maxVal = __riscv_vfmv_f_s_f16m1_f16(v_temp);
#else
   tmpA = *pA++;
   tmpB = *pB++;
   diff = (_Float16)fabsf((float32_t)((_Float16)tmpA - (_Float16)tmpB));
   maxVal = diff;
   blockSize--;

   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      diff = (_Float16)fabsf((float32_t)((_Float16)tmpA - (_Float16)tmpB));
      if ((_Float16)diff > (_Float16)maxVal)
      {
        maxVal = diff;
      }
      blockSize --;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
   return(maxVal);
}


/**
 * @} end of Chebyshev group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

