
/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_euclidean_distance_f16.c
 * Description:  Euclidean distance between two vectors
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
  @defgroup Euclidean Euclidean distance

  Euclidean distance
 */


/**
  @addtogroup Euclidean
  @{
 */


/**
 * @brief        Euclidean distance between two vectors
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */
RISCV_DSP_ATTRIBUTE float16_t riscv_euclidean_distance_f16(const float16_t *pA,const float16_t *pB, uint32_t blockSize)
{
   _Float16 accum=0.0f,tmp;
   float16_t result;
#if defined(RISCV_MATH_VECTOR)
   size_t blkCnt = blockSize;                               /* Loop counter */
   size_t l;
   vfloat16m8_t v_x, v_y, v_at;
   vfloat16m8_t v_sum;
   l = __riscv_vsetvlmax_e16m8();
   v_sum = __riscv_vfmv_v_f_f16m8(0.0f, l);
   for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
      v_x = __riscv_vle16_v_f16m8(pA, l);
      pA += l;
      v_y = __riscv_vle16_v_f16m8(pB, l);
      pB += l;
      v_at = __riscv_vfsub_vv_f16m8(v_x, v_y, l);
      v_sum = __riscv_vfmacc_vv_f16m8(v_sum, v_at, v_at, l);
   }
   l = __riscv_vsetvl_e16m8(1);
   vfloat16m1_t v_temp = __riscv_vfmv_v_f_f16m1(0.0f, l);
   l = __riscv_vsetvlmax_e16m8();
   v_temp = __riscv_vfredusum_vs_f16m8_f16m1(v_sum, v_temp, l);
   accum += __riscv_vfmv_f_s_f16m1_f16(v_temp);
#else
   while(blockSize > 0)
   {
      tmp = (_Float16)*pA++ - (_Float16)*pB++;
      accum += RISCV_SQ(tmp);
      blockSize --;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
   riscv_sqrt_f16(accum,&result);
   return(result);
}



/**
 * @} end of Euclidean group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

