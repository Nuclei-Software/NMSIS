/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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


#include "TestData/lstm_1/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

// Update the buffer size if adding a unit test with larger buffer.
#define LARGEST_BUFFER_SIZE LSTM_1_BUFFER_SIZE

static int16_t buffer1[LARGEST_BUFFER_SIZE];
static int16_t buffer2[LARGEST_BUFFER_SIZE];
static int16_t buffer3[LARGEST_BUFFER_SIZE];

void lstm_1_riscv_lstm_unidirectional_s8(void)
{
   int8_t output[LSTM_1_DST_SIZE] = {0};
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    const int8_t *output_ref = lstm_1_output_ref;
    const int32_t output_ref_size = LSTM_1_DST_SIZE;

    // Calculate kernel sums if using MVE-extension
    int32_t input_data_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t forget_data_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t cell_data_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t output_data_kernel_sum[LSTM_1_NUMBER_UNITS];

    int32_t input_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t forget_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t cell_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t output_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];

    int32_t size_data = LSTM_1_NUMBER_INPUTS;
    int32_t size_hidden = LSTM_1_NUMBER_UNITS;

    riscv_vector_sum_s8(&input_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_input_w[0],
                      LSTM_1_DATA_OFFSET,
                      &lstm_1_input_gate_bias[0]);
    riscv_vector_sum_s8(&forget_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_forget_w[0],
                      LSTM_1_DATA_OFFSET,
                      &lstm_1_forget_gate_bias[0]);
    riscv_vector_sum_s8(&cell_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_cell_w[0],
                      LSTM_1_DATA_OFFSET,
                      &lstm_1_cell_gate_bias[0]);
    riscv_vector_sum_s8(&output_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_output_w[0],
                      LSTM_1_DATA_OFFSET,
                      &lstm_1_output_gate_bias[0]);

    riscv_vector_sum_s8(&input_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_input_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      NULL);
    riscv_vector_sum_s8(&forget_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_forget_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      NULL);
    riscv_vector_sum_s8(&cell_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_cell_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      NULL);
    riscv_vector_sum_s8(&output_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_output_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      NULL);

    // INPUT GATE
    const nmsis_nn_lstm_gate gate_input = {LSTM_1_IN_TO_INPUT_MULTIPLIER,
                                           LSTM_1_IN_TO_INPUT_SHIFT,
                                           &lstm_1_input_to_input_w[0],
                                           &input_data_kernel_sum[0],
                                           LSTM_1_RECURRENT_TO_INPUT_MULTIPLIER,
                                           LSTM_1_RECURRENT_TO_INPUT_SHIFT,
                                           &lstm_1_recurrent_input_to_input_w[0],
                                           &input_hidden_kernel_sum[0],
                                           &lstm_1_input_gate_bias[0],
                                           RISCV_SIGMOID};

    // FORGET GATE
    const nmsis_nn_lstm_gate gate_forget = {LSTM_1_IN_TO_FORGET_MULTIPLIER,
                                            LSTM_1_IN_TO_FORGET_SHIFT,
                                            &lstm_1_input_to_forget_w[0],
                                            &forget_data_kernel_sum[0],
                                            LSTM_1_RECURRENT_TO_FORGET_MULTIPLIER,
                                            LSTM_1_RECURRENT_TO_FORGET_SHIFT,
                                            &lstm_1_recurrent_input_to_forget_w[0],
                                            &forget_hidden_kernel_sum[0],
                                            &lstm_1_forget_gate_bias[0],
                                            RISCV_SIGMOID};

    // CELL GATE
    const nmsis_nn_lstm_gate gate_cell = {LSTM_1_IN_TO_CELL_MULTIPLIER,
                                          LSTM_1_IN_TO_CELL_SHIFT,
                                          &lstm_1_input_to_cell_w[0],
                                          &cell_data_kernel_sum[0],
                                          LSTM_1_RECURRENT_TO_CELL_MULTIPLIER,
                                          LSTM_1_RECURRENT_TO_CELL_SHIFT,
                                          &lstm_1_recurrent_input_to_cell_w[0],
                                          &cell_hidden_kernel_sum[0],
                                          &lstm_1_cell_gate_bias[0],
                                          RISCV_TANH};

    // OUTPUT GATE
    const nmsis_nn_lstm_gate gate_output = {LSTM_1_IN_TO_OUTPUT_MULTIPLIER,
                                            LSTM_1_IN_TO_OUTPUT_SHIFT,
                                            &lstm_1_input_to_output_w[0],
                                            &output_data_kernel_sum[0],
                                            LSTM_1_RECURRENT_TO_OUTPUT_MULTIPLIER,
                                            LSTM_1_RECURRENT_TO_OUTPUT_SHIFT,
                                            &lstm_1_recurrent_input_to_output_w[0],
                                            &output_hidden_kernel_sum[0],
                                            &lstm_1_output_gate_bias[0],
                                            RISCV_SIGMOID};

    // LSTM DATA
    const nmsis_nn_lstm_params params = {LSTM_1_TIME_MAJOR,
                                         LSTM_1_INPUT_BATCHES,
                                         LSTM_1_TIME_STEPS,
                                         LSTM_1_NUMBER_INPUTS,
                                         LSTM_1_NUMBER_UNITS,
                                         LSTM_1_DATA_OFFSET,
                                         LSTM_1_FORGET_MULTIPLIER,
                                         LSTM_1_FORGET_SHIFT,
                                         LSTM_1_INPUT_MULTIPLIER,
                                         LSTM_1_INPUT_SHIFT,
                                         LSTM_1_IN_ACTIVATION_MAX,
                                         LSTM_1_CELL_STATE_SHIFT,
                                         LSTM_1_HIDDEN_MULTIPLIER,
                                         LSTM_1_HIDDEN_SHIFT,
                                         LSTM_1_HIDDEN_OFFSET,
                                         gate_forget,
                                         gate_input,
                                         gate_cell,
                                         gate_output};

    // BUFFERS
    nmsis_nn_lstm_context buffers;
    buffers.temp1 = buffer1;
    buffers.temp2 = buffer2;
    buffers.cell_state = buffer3;
    BENCH_START(riscv_lstm_unidirectional_s8);
    riscv_nmsis_nn_status result = riscv_lstm_unidirectional_s8(lstm_1_input, output, &params, &buffers);
    BENCH_END(riscv_lstm_unidirectional_s8);
    TEST_ASSERT_EQUAL(expected, result);
//    TEST_ASSERT_TRUE(validate(output, output_ref, output_ref_size));
}
