/*
 * SPDX-FileCopyrightText: Copyright 2022 Arm Limited and/or its affiliates <open-source-office@riscv.com>
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

#include "riscv_nnfunctions.h"


#include "../TestData/mul_s16/test_data.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void mul_s16_riscv_elementwise_mul_s16(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int16_t output[MUL_S16_DST_SIZE] = {0};

    const int16_t *input_data1 = mul_s16_input1;
    const int16_t *input_data2 = mul_s16_input2;

    const int32_t input_1_offset = MUL_S16_INPUT1_OFFSET;
    const int32_t input_2_offset = MUL_S16_INPUT2_OFFSET;

    const int32_t out_offset = MUL_S16_OUTPUT_OFFSET;
    const int32_t out_mult = MUL_S16_OUTPUT_MULT;
    const int32_t out_shift = MUL_S16_OUTPUT_SHIFT;

    const int32_t out_activation_min = MUL_S16_OUT_ACTIVATION_MIN;
    const int32_t out_activation_max = MUL_S16_OUT_ACTIVATION_MAX;

    generate_rand_s16(mul_s16_input1, MUL_S16_DST_SIZE);
    generate_rand_s16(mul_s16_input2, MUL_S16_DST_SIZE);

    BENCH_START(riscv_elementwise_mul_s16);
    riscv_nmsis_nn_status result = riscv_elementwise_mul_s16(input_data1,
                                                         input_data2,
                                                         input_1_offset,
                                                         input_2_offset,
                                                         output,
                                                         out_offset,
                                                         out_mult,
                                                         out_shift,
                                                         out_activation_min,
                                                         out_activation_max,
                                                         MUL_S16_DST_SIZE);
    BENCH_END(riscv_elementwise_mul_s16);

    TEST_ASSERT_EQUAL(expected, result);
//    TEST_ASSERT_TRUE(validate_s16(output, mul_s16_output_ref, MUL_S16_DST_SIZE));
}
