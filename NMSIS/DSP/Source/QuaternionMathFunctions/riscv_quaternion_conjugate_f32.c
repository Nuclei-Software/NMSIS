/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_quaternion_conjugate_f32.c
 * Description:  Floating-point quaternion conjugate
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
  @defgroup QuatConjugate Quaternion Conjugate

  Compute the conjugate of a quaternion.
 */

/**
  @addtogroup QuatConjugate
  @{
 */

/**
  @brief         Floating-point quaternion conjugates.
  @param[in]     pInputQuaternions            points to the input vector of quaternions
  @param[out]    pConjugateQuaternions        points to the output vector of conjugate quaternions
  @param[in]     nbQuaternions                number of quaternions in each vector
 */

RISCV_DSP_ATTRIBUTE void riscv_quaternion_conjugate_f32(const float32_t *pInputQuaternions,
    float32_t *pConjugateQuaternions,
    uint32_t nbQuaternions)
{
#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt = nbQuaternions;                               /* Loop counter */
    size_t l;
    const float32_t *pSrc = pInputQuaternions;
    float32_t *pDes = pConjugateQuaternions;
    vfloat32m8_t vx;
    l = __riscv_vsetvlmax_e32m8();
    const uint32_t mask_v[4] = {0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE};
    const uint8_t *mask_v8 = (const uint8_t *)mask_v;
    vbool4_t mask = __riscv_vlm_v_b4(mask_v8, l);
    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
    {
      vx = __riscv_vle32_v_f32m8(pSrc, l);
      pSrc += l;
      __riscv_vse32_v_f32m8(pDes, __riscv_vfmul_vf_f32m8_m(mask, vx, -1, l), l);
      pDes += l;
    }
#else
    for(uint32_t i=0; i < nbQuaternions; i++)
    {

        pConjugateQuaternions[4 * i + 0] = pInputQuaternions[4 * i + 0];
        pConjugateQuaternions[4 * i + 1] = -pInputQuaternions[4 * i + 1];
        pConjugateQuaternions[4 * i + 2] = -pInputQuaternions[4 * i + 2];
        pConjugateQuaternions[4 * i + 3] = -pInputQuaternions[4 * i + 3];
    }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of QuatConjugate group
 */
