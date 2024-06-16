/*
 * SPDX-FileCopyrightText: Copyright 2010-2023 Arm Limited and/or its affiliates <open-source-office@riscv.com>
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

#include "../TestData/avgpooling_int16/test_data.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void avgpooling_int16_riscv_avgpool_s16(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int16_t output[AVGPOOLING_INT16_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_pool_params pool_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims output_dims;

    const int16_t *input_data = avgpooling_int16_input;

    input_dims.n = AVGPOOLING_INT16_INPUT_BATCHES;
    input_dims.w = AVGPOOLING_INT16_INPUT_W;
    input_dims.h = AVGPOOLING_INT16_INPUT_H;
    input_dims.c = AVGPOOLING_INT16_IN_CH;
    filter_dims.w = AVGPOOLING_INT16_FILTER_X;
    filter_dims.h = AVGPOOLING_INT16_FILTER_Y;
    output_dims.w = AVGPOOLING_INT16_OUTPUT_W;
    output_dims.h = AVGPOOLING_INT16_OUTPUT_H;
    output_dims.c = AVGPOOLING_INT16_OUT_CH;

    pool_params.padding.w = AVGPOOLING_INT16_PAD_X;
    pool_params.padding.h = AVGPOOLING_INT16_PAD_Y;
    pool_params.stride.w = AVGPOOLING_INT16_STRIDE_X;
    pool_params.stride.h = AVGPOOLING_INT16_STRIDE_Y;

    pool_params.activation.min = AVGPOOLING_INT16_OUT_ACTIVATION_MIN;
    pool_params.activation.max = AVGPOOLING_INT16_OUT_ACTIVATION_MAX;

    generate_rand_s16(avgpooling_int16_input, AVGPOOLING_INT16_INPUT_SIZE);

    ctx.size = riscv_avgpool_s16_get_buffer_size(AVGPOOLING_INT16_OUTPUT_W, AVGPOOLING_INT16_IN_CH);
    ctx.buf = malloc(ctx.size);

    BENCH_START(riscv_avgpool_s16);
    riscv_nmsis_nn_status result =
        riscv_avgpool_s16(&ctx, &pool_params, &input_dims, input_data, &filter_dims, &output_dims, output);
    BENCH_END(riscv_avgpool_s16);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate_s16(output, avgpooling_int16_output_ref, AVGPOOLING_INT16_DST_SIZE));
}
