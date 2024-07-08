/*
 * Copyright (C) 2022 Arm Limited or its affiliates.
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

#include "riscv_nnsupportfunctions.h"

#include "../TestData/mat_mult_nt_t_s8/test_data.h"
#include "../Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void matMult_riscv_nn_mat_mult_nt_t_s8(void)
{
    const riscv_nmsis_nn_status expected = RISCV_NMSIS_NN_SUCCESS;
    int8_t output_s8[DST_SIZE];
    generate_rand_s8(s8_input1, LHS_COLS_OFFSET * LHS_ROWS * RHS_COLS);
    generate_rand_s8(s8_input2, 2 * RHS_COLS);
    
    for (int i = 0; i < RHS_ROWS; i ++)
    {
	bias[i] = (int32_t)(rand() % 65536 - 32768);
    }

    BENCH_START(riscv_nn_mat_mult_nt_t_s8);
    riscv_nmsis_nn_status result = riscv_nn_mat_mult_nt_t_s8(s8_input1, s8_input2, bias, output_s8, output_mult, output_shift,
                              LHS_ROWS, RHS_ROWS, RHS_COLS, LHS_OFFSET, DST_OFFSET, ACTIVATION_MIN, ACTIVATION_MAX, LHS_COLS_OFFSET);
    BENCH_END(riscv_nn_mat_mult_nt_t_s8);

    TEST_ASSERT_EQUAL(expected, result);
 //   TEST_ASSERT_TRUE(validate(output, add_output_ref, ADD_DST_SIZE));
}
