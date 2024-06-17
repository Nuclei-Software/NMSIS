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
#include <stdlib.h>

#include "../TestData/depthwise_int4_generic/test_data.h"
#include "../Utils/utils.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void depthwise_int4_generic_riscv_depthwise_conv_s4(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[DEPTHWISE_INT4_GENERIC_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_dw_conv_params dw_conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_dims output_dims;

    const int32_t *bias_data = depthwise_int4_generic_biases;
    const int8_t *kernel_data = depthwise_int4_generic_weights;
    const int8_t *input_data = depthwise_int4_generic_input;

    input_dims.n = DEPTHWISE_INT4_GENERIC_INPUT_BATCHES;
    input_dims.w = DEPTHWISE_INT4_GENERIC_INPUT_W;
    input_dims.h = DEPTHWISE_INT4_GENERIC_INPUT_H;
    input_dims.c = DEPTHWISE_INT4_GENERIC_IN_CH;
    filter_dims.w = DEPTHWISE_INT4_GENERIC_FILTER_X;
    filter_dims.h = DEPTHWISE_INT4_GENERIC_FILTER_Y;
    output_dims.w = DEPTHWISE_INT4_GENERIC_OUTPUT_W;
    output_dims.h = DEPTHWISE_INT4_GENERIC_OUTPUT_H;
    output_dims.c = DEPTHWISE_INT4_GENERIC_OUT_CH;

    dw_conv_params.padding.w = DEPTHWISE_INT4_GENERIC_PAD_X;
    dw_conv_params.padding.h = DEPTHWISE_INT4_GENERIC_PAD_Y;
    dw_conv_params.stride.w = DEPTHWISE_INT4_GENERIC_STRIDE_X;
    dw_conv_params.stride.h = DEPTHWISE_INT4_GENERIC_STRIDE_Y;
    dw_conv_params.dilation.w = DEPTHWISE_INT4_GENERIC_DILATION_X;
    dw_conv_params.dilation.h = DEPTHWISE_INT4_GENERIC_DILATION_Y;

    dw_conv_params.ch_mult = DEPTHWISE_INT4_GENERIC_CH_MULT;

    dw_conv_params.input_offset = DEPTHWISE_INT4_GENERIC_INPUT_OFFSET;
    dw_conv_params.output_offset = DEPTHWISE_INT4_GENERIC_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DEPTHWISE_INT4_GENERIC_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DEPTHWISE_INT4_GENERIC_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)depthwise_int4_generic_output_mult;
    quant_params.shift = (int32_t *)depthwise_int4_generic_output_shift;

    generate_rand_s8(depthwise_int4_generic_input, DEPTHWISE_INT4_GENERIC_INPUT_SIZE);
    generate_rand_s8(depthwise_int4_generic_weights, DEPTHWISE_INT4_GENERIC_KERNEL_SIZE);

    ctx.size = riscv_depthwise_conv_s4_opt_get_buffer_size(&input_dims, &filter_dims);
    TEST_ASSERT_TRUE(ctx.size > 0);

    ctx.buf = malloc(ctx.size);

    BENCH_START(riscv_depthwise_conv_s4);
    riscv_nmsis_nn_status result = riscv_depthwise_conv_s4(&ctx,
                                                       &dw_conv_params,
                                                       &quant_params,
                                                       &input_dims,
                                                       input_data,
                                                       &filter_dims,
                                                       kernel_data,
                                                       &bias_dims,
                                                       bias_data,
                                                       &output_dims,
                                                       output);
    BENCH_END(riscv_depthwise_conv_s4);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, depthwise_int4_generic_output_ref, DEPTHWISE_INT4_GENERIC_DST_SIZE));
    memset(output, 0, DEPTHWISE_INT4_GENERIC_DST_SIZE);

    ctx.buf = malloc(ctx.size);
    result = riscv_depthwise_conv_wrapper_s4(&ctx,
                                           &dw_conv_params,
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
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, depthwise_int4_generic_output_ref, DEPTHWISE_INT4_GENERIC_DST_SIZE));
    memset(output, 0, DEPTHWISE_INT4_GENERIC_DST_SIZE);
}
