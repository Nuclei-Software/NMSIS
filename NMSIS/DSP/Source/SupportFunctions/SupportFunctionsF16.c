/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        SupportFunctions.c
 * Description:  Combination of all support function source files.
 *
 * $Date:        18 August 2022
 * $Revision:    V1.1.1
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2019-2020 ARM Limited or its affiliates. All rights reserved.
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

#include "riscv_copy_f16.c"
#include "riscv_fill_f16.c"
#include "riscv_f16_to_q15.c"
#include "riscv_f16_to_float.c"
#include "riscv_f16_to_f64.c"
#include "riscv_f64_to_f16.c"
#include "riscv_q15_to_f16.c"
#include "riscv_float_to_f16.c"
#include "riscv_weighted_average_f16.c"
#include "riscv_barycenter_f16.c"
