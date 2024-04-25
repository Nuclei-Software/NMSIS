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
 * Title:        riscv_convolve_s8.c
 * Description:  s8 version of convolution using symmetric quantization.
 *
 * $Date:        08 June 2023
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
   * Basic s8 convolution function.
   *
   * Refer header file for details. Optimal use case for the DSP implementation is when input and output channels
   * are multiples of 4 or atleast greater than 4.
   *
   */

riscv_nmsis_nn_status riscv_convolve_s8_ref(const nmsis_nn_context *ctx,
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
    (void)bias_dims;

    if (ctx->buf == NULL)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }
    int16_t *buffer_a = (int16_t *)ctx->buf;

    const int32_t input_batches = input_dims->n;
    const uint16_t input_x = input_dims->w;
    const uint16_t input_y = input_dims->h;
    const uint16_t input_ch = input_dims->c;
    const uint16_t kernel_x = filter_dims->w;
    const uint16_t kernel_y = filter_dims->h;
    const uint16_t output_x = output_dims->w;
    const uint16_t output_y = output_dims->h;
    const uint16_t output_ch = output_dims->c;

    const uint16_t pad_x = conv_params->padding.w;
    const uint16_t pad_y = conv_params->padding.h;
    const uint16_t stride_x = conv_params->stride.w;
    const uint16_t stride_y = conv_params->stride.h;
    const int32_t dilation_x = conv_params->dilation.w;
    const int32_t dilation_y = conv_params->dilation.h;
    const int32_t out_offset = conv_params->output_offset;
    const int32_t out_activation_min = conv_params->activation.min;
    const int32_t out_activation_max = conv_params->activation.max;
    const int32_t rhs_cols = kernel_x * kernel_y * input_ch;
    const int32_t input_offset = conv_params->input_offset;

    int32_t *output_mult = quant_params->multiplier;
    int32_t *output_shift = quant_params->shift;

    int i_batch;
    for (i_batch = 0; i_batch < input_batches; i_batch++)
    {

        const int32_t remainder = rhs_cols % 4;
        const int32_t aligned_rhs_cols = remainder != 0 ? rhs_cols + 4 - remainder : rhs_cols;

        /* Use as a ping-pong buffer for unordered elements */
        int8_t *im2col_buf = (int8_t *)buffer_a + aligned_rhs_cols * 2;
        int16_t *im2col_buf_start_s16 = buffer_a;
        int8_t *out = output_data;
        int32_t lhs_rows = 0;
        /* This part implements the im2col function */
        for (int i_out_y = 0; i_out_y < output_y; i_out_y++)
        {
            for (int i_out_x = 0; i_out_x < output_x; i_out_x++)
            {
                const int32_t base_idx_x = stride_x * i_out_x - pad_x;
                const int32_t base_idx_y = stride_y * i_out_y - pad_y;

                for (int32_t i_ker_y = 0; i_ker_y < kernel_y; i_ker_y++)
                {
                    for (int32_t i_ker_x = 0; i_ker_x < kernel_x; i_ker_x++)
                    {
                        const int32_t k_y = base_idx_y + dilation_y * i_ker_y;
                        const int32_t k_x = base_idx_x + dilation_x * i_ker_x;

                        if (k_y < 0 || k_y >= input_y || k_x < 0 || k_x >= input_x)
                        {
                            riscv_memset_s8(im2col_buf, (int8_t)-input_offset, sizeof(int8_t) * input_ch);
                        }
                        else
                        {
                            riscv_memcpy_s8(im2col_buf, input_data + (k_y * input_x + k_x) * input_ch, input_ch);
                        }
                        im2col_buf += input_ch;
                    }
                }
                lhs_rows++;

                /* Computation is filed for every 2 columns */
                riscv_q7_to_q15_with_offset_ref(im2col_buf - rhs_cols, im2col_buf_start_s16, rhs_cols, (int16_t)input_offset);
                im2col_buf_start_s16 += aligned_rhs_cols;

                if (lhs_rows == 2)
                {
                    out = riscv_nn_mat_mult_kernel_s8_s16_ref(filter_data,
                                                        buffer_a,
                                                        output_ch,
                                                        output_shift,
                                                        output_mult,
                                                        out_offset,
                                                        out_activation_min,
                                                        out_activation_max,
                                                        rhs_cols,
                                                        aligned_rhs_cols,
                                                        bias_data,
                                                        out);

                    /* counter reset */
                    im2col_buf_start_s16 = buffer_a;
                    im2col_buf = (int8_t *)buffer_a + aligned_rhs_cols * 2;
                    lhs_rows = 0;
                }
            }

            if (out == NULL)
            {
                return RISCV_NMSIS_NN_NO_IMPL_ERROR;
            }
        }

        /* Handle left over columns */
        if (lhs_rows != 0)
        {
            const int8_t *ker_a = filter_data;
            int i;

            for (i = 0; i < output_ch; i++)
            {
                /* Load the accumulator with bias first */
                int32_t sum = 0;
                if (bias_data)
                {
                    sum = bias_data[i];
                }

                /* Point to the beginning of the im2col buffer where the input is available as a rearranged column */
                const int16_t *ip_as_col = buffer_a;

                uint16_t col_count = rhs_cols;

                while (col_count)
                {
                    int8_t ker_a1 = *ker_a++;
                    int16_t ip_b1 = *ip_as_col++;
                    sum += ker_a1 * ip_b1;
                    col_count--;
                }

                sum = riscv_nn_requantize(sum, output_mult[i], output_shift[i]);
                sum += out_offset;
                sum = MAX(sum, out_activation_min);
                sum = MIN(sum, out_activation_max);
                *out++ = (int8_t)sum;
            }
        }
        /* Advance to the next batch */
        input_data += (input_x * input_y * input_ch);
        output_data += (output_x * output_y * output_ch);
    }

    /* Return to application */
    return RISCV_NMSIS_NN_SUCCESS;
}
/**
 * @} end of NNConv group
 */
