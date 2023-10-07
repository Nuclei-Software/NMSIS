/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_clip_q31.c
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */


/**
  @addtogroup BasicClip
  @{
 */

/**
  @brief         Elementwise fixed-point clipping
  @param[in]     pSrc          points to input values
  @param[out]    pDst          points to output clipped values
  @param[in]     low           lower bound
  @param[in]     high          higher bound
  @param[in]     numSamples    number of samples to clip
  @return        none
 */
void riscv_clip_q31(const q31_t * pSrc,
  q31_t * pDst,
  q31_t low,
  q31_t high,
  uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt = numSamples;
    size_t l;
    vint32m8_t v_x;
    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = __riscv_vle32_v_i32m8(pSrc, l);
        pSrc += l;
        v_x = __riscv_vmax_vx_i32m8(__riscv_vmin_vx_i32m8(v_x, high, l), low, l);
        __riscv_vse32_v_i32m8(pDst, v_x, l);
        pDst += l;
    }
#else
    uint32_t i;
    for (i = 0; i < numSamples; i++)
    {
        if (pSrc[i] > high)
            pDst[i] = high;
        else if (pSrc[i] < low)
            pDst[i] = low;
        else
            pDst[i] = pSrc[i];
    }
#endif /* #if defined (RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicClip group
 */
