/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_nn_softmax_common_s8.c
 * Description:  Softmax with s8 input and output of s8 or s16.
 *
 * $Date:        5 January 2023
 * $Revision:    V.1.1.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

#define ACCUM_BITS 12

/**
 * @ingroup groupSupport
 */

/**
 * @defgroup supportSoftmax Softmax
 *
 * Support functions for Softmax
 *
 */

/**
 * @addtogroup supportSoftmax
 * @{
 */

/*
 * Softmax function with s8 input and output of s8 or s16.
 *
 * Refer header file for details.
 *
 */
void riscv_nn_softmax_common_s8(const int8_t *input,
                              const int32_t num_rows,
                              const int32_t row_size,
                              const int32_t mult,
                              const int32_t shift,
                              const int32_t diff_min,
                              const bool int16_output,
                              void *output)
{
    const int32_t mask = (1 << shift);

    int32_t col = 0;
    int32_t row_idx;

    for (row_idx = 0; row_idx < num_rows; ++row_idx)
    {
        // Find the maximum value in order to ensure numerical stability
        int8_t max = *input;

#if defined(RISCV_MATH_VECTOR)
        size_t l;
        vint8m8_t v_x;
        vint8m1_t v_temp;

        l = __riscv_vsetvl_e8m1(1);
        v_temp = __riscv_vmv_v_x_i8m1(max, l);
        int32_t blkCnt_v = row_size;
        col = 0;
        for (; (l = __riscv_vsetvl_e8m8(blkCnt_v)) > 0; blkCnt_v -= l) {
            v_x = __riscv_vle8_v_i8m8(input + col, l);
            v_temp = __riscv_vredmax_vs_i8m8_i8m1(v_x, v_temp, l);
            col += l;
        }
        max = __riscv_vmv_x_s_i8m1_i8(v_temp);

#else
        for (col = 1; col < row_size; ++col)
        {
            max = MAX(max, input[col]);
        }
#endif /* #if defined(RISCV_MATH_VECTOR) */
        int32_t diff = 0;
        int32_t sum = 0;

        for (col = 0; col < row_size; ++col)
        {
            diff = input[col] - max;
            if (diff >= diff_min)
            {
                sum += DIV_POW2(EXP_ON_NEG(MUL_SAT(diff * mask, mult)), ACCUM_BITS);
            }
        }

        const int32_t headroom = __CLZ(sum);
        const int32_t shifted_scale = ONE_OVER1((sum > 0 ? sum << headroom : 0) - (1 << 31));
        int32_t bits_over_unit;

        if (int16_output)
        {
            int16_t *output_s16 = (int16_t *)output + row_idx * row_size;

            bits_over_unit = ACCUM_BITS - headroom + 15;

            for (col = 0; col < row_size; ++col)
            {
                diff = input[col] - max;

                if (diff >= diff_min)
                {
                    const int32_t res =
                        DIV_POW2(MUL_SAT(shifted_scale, EXP_ON_NEG(MUL_SAT(diff * mask, mult))), bits_over_unit) +
                        NN_Q15_MIN;
                    output_s16[col] = (int16_t)CLAMP(res, (int32_t)NN_Q15_MAX, (int32_t)NN_Q15_MIN);
                }
                else
                {
                    output_s16[col] = NN_Q15_MIN;
                }
            }
        }
        else
        {
            int8_t *output_s8 = (int8_t *)output + row_idx * row_size;

            bits_over_unit = ACCUM_BITS - headroom + 23;

            for (col = 0; col < row_size; ++col)
            {
                diff = input[col] - max;
                if (diff >= diff_min)
                {
                    const int32_t res =
                        DIV_POW2(MUL_SAT(shifted_scale, EXP_ON_NEG(MUL_SAT(diff * mask, mult))), bits_over_unit) +
                        NN_Q7_MIN;
                    output_s8[col] = (int8_t)CLAMP(res, (int32_t)NN_Q7_MAX, (int32_t)NN_Q7_MIN);
                }
                else
                {
                    output_s8[col] = NN_Q7_MIN;
                }
            }
        }

        input += row_size;
    }
}

/**
 * @} end of Softmax group
 */
