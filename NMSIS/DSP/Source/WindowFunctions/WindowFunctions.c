/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        WindowFunctions.c
 * Description:  Windowing functions for spectral estimations
 *
 *
 * Target Processor: RISC-V and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#include "riscv_welch_f32.c"
#include "riscv_welch_f64.c"
#include "riscv_bartlett_f32.c"
#include "riscv_bartlett_f64.c"
#include "riscv_hamming_f32.c"
#include "riscv_hamming_f64.c"
#include "riscv_hanning_f32.c"
#include "riscv_hanning_f64.c"
#include "riscv_nuttall3_f32.c"
#include "riscv_nuttall3_f64.c"
#include "riscv_nuttall4_f32.c"
#include "riscv_nuttall4_f64.c"
#include "riscv_nuttall3a_f32.c"
#include "riscv_nuttall3a_f64.c"
#include "riscv_nuttall3b_f32.c"
#include "riscv_nuttall3b_f64.c"
#include "riscv_nuttall4a_f32.c"
#include "riscv_nuttall4a_f64.c"
#include "riscv_blackman_harris_92db_f32.c"
#include "riscv_blackman_harris_92db_f64.c"
#include "riscv_nuttall4b_f32.c"
#include "riscv_nuttall4b_f64.c"
#include "riscv_nuttall4c_f32.c"
#include "riscv_nuttall4c_f64.c"
#include "riscv_hft90d_f32.c"
#include "riscv_hft90d_f64.c"
#include "riscv_hft95_f32.c"
#include "riscv_hft95_f64.c"
#include "riscv_hft116d_f32.c"
#include "riscv_hft116d_f64.c"
#include "riscv_hft144d_f32.c"
#include "riscv_hft144d_f64.c"
#include "riscv_hft169d_f32.c"
#include "riscv_hft169d_f64.c"
#include "riscv_hft196d_f32.c"
#include "riscv_hft196d_f64.c"
#include "riscv_hft223d_f32.c"
#include "riscv_hft223d_f64.c"
#include "riscv_hft248d_f32.c"
#include "riscv_hft248d_f64.c"

