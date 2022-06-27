
/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_chebyshev_distance_f64.c
 * Description:  Chebyshev distance between two vectors
 *
 * $Date:        13 September 2021
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

#include "dsp/distance_functions.h"
#include <limits.h>
#include <math.h>


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
float64_t riscv_chebyshev_distance_f64(const float64_t *pA,const float64_t *pB, uint32_t blockSize)
{
   float64_t diff=0., maxVal, tmpA, tmpB;

#if 0
   uint32_t blkCnt = blockSize;                               /* Loop counter */
   float64_t max_temp;
   size_t l;
   vfloat64m8_t v_x, v_y;
   vfloat64m8_t v_at, v_bt;
   vfloat64m1_t v_sum;
   l = vsetvl_e64m1(1);
   v_sum = vfsub_vv_f64m1(v_sum, v_sum, l);
   for (; (l = vsetvl_e64m8(blkCnt)) > 0; blkCnt -= l) {
      v_x = vle64_v_f64m8(pA, l);
      pA += l;
      v_y = vle64_v_f64m8(pB, l);
      pB += l;
      v_at = vfsub_vv_f64m8(v_x, v_y, l);
      v_at = vfsgnjx_vv_f64m8(v_at, v_at, l);
      v_sum = vfredmax_vs_f64m8_f64m1(v_sum, v_at, v_sum, l);
   }
   maxVal = vfmv_f_s_f64m1_f64(v_sum);
#else

   tmpA = *pA++;
   tmpB = *pB++;
   diff = fabs(tmpA - tmpB);
   maxVal = diff;
   blockSize--;

   while(blockSize > 0)
   {
      tmpA = *pA++;
      tmpB = *pB++;
      diff = fabs(tmpA - tmpB);
      if (diff > maxVal)
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
