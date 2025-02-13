/*
 * SPDX-FileCopyrightText: Copyright 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_depthwise_conv_nt_t_s16.c
 * Description:  Depthwise convolution on matrices with no padding.
 *
 * $Date:        26 October 2022
 * $Revision:    V.1.0.1
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConvolution
 * @{
 */

/*
 * Depthwise convolution of rhs matrix with 4 lhs matrices with no padding. Dimensions are the same for lhs and rhs.
 *
 * Refer header file for details.
 *
 */
int16_t *riscv_nn_depthwise_conv_nt_t_s16(const int16_t *lhs,
                                        const int8_t *rhs,
                                        const uint16_t num_ch,
                                        const int32_t *out_shift,
                                        const int32_t *out_mult,
                                        const int32_t activation_min,
                                        const int32_t activation_max,
                                        const uint16_t row_x_col,
                                        const int64_t *const output_bias,
                                        int16_t *out)
{
    (void)lhs;
    (void)rhs;
    (void)num_ch;
    (void)out_shift;
    (void)out_mult;
    (void)activation_min;
    (void)activation_max;
    (void)row_x_col;
    (void)output_bias;
    (void)out;
    return NULL;
}

/**
 * @} end of Doxygen group
 */
