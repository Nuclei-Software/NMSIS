/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@riscv.com>
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


#include <riscv_nnfunctions.h>

#include "TestData/softmax/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

#define REPEAT_NUM (2)

void softmax_riscv_softmax_s8(void)
{
    const int32_t num_rows = SOFTMAX_NUM_ROWS;
    const int32_t row_size = SOFTMAX_ROW_SIZE;
    const int32_t mult = SOFTMAX_INPUT_MULT;
    const int32_t shift = SOFTMAX_INPUT_LEFT_SHIFT;
    const int32_t diff_min = SOFTMAX_DIFF_MIN;
    const int8_t *input_data = softmax_input;
    int8_t output[SOFTMAX_DST_SIZE];

    generate_rand_s8(softmax_input, SOFTMAX_DST_SIZE);

    BENCH_START(riscv_softmax_s8);
    for (int i = 0; i < REPEAT_NUM; i++)
    {
        riscv_softmax_s8(input_data, num_rows, row_size, mult, shift, diff_min, output);
        //TEST_ASSERT_TRUE(validate(output, softmax_output_ref, SOFTMAX_DST_SIZE));
    }
    BENCH_SAMPLE(riscv_softmax_s8);
    printf("CSV, riscv_softmax_s8, %lu\n", (unsigned long)(BENCH_GET_USECYC())/REPEAT_NUM);
}
