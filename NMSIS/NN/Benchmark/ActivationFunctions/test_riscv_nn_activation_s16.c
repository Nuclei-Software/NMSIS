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

#include "riscv_nnfunctions.h"


#include "TestData/activation_s16/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void activ_riscv_nn_activation_s16(void)
{
    int16_t output[ACTIVATION_SIZE] = {0};

    generate_rand_s16(s16_input, ACTIVATION_SIZE);

    BENCH_START(riscv_nn_activation_s16);
    riscv_nn_activation_s16(s16_input, output, ACTIVATION_SIZE, LEFT_SHIF, RISCV_SIGMOID);
    BENCH_END(riscv_nn_activation_s16);

}
