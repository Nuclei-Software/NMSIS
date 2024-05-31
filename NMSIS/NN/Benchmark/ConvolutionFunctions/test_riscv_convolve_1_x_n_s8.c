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

#include <stdlib.h>

#include <riscv_nnfunctions.h>

#include "../TestData/conv_1_x_n_1/test_data.h"
#include "../Utils/validate.h"
#include <nmsis_bench.h>

BENCH_DECLARE_VAR()

void conv_1_x_n_1_riscv_convolve_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[CONV_1_X_N_1_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_conv_params conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int32_t *bias_data = conv_1_x_n_1_biases;
    const int8_t *kernel_data = conv_1_x_n_1_weights;
    const int8_t *input_data = conv_1_x_n_1_input;
    const int8_t *output_ref = conv_1_x_n_1_output_ref;
    const int32_t output_ref_size = CONV_1_X_N_1_DST_SIZE;

    input_dims.n = CONV_1_X_N_1_INPUT_BATCHES;
    input_dims.w = CONV_1_X_N_1_INPUT_W;
    input_dims.h = CONV_1_X_N_1_INPUT_H;
    input_dims.c = CONV_1_X_N_1_IN_CH;
    filter_dims.w = CONV_1_X_N_1_FILTER_X;
    filter_dims.h = CONV_1_X_N_1_FILTER_Y;
    output_dims.w = CONV_1_X_N_1_OUTPUT_W;
    output_dims.h = CONV_1_X_N_1_OUTPUT_H;
    output_dims.c = CONV_1_X_N_1_OUT_CH;

    conv_params.padding.w = CONV_1_X_N_1_PAD_X;
    conv_params.padding.h = CONV_1_X_N_1_PAD_Y;
    conv_params.stride.w = CONV_1_X_N_1_STRIDE_X;
    conv_params.stride.h = CONV_1_X_N_1_STRIDE_Y;
    conv_params.dilation.w = CONV_1_X_N_1_DILATION_X;
    conv_params.dilation.h = CONV_1_X_N_1_DILATION_Y;

    conv_params.input_offset = CONV_1_X_N_1_INPUT_OFFSET;
    conv_params.output_offset = CONV_1_X_N_1_OUTPUT_OFFSET;
    conv_params.activation.min = CONV_1_X_N_1_OUT_ACTIVATION_MIN;
    conv_params.activation.max = CONV_1_X_N_1_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)conv_1_x_n_1_output_mult;
    quant_params.shift = (int32_t *)conv_1_x_n_1_output_shift;

    generate_rand_s8(conv_1_x_n_1_input, CONV_1_X_N_1_INPUT_BATCHES * CONV_1_X_N_1_INPUT_H * CONV_1_X_N_1_INPUT_W * CONV_1_X_N_1_IN_CH);
    

    int32_t buf_size = riscv_convolve_1_x_n_s8_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = 0;
    BENCH_START(riscv_convolve_1_x_n_s8);
    riscv_nmsis_nn_status result = riscv_convolve_1_x_n_s8(&ctx,
                                                       &conv_params,
                                                       &quant_params,
                                                       &input_dims,
                                                       input_data,
                                                       &filter_dims,
                                                       kernel_data,
                                                       &bias_dims,
                                                       bias_data,
                                                       &output_dims,
                                                       output);
    BENCH_END(riscv_convolve_1_x_n_s8);
    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
//    TEST_ASSERT_TRUE(validate(output, output_ref, output_ref_size));
    memset(output, 0, sizeof(output));

    buf_size = riscv_convolve_s8_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(buf_size);

    result = riscv_convolve_s8(&ctx,
                             &conv_params,
                             &quant_params,
                             &input_dims,
                             input_data,
                             &filter_dims,
                             kernel_data,
                             &bias_dims,
                             bias_data,
                             &output_dims,
                             output);
    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
//    TEST_ASSERT_TRUE(validate(output, output_ref, output_ref_size));
}