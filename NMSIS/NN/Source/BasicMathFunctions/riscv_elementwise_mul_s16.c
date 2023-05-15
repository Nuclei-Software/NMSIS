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
 * Title:        riscv_elementwise_mul_s16
 * Description:  Element wise multiplication
 *
 * $Date:        20 January 2023
 * $Revision:    V.2.4.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup Public
 */

/**
 * @addtogroup groupElementwise
 * @{
 */

/**
 * @brief s16 element wise multiplication of two vectors
 *
 * @note   Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_elementwise_mul_s16(const int16_t *input_1_vect,
                                            const int16_t *input_2_vect,
                                            const int32_t input_1_offset,
                                            const int32_t input_2_offset,
                                            int16_t *output,
                                            const int32_t out_offset,
                                            const int32_t out_mult,
                                            const int32_t out_shift,
                                            const int32_t out_activation_min,
                                            const int32_t out_activation_max,
                                            const int32_t block_size)
{
    (void)input_1_offset;
    (void)input_2_offset;
    (void)out_offset;
    int32_t loop_count;
    int32_t input_1;
    int32_t input_2;
    int32_t mul_res;
#if defined(RISCV_MATH_VECTOR)
    int32_t blkCnt = block_size & (~RVV_OPT_THRESHOLD); /* Loop counter */
    size_t l;
    vint32m4_t input_1_m4;
    vint32m4_t input_2_m4;
    vint32m4_t sum0m4;

    for (; (l = vsetvl_e16m2(blkCnt)) > 0; blkCnt -= l) {
        input_1_m4 = vwadd_vx_i32m4(vle16_v_i16m2(input_1_vect, l), 0, l);
        input_1_vect += l;
        input_2_m4 = vwadd_vx_i32m4(vle16_v_i16m2(input_2_vect, l), 0, l);
        input_2_vect += l;

        sum0m4 = vmul_vv_i32m4(input_1_m4, input_2_m4, l);
        /* mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift); */
        sum0m4 = riscv_nn_requantize_m4_rvv(sum0m4, l, out_mult, out_shift);

        sum0m4 = vmax_vx_i32m4(sum0m4, out_activation_min, l);
        sum0m4 = vmin_vx_i32m4(sum0m4, out_activation_max, l);
        vse16_v_i16m2(output, vnsra_wx_i16m2(sum0m4, 0, l), l);
        output += l;
    }
    loop_count = block_size & RVV_OPT_THRESHOLD;
#else
    int32_t two_halfword_1, two_halfword_2;
    int16_t mul_1, mul_2;
    loop_count = block_size / 2;

    while (loop_count > 0)
    {
        two_halfword_1 = riscv_nn_read_q15x2_ia(&input_1_vect);
        two_halfword_2 = riscv_nn_read_q15x2_ia(&input_2_vect);

    #if defined(RISCV_MATH_DSP)
        mul_res = __SMBB16(two_halfword_1, two_halfword_2);
    #else
        input_1 = (int16_t)(two_halfword_1 & 0xFFFF);
        input_2 = (int16_t)(two_halfword_2 & 0xFFFF);
        mul_res = input_1 * input_2;
    #endif
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift);
        mul_res = MAX(mul_res, out_activation_min);
        mul_res = MIN(mul_res, out_activation_max);
        mul_1 = (int16_t)mul_res;

    #if defined(RISCV_MATH_DSP)
        mul_res = __SMTT16(two_halfword_1, two_halfword_2);
    #else
        input_1 = (int16_t)(two_halfword_1 >> 16);
        input_2 = (int16_t)(two_halfword_2 >> 16);
        mul_res = input_1 * input_2;
    #endif
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift);
        mul_res = MAX(mul_res, out_activation_min);
        mul_res = MIN(mul_res, out_activation_max);
        mul_2 = (int16_t)mul_res;

        riscv_nn_write_q15x2_ia(&output, PACK_Q15x2_32x1(mul_1, mul_2));

        loop_count--;
    }
    loop_count = block_size & 0x1;
#endif /* defined(RISCV_MATH_VECTOR) */
    while (loop_count > 0)
    {
        /* C = A * B */

        input_1 = *input_1_vect++;
        input_2 = *input_2_vect++;

        mul_res = input_1 * input_2;
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift);

        mul_res = MAX(mul_res, out_activation_min);
        mul_res = MIN(mul_res, out_activation_max);

        *output++ = (int16_t)mul_res;

        /* Decrement loop counter */
        loop_count--;
    }
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Elementwise group
 */
