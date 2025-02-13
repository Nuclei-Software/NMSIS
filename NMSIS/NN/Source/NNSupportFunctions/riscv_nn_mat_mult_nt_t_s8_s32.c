/*
 * SPDX-FileCopyrightText: Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_mat_mult_nt_t_s8_s32
 * Description:  Matrix multiplication support function with the right-hand-side (rhs) matrix transposed
 *
 * $Date:        31 January 2024
 * $Revision:    V.1.1.0
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
 * s32 matrix multiplication with the right-hand-side matrix transposed
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_mat_mult_nt_t_s8_s32(const int8_t *lhs,
                                                const int8_t *rhs,
                                                int32_t *dst,
                                                const int32_t lhs_rows,
                                                const int32_t rhs_rows,
                                                const int32_t rhs_cols,
                                                const int32_t lhs_offset,
                                                const int32_t dst_idx_offset)
{
    int32_t rhs_rows_idx = rhs_rows;
    const int32_t dst_idx_col_offset = dst_idx_offset * rhs_cols;

#if defined(RISCV_MATH_DSP)
    int16_t lhs_offset_s16 = (int16_t)lhs_offset;
    const uint32_t lhs_offset_s16x2 = __PKHBT(lhs_offset_s16, lhs_offset_s16, 16);
    for (; rhs_rows_idx >= 8; rhs_rows_idx -= 8)
    {
        int32_t *dst_ptr = &dst[0];
        const int8_t *lhs_ptr = &lhs[0];
        int32_t lhs_rows_idx = lhs_rows >> 1;

        while (lhs_rows_idx)
        {
            const int8_t *rhs_ptr = &rhs[0];

            int32_t lhs000, lhs001, lhs010, lhs011, lhs100, lhs101, lhs110, lhs111;
            read_pad_and_add_s8(lhs_ptr, &lhs000, &lhs001, lhs_offset_s16x2);
            read_pad_and_add_s8(&lhs_ptr[4], &lhs010, &lhs011, lhs_offset_s16x2);
            read_pad_and_add_s8(&lhs_ptr[rhs_rows], &lhs100, &lhs101, lhs_offset_s16x2);
            read_pad_and_add_s8(&lhs_ptr[rhs_rows + 4], &lhs110, &lhs111, lhs_offset_s16x2);

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                int32_t rhs_val00, rhs_val01;
                read_and_pad(rhs_ptr, &rhs_val00, &rhs_val01);

                dst_ptr[0] = __SMLAD(lhs000, rhs_val00, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs001, rhs_val01, dst_ptr[0]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs100, rhs_val00, dst_ptr[dst_idx_col_offset]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs101, rhs_val01, dst_ptr[dst_idx_col_offset]);

                read_and_pad(&rhs_ptr[4], &rhs_val00, &rhs_val01);

                dst_ptr[0] = __SMLAD(lhs010, rhs_val00, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs011, rhs_val01, dst_ptr[0]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs110, rhs_val00, dst_ptr[dst_idx_col_offset]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs111, rhs_val01, dst_ptr[dst_idx_col_offset]);

                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
            dst_ptr += dst_idx_col_offset;

            lhs_ptr += rhs_rows << 1;

            lhs_rows_idx--;
        }
        // Left-over rows
        if (lhs_rows % 2)
        {
            const int8_t *rhs_ptr = &rhs[0];
            int32_t lhs00, lhs01, lhs10, lhs11;
            read_pad_and_add_s8(lhs_ptr, &lhs00, &lhs01, lhs_offset_s16x2);
            read_pad_and_add_s8(&lhs_ptr[4], &lhs10, &lhs11, lhs_offset_s16x2);

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                int32_t rhs_val00, rhs_val01, rhs_val10, rhs_val11;
                read_and_pad(rhs_ptr, &rhs_val00, &rhs_val01);
                read_and_pad(&rhs_ptr[4], &rhs_val10, &rhs_val11);

                dst_ptr[0] = __SMLAD(lhs00, rhs_val00, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs01, rhs_val01, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs10, rhs_val10, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs11, rhs_val11, dst_ptr[0]);

                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
        }

        rhs += 8;
        lhs += 8;
    }
    for (; rhs_rows_idx >= 4; rhs_rows_idx -= 4)
    {
        int32_t *dst_ptr = &dst[0];
        const int8_t *lhs_ptr = &lhs[0];

        int32_t lhs_rows_idx = lhs_rows >> 1;

        while (lhs_rows_idx)
        {
            const int8_t *rhs_ptr = &rhs[0];

            int32_t lhs00, lhs01, lhs10, lhs11;
            read_pad_and_add_s8(lhs_ptr, &lhs00, &lhs01, lhs_offset_s16x2);
            read_pad_and_add_s8(&lhs_ptr[rhs_rows], &lhs10, &lhs11, lhs_offset_s16x2);

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                int32_t rhs_val0, rhs_val1;
                read_and_pad(rhs_ptr, &rhs_val0, &rhs_val1);

                dst_ptr[0] = __SMLAD(lhs00, rhs_val0, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs01, rhs_val1, dst_ptr[0]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs10, rhs_val0, dst_ptr[dst_idx_col_offset]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs11, rhs_val1, dst_ptr[dst_idx_col_offset]);
                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
            dst_ptr += dst_idx_col_offset;

            lhs_ptr += rhs_rows << 1;

            lhs_rows_idx--;
        }
        // Left-over rows
        if (lhs_rows % 2)
        {
            const int8_t *rhs_ptr = &rhs[0];
            int32_t lhs00, lhs01;
            read_pad_and_add_s8(lhs_ptr, &lhs00, &lhs01, lhs_offset_s16x2);

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                int32_t rhs_val0, rhs_val1;
                read_and_pad(rhs_ptr, &rhs_val0, &rhs_val1);

                dst_ptr[0] = __SMLAD(lhs00, rhs_val0, dst_ptr[0]);
                dst_ptr[0] = __SMLAD(lhs01, rhs_val1, dst_ptr[0]);

                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
        }

        rhs += 4;
        lhs += 4;
    }
    for (; rhs_rows_idx >= 2; rhs_rows_idx -= 2)
    {
        int32_t *dst_ptr = &dst[0];
        const int8_t *lhs_ptr = &lhs[0];

        int32_t lhs_rows_idx = lhs_rows >> 1;

        while (lhs_rows_idx)
        {
            const int8_t *rhs_ptr = &rhs[0];

            int32_t lhs0, lhs1;
            read_pad_and_add_s8x2(lhs_ptr, &lhs0, lhs_offset_s16x2);
            read_pad_and_add_s8x2(&lhs_ptr[rhs_rows], &lhs1, lhs_offset_s16x2);

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                int32_t rhs_val;
                read_and_pad_s8x2(rhs_ptr, &rhs_val);

                dst_ptr[0] = __SMLAD(lhs0, rhs_val, dst_ptr[0]);
                dst_ptr[dst_idx_col_offset] = __SMLAD(lhs1, rhs_val, dst_ptr[dst_idx_col_offset]);

                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
            dst_ptr += dst_idx_col_offset;

            lhs_ptr += rhs_rows << 1;

            lhs_rows_idx--;
        }
        // Left-over rows
        if (lhs_rows % 2)
        {
            const int8_t *rhs_ptr = &rhs[0];
            const int32_t lhs_value = lhs_ptr[0] + lhs_offset;
            const int32_t lhs_value01 = lhs_ptr[1] + lhs_offset;

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                const int32_t rhs_value0 = rhs_ptr[0];
                const int32_t rhs_value01 = rhs_ptr[1];

                dst_ptr[0] += lhs_value * rhs_value0;
                dst_ptr[0] += lhs_value01 * rhs_value01;
                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
        }

        rhs += 2;
        lhs += 2;
    }
#else
    for (; rhs_rows_idx >= 2; rhs_rows_idx -= 2)
    {
        int32_t *dst_ptr = &dst[0];
        const int8_t *lhs_ptr = &lhs[0];

        int32_t lhs_rows_idx = lhs_rows >> 1;

        while (lhs_rows_idx)
        {
            const int8_t *rhs_ptr = &rhs[0];

            const int32_t lhs_value00 = lhs_ptr[0] + lhs_offset;
            const int32_t lhs_value01 = lhs_ptr[1] + lhs_offset;

            const int32_t lhs_value10 = lhs_ptr[rhs_rows] + lhs_offset;
            const int32_t lhs_value11 = lhs_ptr[rhs_rows + 1] + lhs_offset;

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                const int32_t rhs_value0 = rhs_ptr[0];
                const int32_t rhs_value1 = rhs_ptr[1];

                dst_ptr[0] += lhs_value00 * rhs_value0;
                dst_ptr[0] += lhs_value01 * rhs_value1;

                dst_ptr[dst_idx_col_offset] += lhs_value10 * rhs_value0;
                dst_ptr[dst_idx_col_offset] += lhs_value11 * rhs_value1;
                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
            dst_ptr += dst_idx_col_offset;

            lhs_ptr += rhs_rows << 1;

            lhs_rows_idx--;
        }
        // Left-over rows
        if (lhs_rows % 2)
        {
            const int8_t *rhs_ptr = &rhs[0];
            const int32_t lhs_value = lhs_ptr[0] + lhs_offset;
            const int32_t lhs_value01 = lhs_ptr[1] + lhs_offset;

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                const int32_t rhs_value0 = rhs_ptr[0];
                const int32_t rhs_value01 = rhs_ptr[1];

                dst_ptr[0] += lhs_value * rhs_value0;
                dst_ptr[0] += lhs_value01 * rhs_value01;
                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
        }

        rhs += 2;
        lhs += 2;
    }
#endif

    if (rhs_rows_idx)
    {
        const int8_t *lhs_ptr = &lhs[0];
        int32_t *dst_ptr = &dst[0];

        for (int32_t lhs_rows_idx = 0; lhs_rows_idx < lhs_rows; ++lhs_rows_idx)
        {
            const int8_t *rhs_ptr = &rhs[0];
            const int32_t lhs_value = lhs_ptr[0] + lhs_offset;

            for (int32_t rhs_cols_idx = rhs_cols; rhs_cols_idx != 0; rhs_cols_idx--)
            {
                const int32_t rhs_value = rhs_ptr[0];

                *dst_ptr += lhs_value * rhs_value;

                dst_ptr += dst_idx_offset;
                rhs_ptr += rhs_rows;
            }
            lhs_ptr += rhs_rows;
        }
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
