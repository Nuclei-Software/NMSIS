/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office@riscv.com>
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

#include <stdbool.h>
#include <stdlib.h>

#include <riscv_nnfunctions.h>


#include "../TestData/lstm_1/test_data.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

// Update the buffer size if adding a unit test with larger buffer.
#define LARGEST_BUFFER_SIZE LSTM_2_BUFFER_SIZE

int16_t buffer0[LARGEST_BUFFER_SIZE];
int16_t buffer1[LARGEST_BUFFER_SIZE];
int16_t buffer2[LARGEST_BUFFER_SIZE];
int16_t buffer3[LARGEST_BUFFER_SIZE];

void lstm_1_riscv_lstm_unidirectional_s16_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    const bool time_major = (bool)LSTM_1_TIME_MAJOR;

    int8_t output[LSTM_1_DST_SIZE] = {0};

    nmsis_nn_lstm_context scratch_buffers = {};
    nmsis_nn_lstm_dims lstm_dims = {};
    nmsis_nn_lstm_params lstm = {};

    scratch_buffers.input_gate = buffer0;
    scratch_buffers.forget_gate = buffer1;
    scratch_buffers.cell_gate = buffer2;
    scratch_buffers.output_gate = buffer3;

    lstm_dims.num_batches = LSTM_1_INPUT_BATCHES;
    lstm_dims.num_inputs = LSTM_1_NUMBER_INPUTS;
    lstm_dims.max_time = LSTM_1_TIME_STEPS;
    lstm_dims.num_outputs = LSTM_1_NUMBER_UNITS;

    lstm.time_major = time_major;
    lstm.input_to_input_scaling.multiplier = LSTM_1_IN_TO_INPUT_MULTIPLIER;
    lstm.input_to_input_scaling.shift = LSTM_1_IN_TO_INPUT_SHIFT;
    lstm.input_to_forget_scaling.multiplier = LSTM_1_IN_TO_FORGET_MULTIPLIER;
    lstm.input_to_forget_scaling.shift = LSTM_1_IN_TO_FORGET_SHIFT;
    lstm.input_to_cell_scaling.multiplier = LSTM_1_IN_TO_CELL_MULTIPLIER;
    lstm.input_to_cell_scaling.shift = LSTM_1_IN_TO_CELL_SHIFT;
    lstm.input_to_output_scaling.multiplier = LSTM_1_IN_TO_OUTPUT_MULTIPLIER;
    lstm.input_to_output_scaling.shift = LSTM_1_IN_TO_OUTPUT_SHIFT;

    lstm.recurrent_to_input_scaling.multiplier = LSTM_1_RECURRENT_TO_INPUT_MULTIPLIER;
    lstm.recurrent_to_input_scaling.shift = LSTM_1_RECURRENT_TO_INPUT_SHIFT;
    lstm.recurrent_to_cell_scaling.multiplier = LSTM_1_RECURRENT_TO_CELL_MULTIPLIER;
    lstm.recurrent_to_cell_scaling.shift = LSTM_1_RECURRENT_TO_CELL_SHIFT;
    lstm.recurrent_to_forget_scaling.multiplier = LSTM_1_RECURRENT_TO_FORGET_MULTIPLIER;
    lstm.recurrent_to_forget_scaling.shift = LSTM_1_RECURRENT_TO_FORGET_SHIFT;
    lstm.recurrent_to_output_scaling.multiplier = LSTM_1_RECURRENT_TO_OUTPUT_MULTIPLIER;
    lstm.recurrent_to_output_scaling.shift = LSTM_1_RECURRENT_TO_OUTPUT_SHIFT;

    lstm.i2i_effective_bias = lstm_1_input_to_input_eff_bias;
    lstm.i2f_effective_bias = lstm_1_input_to_forget_eff_bias;
    lstm.i2c_effective_bias = lstm_1_input_to_cell_eff_bias;
    lstm.i2o_effective_bias = lstm_1_input_to_output_eff_bias;

    lstm.r2i_effective_bias = lstm_1_recurrent_to_input_eff_bias;
    lstm.r2f_effective_bias = lstm_1_recurrent_to_forget_eff_bias;
    lstm.r2c_effective_bias = lstm_1_recurrent_to_cell_eff_bias;
    lstm.r2o_effective_bias = lstm_1_recurrent_to_output_eff_bias;

    lstm.input_gate_bias = lstm_1_input_gate_bias;
    lstm.forget_gate_bias = lstm_1_forget_gate_bias;
    lstm.cell_gate_bias = lstm_1_cell_gate_bias;
    lstm.output_gate_bias = lstm_1_output_gate_bias;

    lstm.activation.min = LSTM_1_IN_ACTIVATION_MIN;
    lstm.activation.max = LSTM_1_IN_ACTIVATION_MAX;

    lstm.hidden_scaling.multiplier = LSTM_1_HIDDEN_MULTIPLIER;
    lstm.hidden_scaling.shift = LSTM_1_HIDDEN_SHIFT;

    lstm.hidden_offset = LSTM_1_HIDDEN_OFFSET;

    lstm.cell_state_shift = LSTM_1_CELL_STATE_SHIFT;
    lstm.output_state_offset = LSTM_1_OUTPUT_STATE_OFFSET;

    const int32_t output_ref_size = LSTM_1_DST_SIZE;

    generate_rand_s8(lstm_1_input, LSTM_1_TIME_STEPS * LSTM_1_INPUT_BATCHES * LSTM_1_NUMBER_INPUTS);
    generate_rand_s8(lstm_1_input_to_input_w, LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_input_to_forget_w, LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_input_to_cell_w, LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_input_to_output_w, LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_recurrent_input_to_input_w, LSTM_1_NUMBER_UNITS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_recurrent_input_to_forget_w, LSTM_1_NUMBER_UNITS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_recurrent_input_to_cell_w, LSTM_1_NUMBER_UNITS * LSTM_1_NUMBER_UNITS);
    generate_rand_s8(lstm_1_recurrent_input_to_output_w, LSTM_1_NUMBER_UNITS * LSTM_1_NUMBER_UNITS);

    BENCH_START(riscv_lstm_unidirectional_s16_s8);
    riscv_nmsis_nn_status result = riscv_lstm_unidirectional_s16_s8(&scratch_buffers,
                                                                lstm_1_input,
                                                                &lstm_dims,
                                                                lstm_1_input_to_input_w,
                                                                lstm_1_input_to_forget_w,
                                                                lstm_1_input_to_cell_w,
                                                                lstm_1_input_to_output_w,
                                                                lstm_1_recurrent_input_to_input_w,
                                                                lstm_1_recurrent_input_to_forget_w,
                                                                lstm_1_recurrent_input_to_cell_w,
                                                                lstm_1_recurrent_input_to_output_w,
                                                                lstm_1_cell_to_input,
                                                                lstm_1_cell_to_forget,
                                                                lstm_1_cell_to_output,
                                                                lstm_1_projection_weights,
                                                                &lstm,
                                                                lstm_1_output_state,
                                                                lstm_1_cell_state,
                                                                output);
    BENCH_END(riscv_lstm_unidirectional_s16_s8);

    TEST_ASSERT_EQUAL(expected, result);
    //TEST_ASSERT_TRUE(validate(output, output_ref, output_ref_size));
}
