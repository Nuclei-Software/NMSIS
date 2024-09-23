/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_barycenter_f32.c
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

#include "dsp/support_functions.h"
#include <limits.h>
#include <math.h>


/**
  @ingroup barycenter
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

RISCV_DSP_ATTRIBUTE void riscv_barycenter_f32(const float32_t *in, const float32_t *weights, float32_t *out, uint32_t nbVectors,uint32_t vecDim)
{

   const float32_t *pIn,*pW;
   float32_t *pOut;
   uint32_t blkCntVector,blkCntSample;
   float32_t accum, w;

   blkCntVector = nbVectors;
   blkCntSample = vecDim;

   accum = 0.0f;

   pW = weights;
   pIn = in;

   /* Set counters to 0 */
   blkCntSample = vecDim;
   pOut = out;

   while(blkCntSample > 0)
   {
         *pOut = 0.0f;
         pOut++;
         blkCntSample--;
   }

   /* Sum */
   while(blkCntVector > 0)
   {
      pOut = out;
      w = *pW++;
      accum += w;
#if defined(RISCV_MATH_VECTOR)
      uint32_t blkCnt_v = vecDim;                               /* Loop counter */
      size_t l;
      vfloat32m8_t v_x, v_y;

      for (; (l = __riscv_vsetvl_e32m8(blkCnt_v)) > 0; blkCnt_v -= l) {
        v_x = __riscv_vle32_v_f32m8(pIn, l);
        pIn += l;
        v_y = __riscv_vle32_v_f32m8(pOut, l);
        v_y = __riscv_vfmacc_vf_f32m8(v_y, w, v_x, l);
        __riscv_vse32_v_f32m8(pOut, v_y, l);
        pOut += l;
      }
#else
      blkCntSample = vecDim;
      while(blkCntSample > 0)
      {
          *pOut = *pOut + *pIn++ * w;
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
         *pOut = *pOut / accum;
         pOut++;
         blkCntSample--;
   }

}

/**
 * @} end of barycenter group
 */
