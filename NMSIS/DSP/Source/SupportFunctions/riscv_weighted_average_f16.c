/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_weighted_average_f16.c
 * Description:  Weighted average
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

#include <limits.h>
#include <math.h>

#include "dsp/support_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

/**
  @ingroup groupSupport
 */

/**
  @defgroup weightedaverage Weighted Average

  Weighted average of values
 */


/**
 * @addtogroup weightedaverage
 * @{
 */


/**
 * @brief Weighted average
 *
 *
 * @param[in]    *in           Array of input values.
 * @param[in]    *weigths      Weights
 * @param[in]    blockSize     Number of samples in the input array.
 *
 * @return       Weighted average
 *
 */

RISCV_DSP_ATTRIBUTE float16_t riscv_weighted_average_f16(const float16_t *in, const float16_t *weigths, uint32_t blockSize)
{

    _Float16 accum1, accum2;
    const float16_t *pIn, *pW;
    uint32_t blkCnt;


    pIn = in;
    pW = weigths;

    accum1=0.0f16;
    accum2=0.0f16;

#if defined(RISCV_MATH_VECTOR)
    size_t blkCnt_v = blockSize;
    size_t l;
    vfloat16m8_t v_x, v_y;
    vfloat16m8_t v_a, v_b;

    l = __riscv_vsetvlmax_e16m8();
    v_a = __riscv_vfmv_v_f_f16m8(0.0f, l);
    v_b = __riscv_vmv_v_v_f16m8(v_a, l);
    for (; (l = __riscv_vsetvl_e16m8(blkCnt_v)) > 0; blkCnt_v -= l) {
        v_x = __riscv_vle16_v_f16m8(pIn, l);
        pIn += l;
        v_y = __riscv_vle16_v_f16m8(pW, l);
        pW += l;
        v_a = __riscv_vfmacc_vv_f16m8(v_a, v_x, v_y, l);
        v_b = __riscv_vfadd_vv_f16m8(v_y, v_b, l);
    }
    l = __riscv_vsetvl_e16m8(1);
    vfloat16m1_t temp00m1 = __riscv_vfmv_v_f_f16m1(0.0f, l);
    vfloat16m1_t temp01m1 = __riscv_vmv_v_v_f16m1(temp00m1, l);
    l = __riscv_vsetvlmax_e16m8();
    temp00m1 = __riscv_vfredusum_vs_f16m8_f16m1(v_a, temp00m1, l);
    temp01m1 = __riscv_vfredusum_vs_f16m8_f16m1(v_b, temp01m1, l);
    accum1 += __riscv_vfmv_f_s_f16m1_f16(temp00m1);
    accum2 += __riscv_vfmv_f_s_f16m1_f16(temp01m1);
#else
    blkCnt = blockSize;
    while(blkCnt > 0)
    {
        accum1 += (_Float16)*pIn++ * (_Float16)*pW;
        accum2 += (_Float16)*pW++;
        blkCnt--;
    }
#endif /* #if defined(RISCV_MATH_VECTOR) */
    return(accum1 / accum2);
}
/**
 * @} end of weightedaverage group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */

