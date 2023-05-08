/*
 * SPDX-FileCopyrightText: Copyright 2010-2020, 2022 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_depthwise_conv_nt_t_padded_s8.c
 * Description:  Depthwise convolution with padded matrices.
 *
 * $Date:        26 October 2022
 * $Revision:    V.2.0.1
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @defgroup supportConvolution Convolution
 *
 * Support functions for Convolution and DW Convolution
 *
 */

/**
 * @addtogroup supportConvolution
 * @{
 */

/*
 * Depthwise convolution of transposed rhs matrix with 4 lhs matrices. One or more of the rhs matrices are padded.
 * Dimensions are the same for lhs and rhs.
 *
 * Refer header file for details.
 *
 */

riscv_nmsis_nn_status riscv_nn_depthwise_conv_nt_t_padded_s8(const int8_t *lhs,
                                                         const int8_t *rhs,
                                                         const int32_t input_offset,
                                                         const int32_t active_ch,
                                                         const int32_t total_ch,
                                                         const int32_t *out_shift,
                                                         const int32_t *out_mult,
                                                         const int32_t out_offset,
                                                         const int32_t activation_min,
                                                         const int32_t activation_max,
                                                         const uint16_t row_x_col,
                                                         const int32_t *const output_bias,
                                                         int8_t *out)
{
    (void)lhs;
    (void)rhs;
    (void)input_offset;
    (void)active_ch;
    (void)total_ch;
    (void)out_shift;
    (void)out_mult;
    (void)out_offset;
    (void)activation_min;
    (void)activation_max;
    (void)row_x_col;
    (void)output_bias;
    (void)out;
    return RISCV_NMSIS_NN_NO_IMPL_ERROR;
}

/**
 * @} end of Doxygen group
 */
