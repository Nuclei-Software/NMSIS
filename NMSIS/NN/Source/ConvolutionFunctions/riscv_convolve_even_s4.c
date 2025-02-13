/*
 * SPDX-FileCopyrightText: Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2024 Nuclei Limited. All rights reserved.
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
 * Title:        riscv_convolve_even_s4.c
 * Description:  s8 version of convolution using symmetric quantization with 4 bit weights.
 *
 * $Date:        05 Jun 2024
 * $Revision:    V.1.0.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup Public
 */

/**
 * @addtogroup NNConv
 * @{
 */

/*
 * Basic s8 convolution function with int4 packed RHS (weights) and even RHS columns,
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_convolve_even_s4(const nmsis_nn_context *ctx,
                                         const nmsis_nn_conv_params *conv_params,
                                         const nmsis_nn_per_channel_quant_params *quant_params,
                                         const nmsis_nn_dims *input_dims,
                                         const int8_t *input_data,
                                         const nmsis_nn_dims *filter_dims,
                                         const int8_t *packed_filter_data,
                                         const nmsis_nn_dims *bias_dims,
                                         const int32_t *bias_data,
                                         const nmsis_nn_dims *output_dims,
                                         int8_t *output_data)
{
    (void)bias_dims;

    (void)ctx;
    (void)conv_params;
    (void)quant_params;
    (void)input_dims;
    (void)input_data;
    (void)filter_dims;
    (void)packed_filter_data;
    (void)bias_data;
    (void)output_dims;
    (void)output_data;

    return RISCV_NMSIS_NN_NO_IMPL_ERROR;
}

/**
 * @} end of NNConv group
 */
