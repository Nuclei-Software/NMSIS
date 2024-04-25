/*
 * SPDX-FileCopyrightText: Copyright 2023 Arm Limited and/or its affiliates <open-source-office.com>
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
 * Title:        riscv_nn_vec_mat_mult_t_s4_ref
 * Description:  s4 vector by matrix (transposed) multiplication
 *
 * $Date:        10 October 2023
 * $Revision:    V.1.0.0
 *
 * Target :  RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "ref_functions.h"
/**
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
 * s4 vector(lhs) by matrix (transposed) multiplication
 *
 * Refer header file for details.
 *
 */

riscv_nmsis_nn_status riscv_nn_vec_mat_mult_t_s4_ref(const int8_t *lhs,
                                             const int8_t *packed_rhs,
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
    const int8_t *rhs_ptr = &packed_rhs[0];
    int32_t spillover0, spillover1;
    const int rhs_offset = rhs_cols * ((rhs_rows) / 4);

    for (int i_row_loop_cnt = 0; i_row_loop_cnt < rhs_rows / 4; ++i_row_loop_cnt)
    {
        const int8_t *lhs_ptr = &lhs[0];

        int32_t res0 = 0;
        int32_t res1 = 0;

        if (bias)
        {
            res0 += *bias;
            res1 += bias[2 * (rhs_rows / 4)];
            ++bias;
        }

        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols / 2; ++rhs_cols_idx)
        {
            const int32_t rhs_low0 = (int8_t)(rhs_ptr[0] << 4) >> 4;
            const int32_t rhs_high0 = rhs_ptr[0] >> 4;
            const int32_t rhs_low1 = (int8_t)(rhs_ptr[rhs_offset] << 4) >> 4;
            const int32_t rhs_high1 = rhs_ptr[rhs_offset] >> 4;

            const int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
            const int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

            res0 += lhs_low * rhs_low0;
            res0 += lhs_high * rhs_high0;
            res1 += lhs_low * rhs_low1;
            res1 += lhs_high * rhs_high1;

            ++rhs_ptr;
            lhs_ptr += 2;
        }

        if (rhs_cols % 2 == 1)
        {
            const int32_t rhs_low0 = (int8_t)(rhs_ptr[0] << 4) >> 4;
            const int32_t rhs_high0 = rhs_ptr[0] >> 4;
            const int32_t rhs_low1 = (int8_t)(rhs_ptr[rhs_offset] << 4) >> 4;
            const int32_t rhs_high1 = rhs_ptr[rhs_offset] >> 4;

            const int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
            lhs_ptr = &lhs[0];
            const int32_t lhs_high = (int8_t)lhs_ptr[0] + lhs_offset;
            ++lhs_ptr;

            res0 += lhs_low * rhs_low0;
            spillover0 = lhs_high * rhs_high0;
            res1 += lhs_low * rhs_low1;
            spillover1 = lhs_high * rhs_high1;

            ++rhs_ptr;
        }
        else
        {
            spillover0 = 0;
            spillover1 = 0;
            lhs_ptr = &lhs[0];
        }

        // Quantize down
        res0 = riscv_nn_requantize(res0, dst_multiplier, dst_shift);
        res1 = riscv_nn_requantize(res1, dst_multiplier, dst_shift);

        // Add offset
        res0 += dst_offset;
        res1 += dst_offset;

        // Clamp the result
        res0 = MAX(res0, activation_min);
        res0 = MIN(res0, activation_max);
        res1 = MAX(res1, activation_min);
        res1 = MIN(res1, activation_max);

        *dst = (int8_t)res0;

        *(dst + 2 * address_offset * ((rhs_rows) / 4)) = (int8_t)res1;
        dst += address_offset;

        res0 = spillover0;
        res1 = spillover1;
        if (bias)
        {
            res0 += *bias;
            res1 += bias[2 * (rhs_rows / 4)];
            ++bias;
        }

        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols / 2; ++rhs_cols_idx)
        {
            const int32_t rhs_low0 = (int8_t)(rhs_ptr[0] << 4) >> 4;
            const int32_t rhs_high0 = rhs_ptr[0] >> 4;
            const int32_t rhs_low1 = (int8_t)(rhs_ptr[rhs_offset] << 4) >> 4;
            const int32_t rhs_high1 = rhs_ptr[rhs_offset] >> 4;

            const int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
            const int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

            res0 += lhs_low * rhs_low0;
            res0 += lhs_high * rhs_high0;
            res1 += lhs_low * rhs_low1;
            res1 += lhs_high * rhs_high1;

            ++rhs_ptr;
            lhs_ptr += 2;
        }

        // Quantize down
        res0 = riscv_nn_requantize(res0, dst_multiplier, dst_shift);
        res1 = riscv_nn_requantize(res1, dst_multiplier, dst_shift);

        // Add offset
        res0 += dst_offset;
        res1 += dst_offset;

        // Clamp the result
        res0 = MAX(res0, activation_min);
        res0 = MIN(res0, activation_max);
        res1 = MAX(res1, activation_min);
        res1 = MIN(res1, activation_max);

        *dst = (int8_t)res0;
        *(dst + 2 * address_offset * ((rhs_rows) / 4)) = (int8_t)res1;
        dst += address_offset;
    }

    const int8_t *lhs_ptr = &lhs[0];
    spillover0 = 0;

    for (int i_row_loop_cnt = 0; i_row_loop_cnt < rhs_rows % 4; ++i_row_loop_cnt)
    {
        int32_t res0 = spillover0;
        if (bias)
        {
            res0 += bias[2 * (rhs_rows / 4)];
            ++bias;
        }

        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols / 2; ++rhs_cols_idx)
        {
            const int32_t rhs_low0 = (int8_t)(rhs_ptr[rhs_offset] << 4) >> 4;
            const int32_t rhs_high0 = rhs_ptr[rhs_offset] >> 4;

            const int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
            const int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

            res0 += lhs_low * rhs_low0;
            res0 += lhs_high * rhs_high0;

            ++rhs_ptr;
            lhs_ptr += 2;
        }

        if ((rhs_cols % 2 == 1) && (i_row_loop_cnt % 2 == 0))
        {
            const int32_t rhs_low0 = (int8_t)(rhs_ptr[rhs_offset] << 4) >> 4;
            const int32_t rhs_high0 = rhs_ptr[rhs_offset] >> 4;

            const int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
            lhs_ptr = &lhs[0];
            const int32_t lhs_high = (int8_t)lhs_ptr[0] + lhs_offset;
            ++lhs_ptr;

            res0 += lhs_low * rhs_low0;
            spillover0 = lhs_high * rhs_high0;
            ++rhs_ptr;
        }
        else
        {
            spillover0 = 0;
            lhs_ptr = &lhs[0];
        }

        // Quantize down
        res0 = riscv_nn_requantize(res0, dst_multiplier, dst_shift);

        // Add offset
        res0 += dst_offset;

        // Clamp the result
        res0 = MAX(res0, activation_min);
        res0 = MIN(res0, activation_max);

        *(dst + 2 * address_offset * ((rhs_rows) / 4)) = (int8_t)res0;
        dst += address_offset;
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
