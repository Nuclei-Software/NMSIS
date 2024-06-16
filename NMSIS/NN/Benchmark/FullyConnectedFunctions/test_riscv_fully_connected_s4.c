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

#include "../TestData/fully_connected_int4/test_data.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void fully_connected_int4_riscv_fully_connected_s4(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output[FULLY_CONNECTED_INT4_DST_SIZE] = {0};

    nmsis_nn_context ctx;
    nmsis_nn_fc_params fc_params;
    nmsis_nn_per_tensor_quant_params quant_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims filter_dims;
    nmsis_nn_dims bias_dims = {};
    nmsis_nn_dims output_dims;

    const int8_t *kernel_data = fully_connected_int4_weights;
    const int32_t *bias_data = fully_connected_int4_biases;
    const int8_t *input_data = fully_connected_int4_input;
    const int8_t *output_ref = fully_connected_int4_output_ref;
    const int32_t output_ref_size = FULLY_CONNECTED_INT4_DST_SIZE;

    input_dims.n = FULLY_CONNECTED_INT4_INPUT_BATCHES;
    input_dims.w = FULLY_CONNECTED_INT4_INPUT_W;
    input_dims.h = FULLY_CONNECTED_INT4_INPUT_H;
    input_dims.c = FULLY_CONNECTED_INT4_IN_CH;
    filter_dims.n = FULLY_CONNECTED_INT4_ACCUMULATION_DEPTH;
    filter_dims.c = FULLY_CONNECTED_INT4_OUT_CH;
    output_dims.n = FULLY_CONNECTED_INT4_INPUT_BATCHES;
    output_dims.c = FULLY_CONNECTED_INT4_OUT_CH;

    fc_params.input_offset = FULLY_CONNECTED_INT4_INPUT_OFFSET;
    fc_params.filter_offset = 0;
    fc_params.output_offset = FULLY_CONNECTED_INT4_OUTPUT_OFFSET;
    fc_params.activation.min = FULLY_CONNECTED_INT4_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FULLY_CONNECTED_INT4_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FULLY_CONNECTED_INT4_OUTPUT_MULTIPLIER;
    quant_params.shift = FULLY_CONNECTED_INT4_OUTPUT_SHIFT;

    generate_rand_s8(fully_connected_int4_input, FULLY_CONNECTED_INT4_INPUT_SIZE);
    generate_rand_s8(fully_connected_int4_weights, FULLY_CONNECTED_INT4_KERNEL_SIZE);

    int32_t buf_size = 0;
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    BENCH_START(riscv_fully_connected_s4);
    riscv_nmsis_nn_status result = riscv_fully_connected_s4(&ctx,
                                                        &fc_params,
                                                        &quant_params,
                                                        &input_dims,
                                                        input_data,
                                                        &filter_dims,
                                                        kernel_data,
                                                        &bias_dims,
                                                        bias_data,
                                                        &output_dims,
                                                        output);
    BENCH_END(riscv_fully_connected_s4);

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, output_ref, output_ref_size));
}
