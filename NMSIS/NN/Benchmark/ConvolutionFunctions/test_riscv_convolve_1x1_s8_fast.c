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

#include <riscv_nnfunctions.h>
#include <stdlib.h>


#include "TestData/kernel1x1/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void kernel1x1_riscv_convolve_1x1_s8_fast(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[KERNEL1X1_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_conv_params conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int32_t *bias_data = kernel1x1_biases;
    const int8_t *input_data = kernel1x1_input;

    input_dims.n = KERNEL1X1_INPUT_BATCHES;
    input_dims.h = KERNEL1X1_INPUT_H;
    input_dims.w = KERNEL1X1_INPUT_W;
    input_dims.c = KERNEL1X1_IN_CH;
    filter_dims.n = KERNEL1X1_OUT_CH;
    filter_dims.h = KERNEL1X1_FILTER_Y;
    filter_dims.w = KERNEL1X1_FILTER_X;
    filter_dims.c = KERNEL1X1_IN_CH;
    output_dims.n = KERNEL1X1_INPUT_BATCHES;
    output_dims.h = KERNEL1X1_OUTPUT_H;
    output_dims.w = KERNEL1X1_OUTPUT_W;
    output_dims.c = KERNEL1X1_OUT_CH;

    conv_params.padding.h = KERNEL1X1_PAD_Y;
    conv_params.padding.w = KERNEL1X1_PAD_X;
    conv_params.stride.h = KERNEL1X1_STRIDE_Y;
    conv_params.stride.w = KERNEL1X1_STRIDE_X;

    bias_dims.n = 1;
    bias_dims.h = 1;
    bias_dims.w = 1;
    bias_dims.c = output_dims.c;

    conv_params.input_offset = KERNEL1X1_INPUT_OFFSET;
    conv_params.output_offset = KERNEL1X1_OUTPUT_OFFSET;
    conv_params.activation.min = KERNEL1X1_OUT_ACTIVATION_MIN;
    conv_params.activation.max = KERNEL1X1_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)kernel1x1_output_mult;
    quant_params.shift = (int32_t *)kernel1x1_output_shift;

    generate_rand_s8(kernel1x1_input, KERNEL1X1_INPUT_SIZE);
    generate_rand_s8(kernel1x1_weights, KERNEL1X1_KERNEL_SIZE);

    const int32_t buf_size = riscv_convolve_1x1_s8_fast_get_buffer_size(&input_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = 0;

    BENCH_START(riscv_convolve_1x1_s8_fast);
    riscv_nmsis_nn_status result = riscv_convolve_1x1_s8_fast(&ctx,
                                                          &conv_params,
                                                          &quant_params,
                                                          &input_dims,
                                                          input_data,
                                                          &filter_dims,
                                                          kernel1x1_weights,
                                                          &bias_dims,
                                                          bias_data,
                                                          &output_dims,
                                                          output);
    BENCH_END(riscv_convolve_1x1_s8_fast);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
//    TEST_ASSERT_TRUE(validate(output, kernel1x1_output_ref, KERNEL1X1_DST_SIZE));
}
