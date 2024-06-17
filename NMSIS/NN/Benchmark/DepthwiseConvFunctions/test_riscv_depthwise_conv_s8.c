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


#include "../TestData/depthwise_2/test_data.h"
#include "../Utils/utils.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void depthwise_2_riscv_depthwise_conv_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[DEPTHWISE_2_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_dw_conv_params dw_conv_params;
    nmsis_nn_per_channel_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims = {};
    nmsis_nn_dims output_dims;

    const int32_t output_ref_size = DEPTHWISE_2_DST_SIZE;
    const int32_t *bias_data = get_bias_address(depthwise_2_biases, DEPTHWISE_2_OUT_CH);
    const int8_t *kernel_data = depthwise_2_weights;
    const int8_t *input_data = depthwise_2_input;

    input_dims.n = DEPTHWISE_2_INPUT_BATCHES;
    input_dims.w = DEPTHWISE_2_INPUT_W;
    input_dims.h = DEPTHWISE_2_INPUT_H;
    input_dims.c = DEPTHWISE_2_IN_CH;
    filter_dims.w = DEPTHWISE_2_FILTER_X;
    filter_dims.h = DEPTHWISE_2_FILTER_Y;
    output_dims.w = DEPTHWISE_2_OUTPUT_W;
    output_dims.h = DEPTHWISE_2_OUTPUT_H;
    output_dims.c = DEPTHWISE_2_OUT_CH;

    dw_conv_params.padding.w = DEPTHWISE_2_PAD_X;
    dw_conv_params.padding.h = DEPTHWISE_2_PAD_Y;
    dw_conv_params.stride.w = DEPTHWISE_2_STRIDE_X;
    dw_conv_params.stride.h = DEPTHWISE_2_STRIDE_Y;
    dw_conv_params.dilation.w = DEPTHWISE_2_DILATION_X;
    dw_conv_params.dilation.h = DEPTHWISE_2_DILATION_Y;

    dw_conv_params.ch_mult = DEPTHWISE_2_CH_MULT;

    dw_conv_params.input_offset = DEPTHWISE_2_INPUT_OFFSET;
    dw_conv_params.output_offset = DEPTHWISE_2_OUTPUT_OFFSET;
    dw_conv_params.activation.min = DEPTHWISE_2_OUT_ACTIVATION_MIN;
    dw_conv_params.activation.max = DEPTHWISE_2_OUT_ACTIVATION_MAX;
    quant_params.multiplier = (int32_t *)depthwise_2_output_mult;
    quant_params.shift = (int32_t *)depthwise_2_output_shift;

    generate_rand_s8(depthwise_2_input, DEPTHWISE_2_INPUT_SIZE);
    generate_rand_s8(depthwise_2_weights, DEPTHWISE_2_KERNEL_SIZE);

    ctx.buf = NULL;
    ctx.size = 0;

    BENCH_START(riscv_depthwise_conv_s8);
    riscv_nmsis_nn_status result = riscv_depthwise_conv_s8(&ctx,
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
    BENCH_END(riscv_depthwise_conv_s8);

    if (ctx.buf)
    {
        memset(ctx.buf, 0, ctx.size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, depthwise_2_output_ref, output_ref_size));
    memset(output, 0, sizeof(output));

    const int32_t buf_size =
        riscv_depthwise_conv_wrapper_s8_get_buffer_size(&dw_conv_params, &input_dims, &filter_dims, &output_dims);
    TEST_ASSERT_EQUAL(buf_size, 0);

    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    result = riscv_depthwise_conv_wrapper_s8(&ctx,
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
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, depthwise_2_output_ref, output_ref_size));
}
