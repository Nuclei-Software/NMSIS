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
 * Title:        riscv_convolve_1_x_n_s8.c
 * Description:  s8 version of 1xN convolution using symmetric quantization.
 *
 * $Date:        19 March 2024
 * $Revision:    V.3.6.0
 *
 * Target Processor: RISC-V Cores
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
 * 1xN s8 convolution function.
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_convolve_1_x_n_s8(const nmsis_nn_context *ctx,
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
    riscv_nmsis_nn_status status = RISCV_NMSIS_NN_SUCCESS;

    /* The wrapper API is the ultimate reference for argument check */
    if ((input_dims->h != 1) || conv_params->dilation.w != 1 || ctx->buf == NULL || conv_params->stride.w == 0 ||
        (conv_params->stride.w * input_dims->c % 4 != 0))
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    status = riscv_convolve_s8(ctx,
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



    /* Return to application */
    return status;
}

/**
 * @} end of NNConv group
 */

