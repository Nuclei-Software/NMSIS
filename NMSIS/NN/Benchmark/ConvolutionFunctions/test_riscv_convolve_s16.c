/*
 * SPDX-FileCopyrightText: Copyright 2010-2023 Arm Limited and/or its affiliates <open-source-office@riscv.com> All rights
 * reserved.
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

#include "TestData/int16xint8/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void int16xint8_riscv_convolve_s16(void)
{
    int16_t output[INT16XINT8_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_conv_params conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const nmsis_nn_bias_data bias_data = {int16xint8_biases, false};
    const int8_t *kernel_data = int16xint8_weights;
    const int16_t *input_data = int16xint8_input;
    const int16_t *output_ref = int16xint8_output_ref;
    const int32_t output_ref_size = INT16XINT8_DST_SIZE;

    input_dims.n = INT16XINT8_INPUT_BATCHES;
    input_dims.w = INT16XINT8_INPUT_W;
    input_dims.h = INT16XINT8_INPUT_H;
    input_dims.c = INT16XINT8_IN_CH;
    filter_dims.w = INT16XINT8_FILTER_X;
    filter_dims.h = INT16XINT8_FILTER_Y;
    filter_dims.c = INT16XINT8_IN_CH;
    output_dims.w = INT16XINT8_OUTPUT_W;
    output_dims.h = INT16XINT8_OUTPUT_H;
    output_dims.c = INT16XINT8_OUT_CH;

    conv_params.padding.w = INT16XINT8_PAD_X;
    conv_params.padding.h = INT16XINT8_PAD_Y;
    conv_params.stride.w = INT16XINT8_STRIDE_X;
    conv_params.stride.h = INT16XINT8_STRIDE_Y;
    conv_params.dilation.w = INT16XINT8_DILATION_X;
    conv_params.dilation.h = INT16XINT8_DILATION_Y;

    conv_params.input_offset = 0;
    conv_params.output_offset = 0;
    conv_params.activation.min = INT16XINT8_OUT_ACTIVATION_MIN;
    conv_params.activation.max = INT16XINT8_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)int16xint8_output_mult;
    quant_params.shift = (int32_t *)int16xint8_output_shift;

    generate_rand_s16(int16xint8_input, INT16XINT8_INPUT_SIZE);
    generate_rand_s8(int16xint8_weights, INT16XINT8_KERNEL_SIZE);

    int buf_size = riscv_convolve_s16_get_buffer_size(&input_dims, &filter_dims);
    ctx.buf = malloc(buf_size);

    BENCH_START(riscv_convolve_s16);
    riscv_nmsis_nn_status result = riscv_convolve_s16(&ctx,
                                                  &conv_params,
                                                  &quant_params,
                                                  &input_dims,
                                                  input_data,
                                                  &filter_dims,
                                                  kernel_data,
                                                  &bias_dims,
                                                  &bias_data,
                                                  &output_dims,
                                                  output);
    BENCH_END(riscv_convolve_s16);
    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
//    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
    memset(output, 0, sizeof(output));

    buf_size = riscv_convolve_wrapper_s16_get_buffer_size(&conv_params, &input_dims, &filter_dims, &output_dims);
    ctx.buf = malloc(buf_size);

    result = riscv_convolve_wrapper_s16(&ctx,
                                      &conv_params,
                                      &quant_params,
                                      &input_dims,
                                      input_data,
                                      &filter_dims,
                                      kernel_data,
                                      &bias_dims,
                                      &bias_data,
                                      &output_dims,
                                      output);

    if (ctx.buf)
    {
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(RISCV_NMSIS_NN_SUCCESS, result);
//    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
}
