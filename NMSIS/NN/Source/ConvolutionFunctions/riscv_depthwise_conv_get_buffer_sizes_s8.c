/*
 * SPDX-FileCopyrightText: Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_depthwise_conv_get_buffer_sizes_s8.c
 * Description:  Collection of get buffer size functions for the various s8 convolution layer functions.
 *
 * $Date:        1 November 2024
 * $Revision:    V.1.3.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup NNConv
 */

/**
 * @addtogroup GetBufferSizeNNConv
 * @{
 */


int32_t riscv_depthwise_conv_s8_opt_get_buffer_size_dsp(const nmsis_nn_dims *input_dims, const nmsis_nn_dims *filter_dims)
{
    return (input_dims->c * filter_dims->w * filter_dims->h) * sizeof(int16_t);
}

int32_t riscv_depthwise_conv_s8_opt_get_buffer_size(const nmsis_nn_dims *input_dims, const nmsis_nn_dims *filter_dims)
{
#if defined(RISCV_MATH_DSP) || defined(RISCV_MATH_VECTOR)
    return riscv_depthwise_conv_s8_opt_get_buffer_size_dsp(input_dims, filter_dims);
#else
    (void)input_dims;
    (void)filter_dims;
    return 0;
#endif
}

int32_t riscv_depthwise_conv_wrapper_s8_get_buffer_size(const nmsis_nn_dw_conv_params *dw_conv_params,
                                                      const nmsis_nn_dims *input_dims,
                                                      const nmsis_nn_dims *filter_dims,
                                                      const nmsis_nn_dims *output_dims)
{
    int32_t size = 0;

    if (input_dims->c == output_dims->c && input_dims->n == 1 && dw_conv_params->dilation.w == 1 &&
        dw_conv_params->dilation.h == 1)
    {
        if (filter_dims->w == 3 && filter_dims->h == 3 && dw_conv_params->padding.h <= 1 &&
            dw_conv_params->padding.w <= 1)
        {
            return size;
        }
        size = riscv_depthwise_conv_s8_opt_get_buffer_size(input_dims, filter_dims);
    }

    return size;
}

int32_t riscv_depthwise_conv_wrapper_s8_get_buffer_size_dsp(const nmsis_nn_dw_conv_params *dw_conv_params,
                                                          const nmsis_nn_dims *input_dims,
                                                          const nmsis_nn_dims *filter_dims,
                                                          const nmsis_nn_dims *output_dims)
{
    int32_t size = 0;

    if (input_dims->c == output_dims->c && input_dims->n == 1 && dw_conv_params->dilation.w == 1 &&
        dw_conv_params->dilation.h == 1)
    {
        if (filter_dims->w == 3 && filter_dims->h == 3 && dw_conv_params->padding.h <= 1 &&
            dw_conv_params->padding.w <= 1)
        {
            return size;
        }
        size = riscv_depthwise_conv_s8_opt_get_buffer_size_dsp(input_dims, filter_dims);
    }

    return size;
}


/**
 * @} end of GetBufferSizeNNConv group
 */
