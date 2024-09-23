/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_quaternion_normalize_f32.c
 * Description:  Floating-point quaternion normalization
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

#include "dsp/quaternion_math_functions.h"
#include <math.h>

/**
  @ingroup groupQuaternionMath
 */

/**
  @defgroup QuatNormalized Quaternion normalization

  Compute a normalized quaternion.
 */

/**
  @addtogroup QuatNormalized
  @{
 */

/**
  @brief         Floating-point normalization of quaternions.
  @param[in]     pInputQuaternions            points to the input vector of quaternions
  @param[out]    pNormalizedQuaternions       points to the output vector of normalized quaternions
  @param[in]     nbQuaternions                number of quaternions in each vector
 */

RISCV_DSP_ATTRIBUTE void riscv_quaternion_normalize_f32(const float32_t *pInputQuaternions,
    float32_t *pNormalizedQuaternions,
    uint32_t nbQuaternions)
{
   float32_t temp;
#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt = nbQuaternions;                               /* Loop counter */
    size_t l;
    vfloat32m8_t v_x0, v_x1, v_x2, v_x3;
    vfloat32m8_t v_temp;
    const float32_t *pIN = pInputQuaternions;
    float32_t *pOUT = pNormalizedQuaternions;
    ptrdiff_t bstride = 16;
    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        v_x0 = __riscv_vlse32_v_f32m8(pIN, bstride, l);
        v_x1 = __riscv_vlse32_v_f32m8(pIN + 1, bstride, l);
        v_x2 = __riscv_vlse32_v_f32m8(pIN + 2, bstride, l);
        v_x3 = __riscv_vlse32_v_f32m8(pIN + 3, bstride, l);

        v_temp = __riscv_vfmul_vv_f32m8(v_x0, v_x0, l);
        v_temp = __riscv_vfmacc_vv_f32m8(v_temp, v_x1, v_x1, l);
        v_temp = __riscv_vfmacc_vv_f32m8(v_temp, v_x2, v_x2, l);
        v_temp = __riscv_vfmacc_vv_f32m8(v_temp, v_x3, v_x3, l);
        v_temp = __riscv_vfsqrt_v_f32m8(v_temp, l);

        __riscv_vsse32_v_f32m8(pOUT, bstride, __riscv_vfdiv_vv_f32m8(v_x0, v_temp, l), l);
        __riscv_vsse32_v_f32m8(pOUT + 1, bstride, __riscv_vfdiv_vv_f32m8(v_x1, v_temp, l), l);
        __riscv_vsse32_v_f32m8(pOUT + 2, bstride, __riscv_vfdiv_vv_f32m8(v_x2, v_temp, l), l);
        __riscv_vsse32_v_f32m8(pOUT + 3, bstride, __riscv_vfdiv_vv_f32m8(v_x3, v_temp, l), l);

        pIN += l * 4;
        pOUT += l * 4;
    }
#else
   for(uint32_t i=0; i < nbQuaternions; i++)
   {
      temp = RISCV_SQ(pInputQuaternions[4 * i + 0]) +
             RISCV_SQ(pInputQuaternions[4 * i + 1]) +
             RISCV_SQ(pInputQuaternions[4 * i + 2]) +
             RISCV_SQ(pInputQuaternions[4 * i + 3]);
      temp = sqrtf(temp);

      pNormalizedQuaternions[4 * i + 0] = pInputQuaternions[4 * i + 0] / temp;
      pNormalizedQuaternions[4 * i + 1] = pInputQuaternions[4 * i + 1] / temp;
      pNormalizedQuaternions[4 * i + 2] = pInputQuaternions[4 * i + 2] / temp;
      pNormalizedQuaternions[4 * i + 3] = pInputQuaternions[4 * i + 3] / temp;
   }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of QuatNormalized group
 */
