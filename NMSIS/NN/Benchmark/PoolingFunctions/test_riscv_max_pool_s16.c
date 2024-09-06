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


#include <riscv_nnfunctions.h>

#include "TestData/maxpool_int16/test_data.h"
#include "Utils/validate.h"


#define REPEAT_NUM (2)
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void maxpool_int16_riscv_max_pool_s16(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int16_t output[MAXPOOL_INT16_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_pool_params pool_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims output_dims;

    const int16_t *input_data = maxpool_int16_input;

    input_dims.n = MAXPOOL_INT16_INPUT_BATCHES;
    input_dims.w = MAXPOOL_INT16_INPUT_W;
    input_dims.h = MAXPOOL_INT16_INPUT_H;
    input_dims.c = MAXPOOL_INT16_IN_CH;
    filter_dims.w = MAXPOOL_INT16_FILTER_X;
    filter_dims.h = MAXPOOL_INT16_FILTER_Y;
    output_dims.w = MAXPOOL_INT16_OUTPUT_W;
    output_dims.h = MAXPOOL_INT16_OUTPUT_H;
    output_dims.c = MAXPOOL_INT16_OUT_CH;

    pool_params.padding.w = MAXPOOL_INT16_PAD_X;
    pool_params.padding.h = MAXPOOL_INT16_PAD_Y;
    pool_params.stride.w = MAXPOOL_INT16_STRIDE_X;
    pool_params.stride.h = MAXPOOL_INT16_STRIDE_Y;

    pool_params.activation.min = MAXPOOL_INT16_OUT_ACTIVATION_MIN;
    pool_params.activation.max = MAXPOOL_INT16_OUT_ACTIVATION_MAX;

    generate_rand_s16(maxpool_int16_input, MAXPOOL_INT16_INPUT_SIZE);

    BENCH_START(riscv_max_pool_s16);
    for (int i = 0; i < REPEAT_NUM; i++)
    {
        riscv_nmsis_nn_status result =
            riscv_max_pool_s16(&ctx, &pool_params, &input_dims, input_data, &filter_dims, &output_dims, output);

        //TEST_ASSERT_EQUAL(expected, result);
        //TEST_ASSERT_TRUE(validate_s16(output, maxpool_int16_output_ref, MAXPOOL_INT16_DST_SIZE));
    }
    BENCH_SAMPLE(riscv_max_pool_s16);
    printf("CSV, riscv_max_pool_s16, %lu\n", (unsigned long)(BENCH_GET_USECYC())/REPEAT_NUM);
}
