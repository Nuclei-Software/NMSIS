/*
 * Copyright (C) 2022 Arm Limited or its affiliates.
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


#include "TestData/add/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void add_riscv_elementwise_add_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[ADD_DST_SIZE] = {0};

    const int8_t *input_data1 = add_input1;
    const int8_t *input_data2 = add_input2;

    const int32_t input_1_mult = ADD_INPUT1_MULT;
    const int32_t input_1_shift = ADD_INPUT1_SHIFT;
    const int32_t input_1_offset = ADD_INPUT1_OFFSET;
    const int32_t input_2_mult = ADD_INPUT2_MULT;
    const int32_t input_2_shift = ADD_INPUT2_SHIFT;
    const int32_t input_2_offset = ADD_INPUT2_OFFSET;

    const int32_t left_shift = ADD_LEFT_SHIFT;

    const int32_t out_offset = ADD_OUTPUT_OFFSET;
    const int32_t out_mult = ADD_OUTPUT_MULT;
    const int32_t out_shift = ADD_OUTPUT_SHIFT;

    const int32_t out_activation_min = ADD_OUT_ACTIVATION_MIN;
    const int32_t out_activation_max = ADD_OUT_ACTIVATION_MAX;

    generate_rand_s8(add_input1, ADD_DST_SIZE);
    generate_rand_s8(add_input2, ADD_DST_SIZE);

    BENCH_START(riscv_elementwise_add_s8);
    riscv_nmsis_nn_status result = riscv_elementwise_add_s8(input_data1,
                                                        input_data2,
                                                        input_1_offset,
                                                        input_1_mult,
                                                        input_1_shift,
                                                        input_2_offset,
                                                        input_2_mult,
                                                        input_2_shift,
                                                        left_shift,
                                                        output,
                                                        out_offset,
                                                        out_mult,
                                                        out_shift,
                                                        out_activation_min,
                                                        out_activation_max,
                                                        ADD_DST_SIZE);
    BENCH_END(riscv_elementwise_add_s8);

    TEST_ASSERT_EQUAL(expected, result);
 //   TEST_ASSERT_TRUE(validate(output, add_output_ref, ADD_DST_SIZE));
}
