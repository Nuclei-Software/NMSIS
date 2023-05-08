/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2022 Nuclei Limited. All rights reserved.
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
 * Title:        riscv_elementwise_mul_s16_s8.c
 * Description:  Elementwise multiplication of 16 bit input with 8 bit output
 *
 * $Date:        20 January 2023
 * $Revision:    V.1.2.0
 *
 * Target :  Arm(R) M-Profile Architecture
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup groupSupport
 */

/**
 * @addtogroup BasicMath
 * @{
 */

/*
 * s16 elementwise multiplication with s8 output
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_elementwise_mul_s16_s8(const int16_t *input_1_vect,
                                               const int16_t *input_2_vect,
                                               int8_t *output,
                                               const int32_t out_offset,
                                               const int32_t out_mult,
                                               const int32_t out_shift,
                                               const int32_t block_size)
{
    int32_t loop_count = block_size;

#if defined(RISCV_MATH_DSP)

    while (loop_count > 1)
    {
        int32_t input_1 = riscv_nn_read_q15x2_ia(&input_1_vect);
        int32_t input_2 = riscv_nn_read_q15x2_ia(&input_2_vect);

        int32_t mul_res = __SMBB16(input_1, input_2);
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift) + out_offset;
        mul_res = CLAMP(mul_res, NN_Q7_MAX, NN_Q7_MIN);
        int32_t mul = (int16_t)(mul_res & 0xFF);

        mul_res = __SMTT16(input_1, input_2);
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift) + out_offset;
        mul_res = CLAMP(mul_res, NN_Q7_MAX, NN_Q7_MIN);
        mul |= (int16_t)mul_res << 8;

        riscv_nn_write_s8x2_ia(&output, mul);
        loop_count -= 2;
    }
#else
    for (int i = 0; i < loop_count; i++)
    {
        /* C = A * B */
        int32_t mul_res = input_1_vect[i] * input_2_vect[i];
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift) + out_offset;

        mul_res = CLAMP(mul_res, NN_Q7_MAX, NN_Q7_MIN);

        output[i] = (int8_t)mul_res;
    }

#endif /* if defined(RISCV_MATH_DSP) */

    return RISCV_NMSIS_NN_SUCCESS;
}
/**
 * @} end of Elementwise group
 */
