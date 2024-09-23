/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_barycenter_f16.c
 * Description:  Barycenter
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

#include "dsp/support_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

#include <limits.h>
#include <math.h>

/**
  @ingroup groupSupport
 */

/**
  @defgroup barycenter Barycenter

  Barycenter of weighted vectors
 */

/**
  @addtogroup barycenter
  @{
 */


/**
 * @brief Barycenter
 *
 *
 * @param[in]    *in         List of vectors
 * @param[in]    *weights    Weights of the vectors
 * @param[out]   *out        Barycenter
 * @param[in]    nbVectors   Number of vectors
 * @param[in]    vecDim      Dimension of space (vector dimension)
 *
 */

RISCV_DSP_ATTRIBUTE void riscv_barycenter_f16(const float16_t *in, const float16_t *weights, float16_t *out, uint32_t nbVectors,uint32_t vecDim)
{

   const float16_t *pIn,*pW;
   float16_t *pOut;
   uint32_t blkCntVector,blkCntSample;
   float16_t accum, w;

   blkCntVector = nbVectors;
   blkCntSample = vecDim;

   accum = 0.0f16;

   pW = weights;
   pIn = in;

   /* Set counters to 0 */
   blkCntSample = vecDim;
   pOut = out;

   while(blkCntSample > 0)
   {
         *pOut = 0.0f16;
         pOut++;
         blkCntSample--;
   }

   /* Sum */
   while(blkCntVector > 0)
   {
      pOut = out;
      w = *pW++;
      accum += (_Float16)w;
#if defined(RISCV_MATH_VECTOR)
      uint32_t blkCnt_v = vecDim;                               /* Loop counter */
      size_t l;
      vfloat16m8_t v_x, v_y;

      for (; (l = __riscv_vsetvl_e16m8(blkCnt_v)) > 0; blkCnt_v -= l) {
        v_x = __riscv_vle16_v_f16m8(pIn, l);
        pIn += l;
        v_y = __riscv_vle16_v_f16m8(pOut, l);
        v_y = __riscv_vfmacc_vf_f16m8(v_y, w, v_x, l);
        __riscv_vse16_v_f16m8(pOut, v_y, l);
        pOut += l;
      }
#else

      blkCntSample = vecDim;
      while(blkCntSample > 0)
      {
          *pOut = (_Float16)*pOut + (_Float16)*pIn++ * (_Float16)w;
          pOut++;
          blkCntSample--;
      }
#endif /* defined(RISCV_MATH_VECTOR) */
      blkCntVector--;
   }

   /* Normalize */
   blkCntSample = vecDim;
   pOut = out;

   while(blkCntSample > 0)
   {
         *pOut = (_Float16)*pOut / (_Float16)accum;
         pOut++;
         blkCntSample--;
   }

}

/**
 * @} end of barycenter group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

