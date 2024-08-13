/*
 * SPDX-FileCopyrightText: Copyright 2022, 2024 Arm Limited and/or its affiliates <open-source-office.com>
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_nn_lstm_calculate_gate_s8_s16.c
 * Description:  Update single gate for an incremental step of LSTM function.
 *
 * $Date:        19 January 2024
 * $Revision:    V.2.0.0
 *
 * Target Processor:  RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nn_tables.h"
#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"
/**
 * @ingroup groupSupport
 */

/**
 * @defgroup supportLSTM LSTM
 *
 * Support functions for LSTM
 *
 */

/**
 * @addtogroup supportLSTM
 * @{
 */

/*
 * Calculates a single LSTM gate, int8x8_16 version.
 * Refer to header file for details
 */
riscv_nmsis_nn_status riscv_nn_lstm_calculate_gate_s8_s16(const int8_t *data_in,
                                                      const int8_t *hidden_in,
                                                      const nmsis_nn_lstm_gate *gate,
                                                      const nmsis_nn_lstm_params *params,
                                                      int16_t *output,
                                                      const int32_t batch_offset)
{

    memset(output, 0, params->hidden_size * params->batch_size * sizeof(int16_t));

    riscv_nn_vec_mat_mul_result_acc_s8_s16(data_in,
                                         gate->input_weights,
                                         gate->input_effective_bias,
                                         output,
                                         gate->input_multiplier,
                                         gate->input_shift,
                                         params->input_size,
                                         params->hidden_size,
                                         params->batch_size,
                                         batch_offset);

    if (hidden_in)
    {
        riscv_nn_vec_mat_mul_result_acc_s8_s16(hidden_in,
                                             gate->hidden_weights,
                                             gate->hidden_effective_bias,
                                             output,
                                             gate->hidden_multiplier,
                                             gate->hidden_shift,
                                             params->hidden_size,
                                             params->hidden_size,
                                             params->batch_size,
                                             batch_offset);
    }

    riscv_nn_activation_s16(output, output, params->hidden_size * params->batch_size, 0, gate->activation_type);

    return RISCV_NMSIS_NN_SUCCESS;
}
/**
 * @} end of supportLSTM group
 */