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


#include "TestData/relu6_s8/test_data.h"
#include "Utils/validate.h"
#include "nmsis_bench.h"

BENCH_DECLARE_VAR();

void relu6_riscv_relu6_s8(void)
{
    generate_rand_s8(relu6_s8_input, RELU6_INPUT_SIZE);

    BENCH_START(riscv_relu6_s8);
    riscv_relu6_s8(relu6_s8_input, RELU6_INPUT_SIZE);
    BENCH_END(riscv_relu6_s8);

}
