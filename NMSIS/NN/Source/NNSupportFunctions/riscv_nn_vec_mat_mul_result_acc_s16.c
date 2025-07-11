/*
 * SPDX-FileCopyrightText: Copyright 2020-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_vec_mat_mul_result_acc_s16
 * Description:  s16 vector by matrix (transposed) multiplication
 *
 * $Date:        26 March 2023
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
 * @addtogroup supportFC
 * @{
 */

/*
 * s16 vector(lhs) by matrix (transposed) multiplication with result accumulation
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_vec_mat_mul_result_acc_s16(const int16_t *lhs,
                                                      const int8_t *rhs,
                                                      const int64_t *effective_bias,
                                                      int16_t *dst,
                                                      const int32_t dst_multiplier,
                                                      const int32_t dst_shift,
                                                      const int32_t rhs_cols,
                                                      const int32_t rhs_rows,
                                                      const int32_t batches,
                                                      const int32_t batch_offset)
{

    int32_t reduced_multiplier = REDUCE_MULTIPLIER(dst_multiplier);

    for (int batch = 0; batch < batches; batch++)
    {

        const int8_t *rhs_ptr = &rhs[0];
        const int64_t *effective_bias_ptr = &effective_bias[0];

#if defined(RISCV_MATH_DSP)

        int32_t rhs_cols_fast = rhs_cols;

        if (rhs_cols > MAX_COL_COUNT)
        {
            rhs_cols_fast = MAX_COL_COUNT;
        }

        const int32_t row_loop_cnt = rhs_rows / 2;

        for (int32_t i = 0; i < row_loop_cnt; i++)
        {

            int64_t acc_64_0 = 0;
            int64_t acc_64_1 = 0;
            int32_t acc_0 = 0;
            int32_t acc_1 = 0;

            const int32_t col_loop_cnt = rhs_cols_fast / 4;

            const int16_t *lhs_vec = lhs;
            const int8_t *rhs_0 = rhs_ptr;
            rhs_ptr += rhs_cols;
            const int8_t *rhs_1 = rhs_ptr;
            rhs_ptr += rhs_cols;

            for (int j = col_loop_cnt; j != 0; j--)
            {
                int32_t ker_0, ker_1, vec_part_0, vec_part_1;

                vec_part_0 = riscv_nn_read_q15x2_ia(&lhs_vec);
                vec_part_1 = riscv_nn_read_q15x2_ia(&lhs_vec);

                rhs_0 = read_and_pad(rhs_0, &ker_0, &ker_1);

                acc_0 = __SMLAD(ker_0, vec_part_0, acc_0);
                acc_0 = __SMLAD(ker_1, vec_part_1, acc_0);

                rhs_1 = read_and_pad(rhs_1, &ker_0, &ker_1);

                acc_1 = __SMLAD(ker_0, vec_part_0, acc_1);
                acc_1 = __SMLAD(ker_1, vec_part_1, acc_1);
            }

            acc_64_0 += acc_0;
            acc_64_1 += acc_1;

            for (int k = col_loop_cnt * 4; k < rhs_cols; k++)
            {
                const int32_t lhs_temp = (*lhs_vec);
                lhs_vec++;
                acc_64_0 += lhs_temp * (*rhs_0);
                rhs_0++;
                acc_64_1 += lhs_temp * (*rhs_1);
                rhs_1++;
            }

            acc_64_0 += *effective_bias_ptr++;
            acc_64_1 += *effective_bias_ptr++;
            int32_t tmp;

            tmp = riscv_nn_requantize_s64(acc_64_0, reduced_multiplier, dst_shift);
            tmp += (int64_t)*dst;
            tmp = MAX(tmp, NN_Q15_MIN);
            tmp = MIN(tmp, NN_Q15_MAX);
            *dst++ = (int16_t)tmp;

            tmp = riscv_nn_requantize_s64(acc_64_1, reduced_multiplier, dst_shift);
            tmp += (int64_t)*dst;
            tmp = MAX(tmp, NN_Q15_MIN);
            tmp = MIN(tmp, NN_Q15_MAX);
            *dst++ = (int16_t)tmp;
        }

        if (rhs_rows & 0x1)
        {
            int64_t acc_64_0 = 0;
            int32_t acc_0 = 0;
            const int32_t col_loop_cnt = rhs_cols_fast / 4;

            const int16_t *lhs_vec = lhs;
            const int8_t *rhs_0 = rhs_ptr;

            for (int i = col_loop_cnt; i != 0; i--)
            {
                int32_t ker_0, ker_1, vec;
                rhs_0 = read_and_pad(rhs_0, &ker_0, &ker_1);

                vec = riscv_nn_read_q15x2_ia(&lhs_vec);
                acc_0 = __SMLAD(ker_0, vec, acc_0);

                vec = riscv_nn_read_q15x2_ia(&lhs_vec);
                acc_0 = __SMLAD(ker_1, vec, acc_0);
            }

            acc_64_0 += acc_0;

            for (int j = col_loop_cnt * 4; j < rhs_cols; j++)
            {
                const int32_t lhs_temp = (*lhs_vec);
                lhs_vec++;
                acc_64_0 += lhs_temp * (*rhs_0);
                rhs_0++;
            }

            acc_64_0 += *effective_bias_ptr++;

            int32_t tmp;
            tmp = riscv_nn_requantize_s64(acc_64_0, reduced_multiplier, dst_shift);
            tmp += (int64_t)*dst;
            tmp = MAX(tmp, NN_Q15_MIN);
            tmp = MIN(tmp, NN_Q15_MAX);
            *dst++ = (int16_t)tmp;
        }

#else      // RISCV_MATH_DSP
        for (int i_row_loop_cnt = 0; i_row_loop_cnt < rhs_rows; i_row_loop_cnt++)
        {
            const int16_t *lhs_ptr = lhs;

            int64_t result = *effective_bias_ptr++;

            for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
            {
                const int64_t rhs_value0 = (int8_t)*rhs_ptr;
                const int64_t lhs_value = *lhs_ptr;

                result += lhs_value * rhs_value0;
                ++rhs_ptr;
                ++lhs_ptr;
            }

            // Quantize down
            result = riscv_nn_requantize_s64(result, reduced_multiplier, dst_shift);
            result += (int64_t)*dst;

            // Clamp the result
            result = ((result) > (NN_Q15_MIN) ? (result) : (NN_Q15_MIN));
            result = ((result) < (NN_Q15_MAX) ? (result) : (NN_Q15_MAX));

            *dst++ = (int16_t)result;
        }
#endif     // RISCV_MATH_DSP

        lhs += rhs_cols * batch_offset;
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
