/*
 * SPDX-FileCopyrightText: Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2024 Nuclei Limited. All rights reserved.
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
 * Title:        riscv_nn_mat_mult_nt_interleaved_t_even_s4
 * Description:  Matrix multiplication support function with the right-hand-side (rhs) matrix transposed, and 4 bit rhs.
 *
 * $Date:        04 Jun 2024
 * $Revision:    V.1.0.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConvolution
 * @{
 */

/*
 * s4 matrix multiplication with the left-hand side interleaved and the right-hand-side matrix transposed and columns
 * even
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_mat_mult_nt_interleaved_t_even_s4(const int8_t *lhs,
                                                             const int8_t *packed_rhs,
                                                             const int32_t *bias,
                                                             int8_t *dst,
                                                             const int32_t *dst_multipliers,
                                                             const int32_t *dst_shifts,
                                                             const int32_t lhs_rows,
                                                             const int32_t rhs_rows,
                                                             const int32_t rhs_cols,
                                                             const int32_t lhs_offset,
                                                             const int32_t dst_offset,
                                                             const int32_t activation_min,
                                                             const int32_t activation_max,
                                                             const int32_t lhs_cols_offset)
{
    const int32_t rhs_cols_int4 = rhs_cols >> 1;

    for (int32_t rhs_rows_idx = 0; rhs_rows_idx <= (rhs_rows - 4); rhs_rows_idx += 4)
    {
        const int8_t *lhs_ptr = &lhs[0];
        int8_t *dst_ptr = &dst[0];

        for (int32_t lhs_rows_idx = (lhs_rows >> 1); lhs_rows_idx > 0; --lhs_rows_idx)
        {
            // To avoid the issue of packed values leaking into the next rhs row
            // we instead evaluate the rhs rows in pairs like so:
            // rhs[0] and rhs[2], rhs[1] and rhs[3]

            // Start processing rhs_row[0] and rhs_row[2]
            const int8_t *packed_rhs_ptr = &packed_rhs[0];

            int32_t res00 = 0;
            int32_t res01 = 0;
            int32_t res10 = 0;
            int32_t res11 = 0;

            int32_t spillover00 = 0;
            int32_t spillover01 = 0;
            int32_t spillover10 = 0;
            int32_t spillover11 = 0;

            if (bias)
            {
                res00 = bias[rhs_rows_idx];
                res01 = bias[rhs_rows_idx + 2];
                res10 = bias[rhs_rows_idx];
                res11 = bias[rhs_rows_idx + 2];
            }

            for (int32_t rhs_cols_idx = rhs_cols_int4; rhs_cols_idx != 0; --rhs_cols_idx)
            {
                int8_t rhs_low0 = (int8_t)(packed_rhs_ptr[0] << 4) >> 4;
                int8_t rhs_high0 = packed_rhs_ptr[0] >> 4;
                int8_t rhs_low1 = (int8_t)(packed_rhs_ptr[rhs_cols] << 4) >> 4;
                int8_t rhs_high1 = packed_rhs_ptr[rhs_cols] >> 4;

                int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
                int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

                res00 += lhs_low * rhs_low0;
                res00 += lhs_high * rhs_high0;
                res01 += lhs_low * rhs_low1;
                res01 += lhs_high * rhs_high1;

                lhs_low = (int8_t)lhs_ptr[lhs_cols_offset] + lhs_offset;
                lhs_high = (int8_t)lhs_ptr[lhs_cols_offset + 1] + lhs_offset;

                res10 += lhs_low * rhs_low0;
                res10 += lhs_high * rhs_high0;
                res11 += lhs_low * rhs_low1;
                res11 += lhs_high * rhs_high1;

                ++packed_rhs_ptr;
                lhs_ptr += 2;
            }

            lhs_ptr -= rhs_cols;

            // Quantize down
            res00 = riscv_nn_requantize(res00, dst_multipliers[rhs_rows_idx], dst_shifts[rhs_rows_idx]);
            res01 = riscv_nn_requantize(res01, dst_multipliers[rhs_rows_idx + 2], dst_shifts[rhs_rows_idx + 2]);
            res10 = riscv_nn_requantize(res10, dst_multipliers[rhs_rows_idx], dst_shifts[rhs_rows_idx]);
            res11 = riscv_nn_requantize(res11, dst_multipliers[rhs_rows_idx + 2], dst_shifts[rhs_rows_idx + 2]);

            // Add offset
            res00 += dst_offset;
            res01 += dst_offset;
            res10 += dst_offset;
            res11 += dst_offset;

            // Clamp the result
            res00 = MAX(res00, activation_min);
            res00 = MIN(res00, activation_max);
            res01 = MAX(res01, activation_min);
            res01 = MIN(res01, activation_max);
            res10 = MAX(res10, activation_min);
            res10 = MIN(res10, activation_max);
            res11 = MAX(res11, activation_min);
            res11 = MIN(res11, activation_max);

            dst_ptr[0] = (int8_t)res00;
            dst_ptr[2] = (int8_t)res01;
            dst_ptr += rhs_rows;
            dst_ptr[0] = (int8_t)res10;
            dst_ptr[2] = (int8_t)res11;
            dst_ptr -= rhs_rows;

            // Start processing rhs_row[1] and rhs_row[3]
            res00 = spillover00;
            res01 = spillover01;
            res10 = spillover10;
            res11 = spillover11;
            if (bias)
            {
                res00 += bias[rhs_rows_idx + 1];
                res01 += bias[rhs_rows_idx + 3];
                res10 += bias[rhs_rows_idx + 1];
                res11 += bias[rhs_rows_idx + 3];
            }

            for (int32_t rhs_cols_idx = rhs_cols_int4; rhs_cols_idx != 0; --rhs_cols_idx)
            {
                int8_t rhs_low0 = (int8_t)(packed_rhs_ptr[0] << 4) >> 4;
                int8_t rhs_high0 = packed_rhs_ptr[0] >> 4;
                int8_t rhs_low1 = (int8_t)(packed_rhs_ptr[rhs_cols] << 4) >> 4;
                int8_t rhs_high1 = packed_rhs_ptr[rhs_cols] >> 4;

                int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
                int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

                res00 += lhs_low * rhs_low0;
                res00 += lhs_high * rhs_high0;
                res01 += lhs_low * rhs_low1;
                res01 += lhs_high * rhs_high1;

                lhs_low = (int8_t)lhs_ptr[lhs_cols_offset] + lhs_offset;
                lhs_high = (int8_t)lhs_ptr[lhs_cols_offset + 1] + lhs_offset;

                res10 += lhs_low * rhs_low0;
                res10 += lhs_high * rhs_high0;
                res11 += lhs_low * rhs_low1;
                res11 += lhs_high * rhs_high1;

                ++packed_rhs_ptr;
                lhs_ptr += 2;
            }

            // Quantize down
            res00 = riscv_nn_requantize(res00, dst_multipliers[rhs_rows_idx + 1], dst_shifts[rhs_rows_idx + 1]);
            res01 = riscv_nn_requantize(res01, dst_multipliers[rhs_rows_idx + 3], dst_shifts[rhs_rows_idx + 3]);
            res10 = riscv_nn_requantize(res10, dst_multipliers[rhs_rows_idx + 1], dst_shifts[rhs_rows_idx + 1]);
            res11 = riscv_nn_requantize(res11, dst_multipliers[rhs_rows_idx + 3], dst_shifts[rhs_rows_idx + 3]);

            // Add offset
            res00 += dst_offset;
            res01 += dst_offset;
            res10 += dst_offset;
            res11 += dst_offset;

            // Clamp the result
            res00 = MAX(res00, activation_min);
            res00 = MIN(res00, activation_max);
            res01 = MAX(res01, activation_min);
            res01 = MIN(res01, activation_max);
            res10 = MAX(res10, activation_min);
            res10 = MIN(res10, activation_max);
            res11 = MAX(res11, activation_min);
            res11 = MIN(res11, activation_max);

            dst_ptr[1] = (int8_t)res00;
            dst_ptr[3] = (int8_t)res01;
            dst_ptr += rhs_rows;
            dst_ptr[1] = (int8_t)res10;
            dst_ptr[3] = (int8_t)res11;
            dst_ptr += rhs_rows;

            lhs_ptr -= rhs_cols;
            lhs_ptr += 2 * lhs_cols_offset;
        }

        // Left-over row
        if (lhs_rows % 2)
        {
            const int8_t *packed_rhs_ptr = &packed_rhs[0];

            int32_t res00 = 0;
            int32_t res01 = 0;

            int32_t spillover00 = 0;
            int32_t spillover01 = 0;

            if (bias)
            {
                res00 += bias[rhs_rows_idx];
                res01 += bias[rhs_rows_idx + 2];
            }

            for (int32_t rhs_cols_idx = rhs_cols_int4; rhs_cols_idx != 0; --rhs_cols_idx)
            {
                int8_t rhs_low0 = (int8_t)(packed_rhs_ptr[0] << 4) >> 4;
                int8_t rhs_high0 = packed_rhs_ptr[0] >> 4;

                int8_t rhs_low1 = (int8_t)(packed_rhs_ptr[rhs_cols] << 4) >> 4;
                int8_t rhs_high1 = packed_rhs_ptr[rhs_cols] >> 4;

                int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
                int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

                res00 += lhs_low * rhs_low0;
                res00 += lhs_high * rhs_high0;

                res01 += lhs_low * rhs_low1;
                res01 += lhs_high * rhs_high1;

                ++packed_rhs_ptr;
                lhs_ptr += 2;
            }

            lhs_ptr -= rhs_cols;

            // Quantize down
            res00 = riscv_nn_requantize(res00, dst_multipliers[rhs_rows_idx], dst_shifts[rhs_rows_idx]);
            res01 = riscv_nn_requantize(res01, dst_multipliers[rhs_rows_idx + 2], dst_shifts[rhs_rows_idx + 2]);

            // Add offset
            res00 += dst_offset;
            res01 += dst_offset;

            // Clamp the result
            res00 = MAX(res00, activation_min);
            res00 = MIN(res00, activation_max);
            res01 = MAX(res01, activation_min);
            res01 = MIN(res01, activation_max);

            dst_ptr[0] = (int8_t)res00;
            dst_ptr[2] = (int8_t)res01;

            res00 = spillover00;
            res01 = spillover01;

            if (bias)
            {
                res00 += bias[rhs_rows_idx + 1];
                res01 += bias[rhs_rows_idx + 3];
            }

            for (int32_t rhs_cols_idx = rhs_cols_int4; rhs_cols_idx != 0; --rhs_cols_idx)
            {
                int8_t rhs_low0 = (int8_t)(packed_rhs_ptr[0] << 4) >> 4;
                int8_t rhs_high0 = packed_rhs_ptr[0] >> 4;

                int8_t rhs_low1 = (int8_t)(packed_rhs_ptr[rhs_cols] << 4) >> 4;
                int8_t rhs_high1 = packed_rhs_ptr[rhs_cols] >> 4;

                int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
                int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

                res00 += lhs_low * rhs_low0;
                res00 += lhs_high * rhs_high0;

                res01 += lhs_low * rhs_low1;
                res01 += lhs_high * rhs_high1;

                ++packed_rhs_ptr;
                lhs_ptr += 2;
            }

            // Quantize down
            res00 = riscv_nn_requantize(res00, dst_multipliers[rhs_rows_idx + 1], dst_shifts[rhs_rows_idx + 1]);
            res01 = riscv_nn_requantize(res01, dst_multipliers[rhs_rows_idx + 3], dst_shifts[rhs_rows_idx + 3]);

            // Add offset
            res00 += dst_offset;
            res01 += dst_offset;

            // Clamp the result
            res00 = MAX(res00, activation_min);
            res00 = MIN(res00, activation_max);
            res01 = MAX(res01, activation_min);
            res01 = MIN(res01, activation_max);

            dst_ptr[1] = (int8_t)res00;
            dst_ptr[3] = (int8_t)res01;
        }

        packed_rhs += 2 * rhs_cols;
        dst += 4;
    }

    const int32_t rhs_rows_finished = rhs_rows - (rhs_rows % 4);
    // Left over rhs rows will be in the range 0 -> 3
    for (int rhs_rows_idx = 0; rhs_rows_idx < rhs_rows % 4; ++rhs_rows_idx)
    {
        const int8_t *lhs_ptr = &lhs[0];
        int8_t *dst_ptr = &dst[0];

        for (int32_t lhs_rows_idx = (lhs_rows >> 1); lhs_rows_idx > 0; --lhs_rows_idx)
        {
            const int8_t *packed_rhs_ptr = &packed_rhs[0];
            int32_t res00 = 0;
            int32_t res10 = 0;
            if (bias)
            {
                res00 = bias[rhs_rows_finished + rhs_rows_idx];
                res10 = bias[rhs_rows_finished + rhs_rows_idx];
            }

            for (int32_t rhs_cols_idx = rhs_cols_int4; rhs_cols_idx != 0; --rhs_cols_idx)
            {
                int8_t rhs_low = (int8_t)(packed_rhs_ptr[0] << 4) >> 4;
                int8_t rhs_high = packed_rhs_ptr[0] >> 4;

                int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
                int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

                res00 += lhs_low * rhs_low;
                res00 += lhs_high * rhs_high;

                lhs_low = (int8_t)lhs_ptr[lhs_cols_offset] + lhs_offset;
                lhs_high = (int8_t)lhs_ptr[lhs_cols_offset + 1] + lhs_offset;

                res10 += lhs_low * rhs_low;
                res10 += lhs_high * rhs_high;

                ++packed_rhs_ptr;
                lhs_ptr += 2;
            }

            lhs_ptr -= rhs_cols;
            lhs_ptr += 2 * lhs_cols_offset;

            // Quantize down
            res00 = riscv_nn_requantize(
                res00, dst_multipliers[rhs_rows_finished + rhs_rows_idx], dst_shifts[rhs_rows_finished + rhs_rows_idx]);
            res10 = riscv_nn_requantize(
                res10, dst_multipliers[rhs_rows_finished + rhs_rows_idx], dst_shifts[rhs_rows_finished + rhs_rows_idx]);

            // Add offset
            res00 += dst_offset;
            res10 += dst_offset;

            // Clamp the result
            res00 = MAX(res00, activation_min);
            res00 = MIN(res00, activation_max);
            res10 = MAX(res10, activation_min);
            res10 = MIN(res10, activation_max);

            dst_ptr[0] = (int8_t)res00;
            dst_ptr += rhs_rows;
            dst_ptr[0] = (int8_t)res10;
            dst_ptr += rhs_rows;
        }
        if (lhs_rows % 2)
        {
            const int8_t *packed_rhs_ptr = &packed_rhs[0];
            int32_t res00 = 0;
            if (bias)
            {
                res00 = bias[rhs_rows_finished + rhs_rows_idx];
            }

            for (int32_t rhs_cols_idx = rhs_cols_int4; rhs_cols_idx != 0; --rhs_cols_idx)
            {
                int8_t rhs_low = (int8_t)(packed_rhs_ptr[0] << 4) >> 4;
                int8_t rhs_high = packed_rhs_ptr[0] >> 4;

                int32_t lhs_low = (int8_t)lhs_ptr[0] + lhs_offset;
                int32_t lhs_high = (int8_t)lhs_ptr[1] + lhs_offset;

                res00 += lhs_low * rhs_low;
                res00 += lhs_high * rhs_high;

                ++packed_rhs_ptr;
                lhs_ptr += 2;
            }

            // Quantize down
            res00 = riscv_nn_requantize(
                res00, dst_multipliers[rhs_rows_finished + rhs_rows_idx], dst_shifts[rhs_rows_finished + rhs_rows_idx]);

            // Add offset
            res00 += dst_offset;

            // Clamp the result
            res00 = MAX(res00, activation_min);
            res00 = MIN(res00, activation_max);

            dst_ptr[0] = (int8_t)res00;
        }

        packed_rhs += rhs_cols_int4;

        ++dst;
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
