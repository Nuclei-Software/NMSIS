/*
 * SPDX-FileCopyrightText: Copyright 2010-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_convolve_wrapper_s8.c
 * Description:  s8 convolution layer wrapper function with the main purpose to call the optimal kernel available in
 * nmsis-nn to perform the convolution.
 *
 * $Date:        04 November 2024
 * $Revision:    V.2.5.1
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

riscv_nmsis_nn_status riscv_convolve_wrapper_s8(const nmsis_nn_context *ctx,
                                            const nmsis_nn_conv_params *conv_params,
                                            const nmsis_nn_per_channel_quant_params *quant_params,
                                            const nmsis_nn_dims *input_dims,
                                            const int8_t *input_data,
                                            const nmsis_nn_dims *filter_dims,
                                            const int8_t *filter_data,
                                            const nmsis_nn_dims *bias_dims,
                                            const int32_t *bias_data,
                                            const nmsis_nn_dims *output_dims,
                                            int8_t *output_data)
{
    if ((conv_params->padding.w == 0) && (conv_params->padding.h == 0) && (filter_dims->w == 1) &&
        (filter_dims->h == 1) && (conv_params->dilation.w == 1 && conv_params->dilation.h == 1) &&
        (input_dims->c == filter_dims->c))
    {
        if ((conv_params->stride.w == 1) && (conv_params->stride.h == 1))
        {
            return riscv_convolve_1x1_s8_fast(ctx,
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
        else
        {
            return riscv_convolve_1x1_s8(ctx,
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
    }
    else if ((input_dims->h == 1) && conv_params->dilation.w == 1 && (filter_dims->h == 1) &&
             ((conv_params->stride.w * input_dims->c) % 4 == 0) && (input_dims->c == filter_dims->c))
    {
        return riscv_convolve_1_x_n_s8(ctx,
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
    else
    {
        return riscv_convolve_s8(ctx,
                               conv_params,
                               quant_params,
                               input_dims,
                               input_data,
                               filter_dims,
                               filter_data,
                               bias_dims,
                               bias_data,
                               NULL,
                               output_dims,
                               output_data);
    }
}

/**
 * @} end of NNConv group
 */
