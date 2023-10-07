/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_weighted_sum_f32.c
 * Description:  Weighted Sum
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

#include "dsp/support_functions.h"

/**
 * @addtogroup weightedsum
 * @{
 */


/**
 * @brief Weighted sum
 *
 *
 * @param[in]    *in           Array of input values.
 * @param[in]    *weigths      Weights
 * @param[in]    blockSize     Number of samples in the input array.
 * @return       Weighted sum
 *
 */

float32_t riscv_weighted_sum_f32(const float32_t *in, const float32_t *weigths, uint32_t blockSize)
{

    float32_t accum1, accum2;
    const float32_t *pIn, *pW;
    uint32_t blkCnt;


    pIn = in;
    pW = weigths;

    accum1=0.0f;
    accum2=0.0f;

#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt_v = blockSize;
    size_t l;
    vfloat32m8_t v_x, v_y;
    vfloat32m1_t v_a, v_b;
    l = __riscv_vsetvl_e32m1(1);
    v_a = __riscv_vfsub_vv_f32m1(v_a, v_a, l);
    v_b = __riscv_vfsub_vv_f32m1(v_b, v_b, l);
    for (; (l = __riscv_vsetvl_e32m8(blkCnt_v)) > 0; blkCnt_v -= l) {
        v_x = __riscv_vle32_v_f32m8(pIn, l);
        pIn += l;
        v_y = __riscv_vle32_v_f32m8(pW, l);
        pW += l;
        v_a = __riscv_vfredusum_vs_f32m8_f32m1(__riscv_vfmul_vv_f32m8(v_x, v_y, l), v_a, l);
        v_b = __riscv_vfredusum_vs_f32m8_f32m1(v_y, v_b, l);
    }
    accum1 += __riscv_vfmv_f_s_f32m1_f32(v_a);
    accum2 += __riscv_vfmv_f_s_f32m1_f32(v_b);
#else
    blkCnt = blockSize;
    while(blkCnt > 0)
    {
        accum1 += *pIn++ * *pW;
        accum2 += *pW++;
        blkCnt--;
    }
#endif /* #if defined(RISCV_MATH_VECTOR) */
    return(accum1 / accum2);
}

/**
 * @} end of weightedsum group
 */
