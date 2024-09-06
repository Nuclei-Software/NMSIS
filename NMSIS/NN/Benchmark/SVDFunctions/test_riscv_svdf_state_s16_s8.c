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

#include "TestData/svdf/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

static bool check_null_bias(const int32_t *bias, int32_t size)
{
    bool null_bias = true;
    for (int i = 0; i < size; i++)
    {
        if (bias[i] != 0)
        {
            null_bias = false;
            break;
        }
    }
    return null_bias;
}

void svdf_riscv_svdf_state_s16_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    nmsis_nn_context input_ctx;
    nmsis_nn_context output_ctx;
    nmsis_nn_svdf_params svdf_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims weights_feature_dims;
    nmsis_nn_dims weights_time_dims;
    nmsis_nn_dims state_dims;
    nmsis_nn_dims output_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_per_tensor_quant_params input_quant_params;
    nmsis_nn_per_tensor_quant_params output_quant_params;
    int8_t output_data[SVDF_DST_SIZE];
    const int8_t *weights_feature_data = svdf_weights_feature;
    const int16_t *weights_time_data = svdf_weights_time;

    input_dims.n = SVDF_INPUT_BATCHES;
    input_dims.h = SVDF_INPUT_SIZE;
    weights_feature_dims.n = SVDF_FEATURE_BATCHES;
    weights_time_dims.h = SVDF_TIME_BATCHES;

    input_quant_params.multiplier = SVDF_MULTIPLIER_IN;
    input_quant_params.shift = SVDF_SHIFT_1;
    output_quant_params.multiplier = SVDF_MULTIPLIER_OUT;
    output_quant_params.shift = SVDF_SHIFT_2;

    svdf_params.input_activation.min = SVDF_IN_ACTIVATION_MIN;
    svdf_params.input_activation.max = SVDF_IN_ACTIVATION_MAX;
    svdf_params.output_activation.min = SVDF_OUT_ACTIVATION_MIN;
    svdf_params.output_activation.max = SVDF_OUT_ACTIVATION_MAX;
    svdf_params.input_offset = SVDF_INPUT_OFFSET;
    svdf_params.output_offset = SVDF_OUTPUT_OFFSET;
    svdf_params.rank = SVDF_RANK;

    const int32_t output_ref_size = SVDF_DST_SIZE;
    const int8_t *output_ref = svdf_output_ref;
    const int input_round_size = SVDF_INPUT_BATCHES * SVDF_INPUT_SIZE;
    // const int number_inputs = sizeof(svdf_input_sequence) / input_round_size;
    const int32_t number_units = SVDF_FEATURE_BATCHES / SVDF_RANK;
    const int scratch_size = SVDF_INPUT_BATCHES * SVDF_FEATURE_BATCHES * sizeof(int32_t);
    const int scratch_size_out = SVDF_INPUT_BATCHES * number_units * sizeof(int32_t);

    input_ctx.buf = malloc(scratch_size);
    output_ctx.buf = malloc(scratch_size_out);

    int8_t *input_data = malloc(input_round_size);
    int16_t *state_data = malloc(sizeof(svdf_state));

    const bool null_bias = check_null_bias(svdf_biases, SVDF_DST_SIZE / SVDF_INPUT_BATCHES);

    generate_rand_s8(svdf_input_sequence, SVDF_INPUT_BATCHES * SVDF_INPUT_SIZE);
    generate_rand_s8(svdf_weights_feature, SVDF_INPUT_SIZE * SVDF_FEATURE_BATCHES);
    generate_rand_s16(svdf_weights_time, SVDF_FEATURE_BATCHES * SVDF_TIME_BATCHES);

    memcpy(state_data, svdf_state, sizeof(svdf_state));
    BENCH_START(riscv_svdf_state_s16_s8);
    riscv_nmsis_nn_status result = riscv_svdf_state_s16_s8(&input_ctx,
                                                       &output_ctx,
                                                       &svdf_params,
                                                       &input_quant_params,
                                                       &output_quant_params,
                                                       &input_dims,
                                                       svdf_input_sequence,
                                                       &state_dims,
                                                       state_data,
                                                       &weights_feature_dims,
                                                       weights_feature_data,
                                                       &weights_time_dims,
                                                       weights_time_data,
                                                       &bias_dims,
                                                       null_bias == true ? NULL : svdf_biases,
                                                       &output_dims,
                                                       output_data);
    BENCH_END(riscv_svdf_state_s16_s8);
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output_data, output_ref, output_ref_size));
    free(state_data);
    free(input_ctx.buf);
    free(output_ctx.buf);
}
