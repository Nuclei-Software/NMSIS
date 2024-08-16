/*
 * SPDX-FileCopyrightText: Copyright 2022-2024 Arm Limited and/or its affiliates <open-source-office.com>
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
 * Title:        riscv_nn_vec_mat_mul_result_acc_s8_s16.c
 * Description:  Multiplies a matrix by a vector and accumulate with output.
 *
 * $Date:        19 January 2024
 * $Revision:    V.2.0.0
 *
 * Target :  RISC-V Cores
 *
 * -------------------------------------------------------------------- */
#include "riscv_nnsupportfunctions.h"
/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportLSTM
 * @{
 */

/*
 *  Refer to header file for details.
 */
riscv_nmsis_nn_status riscv_nn_vec_mat_mul_result_acc_s8_s16_ref(const int8_t *lhs,
                                                         const int8_t *rhs,
                                                         const int32_t *effective_bias,
                                                         int16_t *dst,
                                                         const int32_t dst_multiplier,
                                                         const int32_t dst_shift,
                                                         const int32_t rhs_cols,
                                                         const int32_t rhs_rows,
                                                         const int32_t batches,
                                                         const int32_t batch_offset)
{

    for (int batch = 0; batch < batches; batch++)
    {
        const int8_t *rhs_ptr = &rhs[0];
        const int32_t *effective_bias_ptr = &effective_bias[0];

        const int32_t row_loop_cnt = rhs_rows / 3;

        for (int i_row_loop_cnt = 0; i_row_loop_cnt < row_loop_cnt; i_row_loop_cnt++)
        {
            const int8_t *lhs_ptr = lhs;
            const int8_t *rhs_ptr_0 = rhs_ptr;
            rhs_ptr += rhs_cols;
            const int8_t *rhs_ptr_1 = rhs_ptr;
            rhs_ptr += rhs_cols;
            const int8_t *rhs_ptr_2 = rhs_ptr;
            rhs_ptr += rhs_cols;

            int32_t res00 = *effective_bias_ptr++;
            int32_t res01 = *effective_bias_ptr++;
            int32_t res02 = *effective_bias_ptr++;

            for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
            {
                const int32_t rhs_value0 = (int8_t)*rhs_ptr_0;
                const int32_t rhs_value1 = (int8_t)*rhs_ptr_1;
                const int32_t rhs_value2 = (int8_t)*rhs_ptr_2;
                const int32_t lhs_value = (int8_t)*lhs_ptr;

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
            res00 += (int32_t)*dst;
            res00 = CLAMP(res00, NN_Q15_MAX, NN_Q15_MIN);
            *dst++ = (int16_t)res00;

            res01 += (int32_t)*dst;
            res01 = CLAMP(res01, NN_Q15_MAX, NN_Q15_MIN);
            *dst++ = (int16_t)res01;

            res02 += (int32_t)*dst;
            res02 = CLAMP(res02, NN_Q15_MAX, NN_Q15_MIN);
            *dst++ = (int16_t)res02;
        }

        const int loop_cnt = rhs_rows % 3;

        for (int i_loop_cnt = 0; i_loop_cnt < loop_cnt; i_loop_cnt++)
        {
            const int8_t *lhs_ptr = &lhs[0];
            const int8_t *rhs_ptr_0 = &rhs_ptr[0];

            int32_t res00 = *effective_bias_ptr++;

            for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
            {
                int32_t rhs_value0 = (int8_t)rhs_ptr_0[0];
                int32_t lhs_value = (int8_t)lhs_ptr[0];

                res00 += lhs_value * rhs_value0;

                ++rhs_ptr_0;
                ++lhs_ptr;
            }

            // Quantize down
            res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);

            // Accumulate
            res00 += (int32_t)dst[0];

            // Clamp the result
            res00 = CLAMP(res00, NN_Q15_MAX, NN_Q15_MIN);

            *dst++ = (int16_t)res00;
            rhs_ptr += rhs_cols;
        }

        lhs += rhs_cols * batch_offset;
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of supportLSTM group
 */
