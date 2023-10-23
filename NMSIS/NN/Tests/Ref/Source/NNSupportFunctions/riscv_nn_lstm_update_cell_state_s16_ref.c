/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office.com>
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
 * Title:        riscv_nn_lstm_update_cell_state_s16.c
 * Description:  Update cell state for an incremental step of LSTM function.
 *
 * $Date:        20 January 2023
 * $Revision:    V.1.2.0
 *
 * Target :   RISC-V Cores
 *
 * -------------------------------------------------------------------- */
#include "ref_functions.h"
#include "riscv_nnsupportfunctions.h"
/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportLSTM
 * @{
 */

/*
 * Update cell state for a single LSTM iteration step, int8x8_16 version.
 *
 * Refer to header file for more details
 */
void riscv_nn_lstm_update_cell_state_s16_ref(const int32_t n_block,
                                       const int32_t cell_state_scale,
                                       int16_t *cell_state,
                                       const int16_t *input_gate,
                                       const int16_t *forget_gate,
                                       const int16_t *cell_gate)
{
    const int32_t cell_scale = 30 + cell_state_scale;
    int32_t loop_count = n_block;

    for (int i = 0; i < loop_count; i++)
    {
        int32_t value = cell_state[i] * forget_gate[i];
        int32_t value_1 = input_gate[i] * cell_gate[i];

        value = riscv_nn_divide_by_power_of_two(value, 15);
        value_1 = riscv_nn_divide_by_power_of_two(value_1, cell_scale);

        cell_state[i] = CLAMP(value + value_1, NN_Q15_MAX, NN_Q15_MIN);
    }
}
/**
 * @} end of supportLSTM group
 */
