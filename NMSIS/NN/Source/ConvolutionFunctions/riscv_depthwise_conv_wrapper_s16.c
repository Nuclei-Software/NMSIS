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
 * Title:        riscv_depthwise_conv_wrapper_s16.c
 * Description:  Wrapper API to select appropriate depthwise conv API based
 *               on dimensions.
 *
 * $Date:        20 January 2023
 * $Revision:    V.1.1.0
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
 *  s16 Depthwise conv wrapper function
 *
 *  Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_depthwise_conv_wrapper_s16(const nmsis_nn_context *ctx,
                                                   const nmsis_nn_dw_conv_params *dw_conv_params,
                                                   const nmsis_nn_per_channel_quant_params *quant_params,
                                                   const nmsis_nn_dims *input_dims,
                                                   const int16_t *input,
                                                   const nmsis_nn_dims *filter_dims,
                                                   const int8_t *filter,
                                                   const nmsis_nn_dims *bias_dims,
                                                   const int64_t *bias,
                                                   const nmsis_nn_dims *output_dims,
                                                   int16_t *output)
{
    riscv_nmsis_nn_status status = RISCV_NMSIS_NN_SUCCESS;

    if (USE_FAST_DW_CONV_S16_FUNCTION(dw_conv_params, filter_dims, input_dims))
    {
        status = riscv_depthwise_conv_fast_s16(ctx,
                                             dw_conv_params,
                                             quant_params,
                                             input_dims,
                                             input,
                                             filter_dims,
                                             filter,
                                             bias_dims,
                                             bias,
                                             output_dims,
                                             output);
    }
    else
    {
        status = riscv_depthwise_conv_s16(ctx,
                                        dw_conv_params,
                                        quant_params,
                                        input_dims,
                                        input,
                                        filter_dims,
                                        filter,
                                        bias_dims,
                                        bias,
                                        output_dims,
                                        output);
    }

    /* Return to application */
    return status;
}

/**
 * @} end of NNConv group
 */
