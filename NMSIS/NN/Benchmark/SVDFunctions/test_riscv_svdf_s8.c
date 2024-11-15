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

#include "TestData/svdf_int8/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void svdf_int8_riscv_svdf_s8(void)
{
    const int32_t output_ref_size = SVDF_INT8_DST_SIZE;
    const int8_t *output_ref = svdf_int8_output_ref;
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    nmsis_nn_context input_ctx;
    nmsis_nn_context output_ctx;
    nmsis_nn_svdf_params svdf_int8_params;
    nmsis_nn_dims input_dims;
    nmsis_nn_dims weights_feature_dims;
    nmsis_nn_dims weights_time_dims;
    nmsis_nn_dims state_dims;
    nmsis_nn_dims output_dims;
    nmsis_nn_dims bias_dims;
    nmsis_nn_per_tensor_quant_params input_quant_params;
    nmsis_nn_per_tensor_quant_params output_quant_params;
    int8_t output_data[SVDF_INT8_DST_SIZE] = {1};
    const int8_t *weights_feature_data = svdf_int8_weights_feature;
    const int8_t *weights_time_data = svdf_int8_weights_time;

    input_dims.n = SVDF_INT8_INPUT_BATCHES;
    input_dims.h = SVDF_INT8_INPUT_SIZE;
    weights_feature_dims.n = SVDF_INT8_FEATURE_BATCHES;
    weights_time_dims.h = SVDF_INT8_TIME_BATCHES;

    input_quant_params.multiplier = SVDF_INT8_MULTIPLIER_IN;
    input_quant_params.shift = SVDF_INT8_SHIFT_1;
    output_quant_params.multiplier = SVDF_INT8_MULTIPLIER_OUT;
    output_quant_params.shift = SVDF_INT8_SHIFT_2;

    svdf_int8_params.input_activation.min = SVDF_INT8_IN_ACTIVATION_MIN;
    svdf_int8_params.input_activation.max = SVDF_INT8_IN_ACTIVATION_MAX;
    svdf_int8_params.output_activation.min = SVDF_INT8_OUT_ACTIVATION_MIN;
    svdf_int8_params.output_activation.max = SVDF_INT8_OUT_ACTIVATION_MAX;
    svdf_int8_params.input_offset = SVDF_INT8_INPUT_OFFSET;
    svdf_int8_params.output_offset = SVDF_INT8_OUTPUT_OFFSET;
    svdf_int8_params.rank = SVDF_INT8_RANK;

    const int input_round_size = SVDF_INT8_INPUT_BATCHES * SVDF_INT8_INPUT_SIZE;
    //const int number_inputs = sizeof(svdf_int8_input_sequence) / input_round_size;
    const int32_t number_units = SVDF_INT8_FEATURE_BATCHES / SVDF_INT8_RANK;
    const int scratch_size = SVDF_INT8_INPUT_BATCHES * SVDF_INT8_FEATURE_BATCHES * sizeof(int32_t);
    const int scratch_size_out = SVDF_INT8_INPUT_BATCHES * number_units * sizeof(int32_t);

    nmsis_nn_context ctx;
    const int32_t buf_size = weights_feature_dims.n * sizeof(int32_t);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    // + SVDF_INT8_TIME_BATCHES additional bytes to make sure it is not overwritten
    const int state_data_size = sizeof(svdf_int8_state) + SVDF_INT8_TIME_BATCHES;
    const int8_t initial_data = 66;

    input_ctx.buf = malloc(scratch_size);
    output_ctx.buf = malloc(scratch_size_out);

    int8_t *input_data = malloc(input_round_size);
    int8_t *state_data = malloc(state_data_size);

    memset(state_data, initial_data, state_data_size);
    memcpy(state_data, svdf_int8_state, sizeof(svdf_int8_state));

    generate_rand_s8(svdf_int8_input_sequence, SVDF_INT8_INPUT_BATCHES * SVDF_INT8_INPUT_SIZE);
    generate_rand_s8(svdf_int8_weights_feature, SVDF_INT8_INPUT_SIZE * SVDF_INT8_FEATURE_BATCHES);
    generate_rand_s8(svdf_int8_weights_time, SVDF_INT8_FEATURE_BATCHES * SVDF_INT8_TIME_BATCHES);

    BENCH_START(riscv_svdf_s8);
    riscv_nmsis_nn_status result = riscv_svdf_s8(&ctx,
                                             &input_ctx,
                                             &output_ctx,
                                             &svdf_int8_params,
                                             &input_quant_params,
                                             &output_quant_params,
                                             &input_dims,
                                             svdf_int8_input_sequence,
                                             &state_dims,
                                             state_data,
                                             &weights_feature_dims,
                                             weights_feature_data, 
                                             &weights_time_dims,
                                             weights_time_data,
                                             &bias_dims,
                                             svdf_int8_biases,
                                             &output_dims,
                                             output_data);
    BENCH_END(riscv_svdf_s8);
    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output_data, output_ref, output_ref_size));

    if (ctx.buf)
    {
        // The caller is responsible to clear the scratch buffers for security reasons if applicable.
        memset(ctx.buf, 0, buf_size);
        free(ctx.buf);
    }

    // Make sure state data is not written outside boundary
    for (int i = sizeof(svdf_int8_state); i < state_data_size; i++)
    {
        TEST_ASSERT_EQUAL(state_data[i], initial_data);
    }

    free(state_data);
    free(input_ctx.buf);
    free(output_ctx.buf);
}
