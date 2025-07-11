/*
 * SPDX-FileCopyrightText: Copyright 2021-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_convolve_wrapper_s16.c
 * Description:  s16 convolution layer wrapper function with the main purpose to call the optimal kernel available in
 * nmsis-nn to perform the convolution.
 *
 * $Date:        23 April 2024
 * $Revision:    V.3.0.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"

/**
 *  @ingroup Public
 */

/**
 * @addtogroup NNConv
 * @{
 */

/*
 * Convolution layer
 *
 * Refer header file for details.
 *
 */

riscv_nmsis_nn_status riscv_convolve_wrapper_s16(const nmsis_nn_context *ctx,
                                             const nmsis_nn_conv_params *conv_params,
                                             const nmsis_nn_per_channel_quant_params *quant_params,
                                             const nmsis_nn_dims *input_dims,
                                             const int16_t *input_data,
                                             const nmsis_nn_dims *filter_dims,
                                             const int8_t *filter_data,
                                             const nmsis_nn_dims *bias_dims,
                                             const nmsis_nn_bias_data *bias_data,
                                             const nmsis_nn_dims *output_dims,
                                             int16_t *output_data)
{
    return riscv_convolve_s16(ctx,
                            conv_params,
                            quant_params,
                            input_dims,
                            input_data,
                            filter_dims,
                            filter_data,
                            bias_dims,
                            bias_data,
                            output_dims,
                            output_data);
}

/**
 * @} end of NNConv group
 */
