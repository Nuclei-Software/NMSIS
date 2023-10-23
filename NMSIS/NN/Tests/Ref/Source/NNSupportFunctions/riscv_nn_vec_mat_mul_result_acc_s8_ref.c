/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office.com>
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
 * Title:        riscv_nn_vec_mat_mul_result_acc_s8.c
 * Description:  Multiplies a matrix by a vector and accumulate with output.
 *
 * $Date:        20 January 2023
 * $Revision:    V.1.2.0
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
void riscv_nn_vec_mat_mul_result_acc_s8_ref(const int8_t *lhs_in,
                                      const int8_t *rhs_in,
                                      const int32_t *bias,
                                      int16_t *dst,
                                      const int32_t dst_offset,
                                      const int32_t dst_multiplier,
                                      const int32_t dst_shift,
                                      const int32_t rhs_cols,
                                      const int32_t rhs_rows,
                                      const int32_t batch)
{
    for (int i_batch = 0; i_batch < batch; ++i_batch)
    {
        const int8_t *rhs = rhs_in;
        const int8_t *lhs = lhs_in + i_batch * rhs_cols;

        const int32_t row_loop_cnt = rhs_rows / 3;

        for (int i_row_loop_cnt = 0; i_row_loop_cnt < row_loop_cnt; i_row_loop_cnt++)
        {
            const int8_t *lhs_ptr = lhs;
            const int8_t *rhs_ptr_0 = &rhs[0];
            const int8_t *rhs_ptr_1 = &rhs[rhs_cols];
            const int8_t *rhs_ptr_2 = &rhs[rhs_cols * 2];

            int32_t res00 = *bias++;
            int32_t res01 = *bias++;
            int32_t res02 = *bias++;

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
            res00 += dst_offset + *dst;
            res01 += dst_offset + dst[1];
            res02 += dst_offset + dst[2];

            // Clamp the result
            res00 = CLAMP(res00, NN_Q15_MAX, NN_Q15_MIN);
            res01 = CLAMP(res01, NN_Q15_MAX, NN_Q15_MIN);
            res02 = CLAMP(res02, NN_Q15_MAX, NN_Q15_MIN);

            dst[0] = (int16_t)res00;
            dst[1] = (int16_t)res01;
            dst[2] = (int16_t)res02;
            dst += 3;

            rhs += 3 * rhs_cols;
        }

        const int loop_cnt = rhs_rows % 3;

        for (int i_loop_cnt = 0; i_loop_cnt < loop_cnt; i_loop_cnt++)
        {
            const int8_t *lhs_ptr = &lhs[0];
            const int8_t *rhs_ptr = &rhs[0];

            int32_t res00 = *bias++;

            for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
            {
                int32_t rhs_value0 = (int8_t)rhs_ptr[0];
                int32_t lhs_value = (int8_t)lhs_ptr[0];

                res00 += lhs_value * rhs_value0;

                ++rhs_ptr;
                ++lhs_ptr;
            }

            // Quantize down
            res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);

            // Add offset
            res00 += dst_offset + *dst;

            // Clamp the result
            res00 = CLAMP(res00, NN_Q15_MAX, NN_Q15_MIN);

            *dst++ = (int16_t)res00;
            rhs += rhs_cols;
        }
    }
}

/**
 * @} end of supportLSTM group
 */
