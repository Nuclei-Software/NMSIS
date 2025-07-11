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
 * Title:        riscv_convolve_1x1_s4_fast.c
 * Description:  Fast s4 version of 1x1 convolution (non-square shape)
 *
 * $Date:        01 November 2023
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
 * Fast s4 version for 1x1 convolution (non-square shape)
 *
 * Refer header file for details.
 *
 */

riscv_nmsis_nn_status riscv_convolve_1x1_s4_fast(const nmsis_nn_context *ctx,
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
    if (conv_params->padding.w != 0 || conv_params->padding.h != 0 || conv_params->stride.w != 1 ||
        conv_params->stride.h != 1)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    (void)ctx;
    (void)filter_dims;
    (void)bias_dims;

    const int32_t lhs_rows = input_dims->w * input_dims->h * input_dims->n;
    const int32_t rhs_rows = output_dims->c;
    const int32_t rhs_cols = input_dims->c;

    riscv_nn_mat_mult_nt_t_s4(input_data,
                            filter_data,
                            bias_data,
                            output_data,
                            quant_params->multiplier,
                            quant_params->shift,
                            lhs_rows,
                            rhs_rows,
                            rhs_cols,
                            conv_params->input_offset,
                            conv_params->output_offset,
                            conv_params->activation.min,
                            conv_params->activation.max,
                            rhs_cols);

    /* Return to application */
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of NNConv group
 */
