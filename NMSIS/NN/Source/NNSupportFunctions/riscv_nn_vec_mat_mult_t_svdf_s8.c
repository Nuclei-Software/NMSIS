/*
 * SPDX-FileCopyrightText: Copyright 2021-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_vec_mat_mult_t_svdf_s8
 * Description:  s8 vector by matrix (transposed) multiplication with
 *               s16 output. Targetted at SVDF operator.
 *
 * $Date:        28 March 2023
 * $Revision:    V.3.2.0
 *
 * Target Processor: RISC-V Cores
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
 * s8 vector(lhs) by matrix (transposed) multiplication
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_vec_mat_mult_t_svdf_s8(const int8_t *lhs,
                                                  const int8_t *rhs,
                                                  int16_t *dst,
                                                  const int32_t lhs_offset,
                                                  const int32_t dst_offset,
                                                  const int32_t dst_multiplier,
                                                  const int32_t dst_shift,
                                                  const int32_t rhs_cols,
                                                  const int32_t rhs_rows,
                                                  const int32_t activation_min,
                                                  const int32_t activation_max)
{
    if (rhs_cols < 0 || (NN_Q31_MAX - rhs_cols) < 16 || dst_offset < 0)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

#if defined(RISCV_MATH_VECTOR)
    int32_t blkCnt;    /* Loop counter */
    size_t l;

    vint32m8_t v_rhs0, v_rhs1, v_rhs2, v_lhs;
    vint32m1_t temp00m1, temp01m1, temp02m1;

    int32_t row_loop_cnt = rhs_rows / 3;
    for (int i_row_loop_cnt = 0; i_row_loop_cnt < row_loop_cnt; i_row_loop_cnt++)
    {
        const q7_t *lhs_ptr = lhs;
        const q7_t *rhs_ptr_0 = &rhs[0];
        const q7_t *rhs_ptr_1 = &rhs[rhs_cols];
        const q7_t *rhs_ptr_2 = &rhs[rhs_cols * 2];

        q31_t res00 = 0;
        q31_t res01 = 0;
        q31_t res02 = 0;

        blkCnt = rhs_cols;
        l = vsetvl_e32m1(1);
        temp00m1 = vmv_v_x_i32m1(0, l);
        temp01m1 = vmv_v_v_i32m1(temp00m1, l);
        temp02m1 = vmv_v_v_i32m1(temp00m1, l);

        for (; (l = vsetvl_e8m2(blkCnt)) > 0; blkCnt -= l) {
            v_rhs0 = vsext_vf4_i32m8(vle8_v_i8m2(rhs_ptr_0, l), l);
            v_rhs1 = vsext_vf4_i32m8(vle8_v_i8m2(rhs_ptr_1, l), l);
            v_rhs2 = vsext_vf4_i32m8(vle8_v_i8m2(rhs_ptr_2, l), l);
            v_lhs = vadd_vx_i32m8(vsext_vf4_i32m8(vle8_v_i8m2(lhs_ptr, l), l), lhs_offset, l);

            rhs_ptr_0 += l;
            rhs_ptr_1 += l;
            rhs_ptr_2 += l;
            lhs_ptr += l;

            temp00m1 = vredsum_vs_i32m8_i32m1(temp00m1, vmul_vv_i32m8(v_lhs, v_rhs0, l), temp00m1, l);
            temp01m1 = vredsum_vs_i32m8_i32m1(temp01m1, vmul_vv_i32m8(v_lhs, v_rhs1, l), temp01m1, l);
            temp02m1 = vredsum_vs_i32m8_i32m1(temp02m1, vmul_vv_i32m8(v_lhs, v_rhs2, l), temp02m1, l);
        }
        res00 += (q31_t)vmv_x_s_i32m1_i32(temp00m1);
        res01 += (q31_t)vmv_x_s_i32m1_i32(temp01m1);
        res02 += (q31_t)vmv_x_s_i32m1_i32(temp02m1);

        // Quantize down
        res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);
        res01 = riscv_nn_requantize(res01, dst_multiplier, dst_shift);
        res02 = riscv_nn_requantize(res02, dst_multiplier, dst_shift);

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);
        res01 = MAX(res01, activation_min);
        res01 = MIN(res01, activation_max);
        res02 = MAX(res02, activation_min);
        res02 = MIN(res02, activation_max);

        *dst = (q15_t)res00;
        *(dst + dst_offset) = (q15_t)res01;
        *(dst + 2 * dst_offset) = (q15_t)res02;
        dst += 3 * dst_offset;
        rhs += 3 * rhs_cols;
    }

    const int loop_cnt = rhs_rows % 3;

    for (int i_loop_cnt = 0; i_loop_cnt < loop_cnt; i_loop_cnt++)
    {
        const q7_t *lhs_ptr = &lhs[0];
        const q7_t *rhs_ptr_0 = &rhs[0];

        q31_t res00 = 0;

        blkCnt = rhs_cols;
        l = vsetvl_e32m1(1);
        temp00m1 = vmv_v_x_i32m1(0, l);
        for (; (l = vsetvl_e8m2(blkCnt)) > 0; blkCnt -= l) {
            v_rhs0 = vsext_vf4_i32m8(vle8_v_i8m2(rhs_ptr_0, l), l);
            v_lhs = vadd_vx_i32m8(vsext_vf4_i32m8(vle8_v_i8m2(lhs_ptr, l), l), lhs_offset, l);
            rhs_ptr_0 += l;
            lhs_ptr += l;
            temp00m1 = vredsum_vs_i32m8_i32m1(temp00m1, vmul_vv_i32m8(v_lhs, v_rhs0, l), temp00m1, l);
        }
        res00 += (q31_t)vmv_x_s_i32m1_i32(temp00m1);
        // Quantize down
        res00 = riscv_nn_requantize(res00, dst_multiplier, dst_shift);

        // Clamp the result
        res00 = CLAMP(res00, activation_max, activation_min);

        *dst = (q15_t)res00;
        dst += dst_offset;
        rhs += rhs_cols;
    }
#elif defined(RISCV_MATH_DSP)
    int32_t row_loop_cnt = rhs_rows / 2;

    const int16_t lhs_offset_s16 = lhs_offset;

    const uint32_t lhs_offset_s16x2 = __NN_PKHBT(lhs_offset_s16, lhs_offset_s16, 16);
    for (int32_t i = 0; i < row_loop_cnt; i++)
    {
        int32_t acc_0 = 0;
        int32_t acc_1 = 0;

        const int8_t *lhs_vec = lhs;
        const int8_t *rhs_0 = rhs;
        const int8_t *rhs_1 = rhs + rhs_cols;
        rhs += 2 * rhs_cols;

        int32_t rhs_cols_idx = 0;

        int32_t vec_0, vec_1, ker_0, ker_1;
        for (; rhs_cols_idx <= (rhs_cols - 16); rhs_cols_idx += 16)
        {
            // 4 x MAC acc_0, acc1
            vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);
            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_1);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_1 = __SMLAD(ker_1, vec_1, acc_1);
            acc_1 = __SMLAD(ker_0, vec_0, acc_1);
            // 4 x MAC acc_0, acc1
            vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);
            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_1);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_1 = __SMLAD(ker_1, vec_1, acc_1);
            acc_1 = __SMLAD(ker_0, vec_0, acc_1);

            // 4 x MAC acc_0, acc1
            vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);
            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_1);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_1 = __SMLAD(ker_1, vec_1, acc_1);
            acc_1 = __SMLAD(ker_0, vec_0, acc_1);

            // 4 x MAC acc_0, acc1
            vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);
            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);
            ker_0 = riscv_nn_read_s8x4_ia(&rhs_1);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);
            acc_1 = __SMLAD(ker_1, vec_1, acc_1);
            acc_1 = __SMLAD(ker_0, vec_0, acc_1);
        }
        for (; rhs_cols_idx <= (rhs_cols - 4); rhs_cols_idx += 4)
        {
            vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            vec_1 = __SXTAB16_RORn(lhs_offset_s16x2, (uint32_t)vec_0, 8);

            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);

            ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);

            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);

            ker_0 = riscv_nn_read_s8x4_ia(&rhs_1);
            ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);

            acc_1 = __SMLAD(ker_1, vec_1, acc_1);
            acc_1 = __SMLAD(ker_0, vec_0, acc_1);
        }
        for (; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
        {
            const int32_t lhs_temp = (*lhs_vec + lhs_offset);
            lhs_vec++;
            acc_0 += lhs_temp * (*rhs_0);
            rhs_0++;
            acc_1 += lhs_temp * (*rhs_1);
            rhs_1++;
        }

        acc_0 = riscv_nn_requantize(acc_0, dst_multiplier, dst_shift);
        acc_1 = riscv_nn_requantize(acc_1, dst_multiplier, dst_shift);

        // Clamp the result
        acc_0 = MAX(acc_0, activation_min);
        acc_0 = MIN(acc_0, activation_max);
        acc_1 = MAX(acc_1, activation_min);
        acc_1 = MIN(acc_1, activation_max);
        *dst = (int16_t)acc_0;
        *(dst + dst_offset) = (int16_t)acc_1;
        dst += 2 * dst_offset;
    }
    if (rhs_rows & 0x1)
    {
        int32_t acc_0 = 0;
        const int32_t col_loop_cnt = rhs_cols / 4;
        const int8_t *lhs_vec = lhs;
        const int8_t *rhs_0 = rhs;
        for (int i = col_loop_cnt; i != 0; i--)
        {
            int32_t vec_0 = riscv_nn_read_s8x4_ia(&lhs_vec);
            int32_t vec_1 = __SXTAB16(lhs_offset_s16x2, __ROR((uint32_t)vec_0, 8));
            vec_0 = __SXTAB16(lhs_offset_s16x2, vec_0);
            int32_t ker_0 = riscv_nn_read_s8x4_ia(&rhs_0);
            int32_t ker_1 = __SXTB16_RORn((uint32_t)ker_0, 8);
            ker_0 = __SXTB16(ker_0);

            acc_0 = __SMLAD(ker_1, vec_1, acc_0);
            acc_0 = __SMLAD(ker_0, vec_0, acc_0);
        }
        for (int j = col_loop_cnt * 4; j < rhs_cols; j++)
        {
            const int32_t lhs_temp = (*lhs_vec + lhs_offset);
            lhs_vec++;
            acc_0 += lhs_temp * *rhs_0;
            rhs_0++;
        }
        acc_0 = riscv_nn_requantize(acc_0, dst_multiplier, dst_shift);

        // Clamp the result
        acc_0 = MAX(acc_0, activation_min);
        acc_0 = MIN(acc_0, activation_max);
        *dst = (int16_t)acc_0;
        dst += dst_offset;
    }

#else

    int32_t row_loop_cnt = rhs_rows / 3;

    for (int i_row_loop_cnt = 0; i_row_loop_cnt < row_loop_cnt; i_row_loop_cnt++)
    {
        const int8_t *lhs_ptr = lhs;
        const int8_t *rhs_ptr_0 = &rhs[0];
        const int8_t *rhs_ptr_1 = &rhs[rhs_cols];
        const int8_t *rhs_ptr_2 = &rhs[rhs_cols * 2];

        int32_t res00 = 0;
        int32_t res01 = 0;
        int32_t res02 = 0;
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

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);
        res01 = MAX(res01, activation_min);
        res01 = MIN(res01, activation_max);
        res02 = MAX(res02, activation_min);
        res02 = MIN(res02, activation_max);

        *dst = (int16_t)res00;
        *(dst + dst_offset) = (int16_t)res01;
        *(dst + 2 * dst_offset) = (int16_t)res02;
        dst += 3 * dst_offset;
        rhs += 3 * rhs_cols;
    }

    const int loop_cnt = rhs_rows % 3;

    for (int i_loop_cnt = 0; i_loop_cnt < loop_cnt; i_loop_cnt++)
    {
        const int8_t *lhs_ptr = &lhs[0];
        const int8_t *rhs_ptr = &rhs[0];

        int32_t res00 = 0;

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

        // Clamp the result
        res00 = MAX(res00, activation_min);
        res00 = MIN(res00, activation_max);

        *dst = (int16_t)res00;
        dst += dst_offset;
        rhs += rhs_cols;
    }
#endif /* defined(RISCV_MATH_VECTOR) */

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
