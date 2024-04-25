/*
 * SPDX-FileCopyrightText: Copyright 2020-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_vec_mat_mult_t_s8_ref
 * Description:  s8 vector by matrix (transposed) multiplication
 *
 * $Date:        5 May 2023
 * $Revision:    V.5.4.1
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"
#include "ref_functions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @defgroup supportFC Fully Connected
 *
 * Support functions for Fully Connected
 *
 */

/**
 * @addtogroup supportFC
 * @{
 */

/*
 * s8 vector(lhs) by matrix (transposed) multiplication
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_vec_mat_mult_t_s8_ref(const int8_t *lhs,
                                        const int8_t *rhs,
                                        const int32_t *kernel_sum,
                                        const int32_t *bias,
                                        int8_t *dst,
                                        const int32_t lhs_offset,
                                        const int32_t dst_offset,
                                        const int32_t dst_multiplier,
                                        const int32_t dst_shift,
                                        const int32_t rhs_cols,
                                        const int32_t rhs_rows,
                                        const int32_t activation_min,
                                        const int32_t activation_max,
                                        const int32_t address_offset)
{
    (void)kernel_sum;

    const int32_t row_loop_cnt = rhs_rows / 3;

    for (int i_row_loop_cnt = 0; i_row_loop_cnt < row_loop_cnt; i_row_loop_cnt++)
    {
        const int8_t *lhs_ptr = lhs;
        const int8_t *rhs_ptr_0 = &rhs[0];
        const int8_t *rhs_ptr_1 = &rhs[rhs_cols];
        const int8_t *rhs_ptr_2 = &rhs[rhs_cols * 2];

        int32_t res00 = 0;
        int32_t res01 = 0;
        int32_t res02 = 0;
        if (bias)
        {
            res00 = *bias++;
            res01 = *bias++;
            res02 = *bias++;
        }
        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
        {
            const int32_t rhs_value0 = (int8_t)*rhs_ptr_0;
            const int32_t rhs_value1 = (int8_t)*rhs_ptr_1;
            const int32_t rhs_value2 = (int8_t)*rhs_ptr_2;
            const int32_t lhs_value = (int8_t)*lhs_ptr + lhs_offset;

            res00 += lhs_value * rhs_value0;
            res01 += lhs_value * rhs_value1;
            res02 += lhs_value * rhs_value2;

            ++rhs_ptr_0;
            ++rhs_ptr_1;
            ++rhs_ptr_2;
            ++lhs_ptr;
        }
        // Quantize down
        res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);
        res01 = riscv_nn_requantize(res01, dst_multiplier, dst_shift);
        res02 = riscv_nn_requantize(res02, dst_multiplier, dst_shift);

        // Add offset
        res00 += dst_offset;
        res01 += dst_offset;
        res02 += dst_offset;

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);
        res01 = MAX(res01, activation_min);
        res01 = MIN(res01, activation_max);
        res02 = MAX(res02, activation_min);
        res02 = MIN(res02, activation_max);

        *dst = (int8_t)res00;
        *(dst + address_offset) = (int8_t)res01;
        *(dst + 2 * address_offset) = (int8_t)res02;
        dst += 3 * address_offset;

        rhs += 3 * rhs_cols;
    }

    const int loop_cnt = rhs_rows % 3;

    for (int i_loop_cnt = 0; i_loop_cnt < loop_cnt; i_loop_cnt++)
    {
        const int8_t *lhs_ptr = &lhs[0];
        const int8_t *rhs_ptr = &rhs[0];

        int32_t res00 = 0;
        if (bias)
        {
            res00 = *bias++;
        }

        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
        {
            int32_t rhs_value0 = (int8_t)rhs_ptr[0];
            int32_t lhs_value = (int8_t)lhs_ptr[0] + lhs_offset;

            res00 += lhs_value * rhs_value0;

            ++rhs_ptr;
            ++lhs_ptr;
        }

        // Quantize down
        res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);

        // Add offset
        res00 += dst_offset;

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);

        *dst = (int8_t)res00;
        dst += address_offset;
        rhs += rhs_cols;
    }
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
