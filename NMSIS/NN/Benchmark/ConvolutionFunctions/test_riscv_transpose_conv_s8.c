/*
 * SPDX-FileCopyrightText: Copyright 2023 Arm Limited and/or its affiliates <open-source-office@riscv.com>
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


#include "../TestData/transpose_conv_1/test_data.h"
#include "../Utils/utils.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void transpose_conv_1_riscv_transpose_conv_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[TRANSPOSE_CONV_1_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_context output_ctx;
    nmsis_nn_transpose_conv_params transpose_conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims = {0};
    nmsis_nn_dims output_dims;

    const int32_t *bias_data = transpose_conv_1_biases;
    const int8_t *kernel_data = transpose_conv_1_weights;
    const int8_t *input_data = transpose_conv_1_input;
    const int8_t *output_ref = transpose_conv_1_output_ref;
    const int32_t output_ref_size = TRANSPOSE_CONV_1_DST_SIZE;

    input_dims.n = TRANSPOSE_CONV_1_INPUT_BATCHES;
    input_dims.w = TRANSPOSE_CONV_1_INPUT_W;
    input_dims.h = TRANSPOSE_CONV_1_INPUT_H;
    input_dims.c = TRANSPOSE_CONV_1_IN_CH;
    filter_dims.w = TRANSPOSE_CONV_1_FILTER_X;
    filter_dims.h = TRANSPOSE_CONV_1_FILTER_Y;
    output_dims.n = TRANSPOSE_CONV_1_INPUT_BATCHES;
    output_dims.w = TRANSPOSE_CONV_1_OUTPUT_W;
    output_dims.h = TRANSPOSE_CONV_1_OUTPUT_H;
    output_dims.c = TRANSPOSE_CONV_1_OUT_CH;

    output_ctx.size = output_dims.w * output_dims.h * output_dims.c * sizeof(int32_t);
    output_ctx.buf = malloc(output_ctx.size);

    transpose_conv_params.padding.w = TRANSPOSE_CONV_1_PAD_X;
    transpose_conv_params.padding.h = TRANSPOSE_CONV_1_PAD_Y;
    transpose_conv_params.padding_offsets.w = TRANSPOSE_CONV_1_PAD_X_WITH_OFFSET;
    transpose_conv_params.padding_offsets.h = TRANSPOSE_CONV_1_PAD_Y_WITH_OFFSET;

    transpose_conv_params.stride.w = TRANSPOSE_CONV_1_STRIDE_X;
    transpose_conv_params.stride.h = TRANSPOSE_CONV_1_STRIDE_Y;
    transpose_conv_params.dilation.w = TRANSPOSE_CONV_1_DILATION_X;
    transpose_conv_params.dilation.h = TRANSPOSE_CONV_1_DILATION_Y;

    transpose_conv_params.input_offset = TRANSPOSE_CONV_1_INPUT_OFFSET;
    transpose_conv_params.output_offset = TRANSPOSE_CONV_1_OUTPUT_OFFSET;
    transpose_conv_params.activation.min = TRANSPOSE_CONV_1_OUT_ACTIVATION_MIN;
    transpose_conv_params.activation.max = TRANSPOSE_CONV_1_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)transpose_conv_1_output_mult;
    quant_params.shift = (int32_t *)transpose_conv_1_output_shift;

    generate_rand_s8(transpose_conv_1_input, TRANSPOSE_CONV_1_INPUT_BATCHES * TRANSPOSE_CONV_1_INPUT_H * TRANSPOSE_CONV_1_INPUT_W * TRANSPOSE_CONV_1_IN_CH);
    generate_rand_s8(transpose_conv_1_weights, TRANSPOSE_CONV_1_OUT_CH * TRANSPOSE_CONV_1_FILTER_Y * TRANSPOSE_CONV_1_FILTER_X * TRANSPOSE_CONV_1_IN_CH);

    const int32_t buf_size = riscv_transpose_conv_s8_get_buffer_size(&input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    BENCH_START(riscv_transpose_conv_s8);
    riscv_nmsis_nn_status result = riscv_transpose_conv_s8(&ctx,
                                                       &output_ctx,
                                                       &transpose_conv_params,
                                                       &quant_params,
                                                       &input_dims,
                                                       input_data,
                                                       &filter_dims,
                                                       kernel_data,
                                                       &bias_dims,
                                                       bias_data,
                                                       &output_dims,
                                                       output);
    BENCH_END(riscv_transpose_conv_s8);

    if (output_ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(output_ctx.buf, 0, output_ctx.size);
        free(output_ctx.buf);
    }

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, output_ref, output_ref_size));
}