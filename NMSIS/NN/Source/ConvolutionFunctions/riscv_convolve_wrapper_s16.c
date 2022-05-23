/*
 * Copyright (C) 2021-2022 Arm Limited or its affiliates. All rights reserved.
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
 * $Date:        13 January 2022
 * $Revision:    V.1.2.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"

/**
 *  @ingroup groupNN
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

riscv_status riscv_convolve_wrapper_s16(const nmsis_nn_context *ctx,
                                    const nmsis_nn_conv_params *conv_params,
                                    const nmsis_nn_per_channel_quant_params *quant_params,
                                    const nmsis_nn_dims *input_dims,
                                    const q15_t *input_data,
                                    const nmsis_nn_dims *filter_dims,
                                    const q7_t *filter_data,
                                    const nmsis_nn_dims *bias_dims,
                                    const int64_t *bias_data,
                                    const nmsis_nn_dims *output_dims,
                                    q15_t *output_data)
{
#if defined(RISCV_MATH_DSP)
    if (filter_dims->w * filter_dims->h * input_dims->c < 512 &&
        (conv_params->dilation.w == 1 && conv_params->dilation.h == 1))
    {
        return riscv_convolve_fast_s16(ctx,
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
#else
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
#endif
}

int32_t riscv_convolve_wrapper_s16_get_buffer_size(const nmsis_nn_conv_params *conv_params,
                                                 const nmsis_nn_dims *input_dims,
                                                 const nmsis_nn_dims *filter_dims,
                                                 const nmsis_nn_dims *output_dims)
{
    (void)conv_params;
    (void)output_dims;

#if defined(RISCV_MATH_DSP)
    if (filter_dims->w * filter_dims->h * input_dims->c < 512 &&
        (conv_params->dilation.w == 1 && conv_params->dilation.h == 1))
    {
        return riscv_convolve_fast_s16_get_buffer_size(input_dims, filter_dims);
    }

    return riscv_convolve_s16_get_buffer_size(input_dims, filter_dims);
#else
    return riscv_convolve_s16_get_buffer_size(input_dims, filter_dims);
#endif
}

/**
 * @} end of NNConv group
 */
