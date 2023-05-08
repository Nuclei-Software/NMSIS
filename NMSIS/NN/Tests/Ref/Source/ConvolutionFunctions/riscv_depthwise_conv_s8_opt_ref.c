/*
 * SPDX-FileCopyrightText: Copyright 2010-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_depthwise_conv_s8_opt.c
 * Description:  Optimized s8 depthwise separable convolution function for
 *               channel multiplier of 1.
 *
 * $Date:        22 March 2023
 * $Revision:    V.3.5.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "ref_functions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup Public
 */

/**
 * @addtogroup NNConv
 * @{
 */

/*
 * Optimized s8 depthwise convolution function with constraint that in_channel equals out_channel
 *
 *  Refer prototype header file for details.
 *
 */

riscv_nmsis_nn_status riscv_depthwise_conv_s8_opt_ref(const nmsis_nn_context *ctx,
                                              const nmsis_nn_dw_conv_params *dw_conv_params,
                                              const nmsis_nn_per_channel_quant_params *quant_params,
                                              const nmsis_nn_dims *input_dims,
                                              const int8_t *input,
                                              const nmsis_nn_dims *filter_dims,
                                              const int8_t *kernel,
                                              const nmsis_nn_dims *bias_dims,
                                              const int32_t *bias,
                                              const nmsis_nn_dims *output_dims,
                                              int8_t *output)
{

    const int32_t input_ch = input_dims->c;
    const int32_t output_ch = output_dims->c;

    /* Check depth multiplier is 1 */
    if (input_ch != output_ch)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    if (ctx->buf == NULL && riscv_depthwise_conv_s8_opt_get_buffer_size_ref(input_dims, filter_dims) > 0)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    /* Run the following code as reference implementation for RISC-V Core without DSP */
    return riscv_depthwise_conv_s8_ref(ctx,
                                 dw_conv_params,
                                 quant_params,
                                 input_dims,
                                 input,
                                 filter_dims,
                                 kernel,
                                 bias_dims,
                                 bias,
                                 output_dims,
                                 output);


    /* Return to application */
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of NNConv group
 */
