/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_clip_f16.c
 * Description:  Floating-point vector addition
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

#include "dsp/basic_math_functions_f16.h"

/**
  @ingroup groupMath
 */


/**
  @addtogroup BasicClip
  @{
 */

/**
  @brief         Elementwise floating-point clipping
  @param[in]     pSrc          points to input values
  @param[out]    pDst          points to output clipped values
  @param[in]     low           lower bound
  @param[in]     high          higher bound
  @param[in]     numSamples    number of samples to clip
 */


#if defined(RISCV_FLOAT16_SUPPORTED)

RISCV_DSP_ATTRIBUTE void riscv_clip_f16(const float16_t * pSrc, 
  float16_t * pDst, 
  float16_t low, 
  float16_t high, 
  uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt = numSamples;
    size_t l;
    vfloat16m8_t v_x;
    for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = __riscv_vle16_v_f16m8(pSrc, l);
        pSrc += l;
        v_x = __riscv_vfmax_vf_f16m8(__riscv_vfmin_vf_f16m8(v_x, high, l), low, l);
        __riscv_vse16_v_f16m8(pDst, v_x, l);
        pDst += l;
    }
#else
    for (uint32_t i = 0; i < numSamples; i++)
    {                                        
        if ((_Float16)pSrc[i] > (_Float16)high)
            pDst[i] = high;
        else if ((_Float16)pSrc[i] < (_Float16)low)
            pDst[i] = low;
        else
            pDst[i] = pSrc[i];
    }
#endif /* defined(RISCV_MATH_VECTOR) */
}
#endif /* defined(RISCV_FLOAT16_SUPPORTED */



/**
  @} end of BasicClip group
 */
