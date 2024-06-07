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

#include "../TestData/maxpooling/test_data.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

#define REPEAT_NUM (1)

void maxpooling_riscv_max_pool_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[MAXPOOLING_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_pool_params pool_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims output_dims;

    const int8_t *input_data = maxpooling_input;

    input_dims.n = MAXPOOLING_INPUT_BATCHES;
    input_dims.w = MAXPOOLING_INPUT_W;
    input_dims.h = MAXPOOLING_INPUT_H;
    input_dims.c = MAXPOOLING_IN_CH;
    filter_dims.w = MAXPOOLING_FILTER_X;
    filter_dims.h = MAXPOOLING_FILTER_Y;
    output_dims.w = MAXPOOLING_OUTPUT_W;
    output_dims.h = MAXPOOLING_OUTPUT_H;
    output_dims.c = MAXPOOLING_OUT_CH;

    pool_params.padding.w = MAXPOOLING_PAD_X;
    pool_params.padding.h = MAXPOOLING_PAD_Y;
    pool_params.stride.w = MAXPOOLING_STRIDE_X;
    pool_params.stride.h = MAXPOOLING_STRIDE_Y;

    pool_params.activation.min = MAXPOOLING_OUT_ACTIVATION_MIN;
    pool_params.activation.max = MAXPOOLING_OUT_ACTIVATION_MAX;

    generate_rand_s8(maxpooling_input, MAXPOOLING_INPUT_BATCHES * MAXPOOLING_INPUT_H * MAXPOOLING_INPUT_W * MAXPOOLING_IN_CH);

    BENCH_START(riscv_max_pool_s8);
    for (int i = 0; i < REPEAT_NUM; i++)
    {
        riscv_nmsis_nn_status result = riscv_max_pool_s8(&ctx, &pool_params, &input_dims, input_data, &filter_dims, &output_dims, output);

        //TEST_ASSERT_EQUAL(expected, result);
        //TEST_ASSERT_TRUE(validate(output, maxpooling_output_ref, MAXPOOLING_DST_SIZE));
    }
    BENCH_SAMPLE(riscv_max_pool_s8);
    printf("CSV, riscv_max_pool_s8, %lu\n", (unsigned long)(BENCH_GET_USECYC())/REPEAT_NUM);
}
