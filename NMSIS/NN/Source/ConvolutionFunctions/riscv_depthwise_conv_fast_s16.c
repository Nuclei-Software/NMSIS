/*
 * SPDX-FileCopyrightText: Copyright 2022-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_depthwise_conv_fast_s16.c
 * Description:  Optimized s16 depthwise separable convolution function for
 *               channel multiplier of 1.
 *
 * $Date:        19 March 2024
 * $Revision:    V.1.4.0
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
 * Optimized s16 depthwise convolution function with constraint that in_channel equals out_channel
 *
 *  Refer prototype header file for details.
 *
 */

riscv_nmsis_nn_status riscv_depthwise_conv_fast_s16(const nmsis_nn_context *ctx,
                                                const nmsis_nn_dw_conv_params *dw_conv_params,
                                                const nmsis_nn_per_channel_quant_params *quant_params,
                                                const nmsis_nn_dims *input_dims,
                                                const int16_t *input,
                                                const nmsis_nn_dims *filter_dims,
                                                const int8_t *kernel,
                                                const nmsis_nn_dims *bias_dims,
                                                const int64_t *bias,
                                                const nmsis_nn_dims *output_dims,
                                                int16_t *output)
{
    const int32_t input_ch = input_dims->c;
    const int32_t output_ch = output_dims->c;

    /* Check input constraints input_ch == output_ch */
    if (input_ch != output_ch)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    if (filter_dims->w * filter_dims->h >= MAX_COL_COUNT)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    if (ctx->buf == NULL && riscv_depthwise_conv_fast_s16_get_buffer_size(input_dims, filter_dims) > 0)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

#if defined(RISCV_MATH_DSP)
    (void)bias_dims;
    const int32_t input_x = input_dims->w;
    const int32_t input_y = input_dims->h;
    const int32_t input_batches = input_dims->n;
    const int32_t kernel_x = filter_dims->w;
    const int32_t kernel_y = filter_dims->h;
    const int32_t pad_x = dw_conv_params->padding.w;
    const int32_t pad_y = dw_conv_params->padding.h;
    const int32_t stride_x = dw_conv_params->stride.w;
    const int32_t stride_y = dw_conv_params->stride.h;
    const int32_t *output_shift = quant_params->shift;
    const int32_t *output_mult = quant_params->multiplier;
    const int32_t output_x = output_dims->w;
    const int32_t output_y = output_dims->h;
    const int32_t output_activation_min = dw_conv_params->activation.min;
    const int32_t output_activation_max = dw_conv_params->activation.max;
    int16_t *buffer_a = (int16_t *)ctx->buf;


    /* Run the following code in cores using DSP extension */
    int16_t *const col_buffer_start = buffer_a;
    int16_t *col_buffer = col_buffer_start;
    const int64_t *const bias_start_pos = bias;
    const int32_t *const out_mult_start_pos = output_mult;
    const int32_t *const out_shift_start_pos = output_shift;
    uint16_t row_count;
    uint16_t row_shift;
    int32_t result;

    for (int i_batch = 0; i_batch < input_batches; i_batch++)
    {
        for (int i_out_y = 0; i_out_y < output_y; i_out_y++)
        {
            const int16_t base_idx_y = (i_out_y * stride_y) - pad_y;
            for (int i_out_x = 0; i_out_x < output_x; i_out_x++)
            {
                const int16_t base_idx_x = (i_out_x * stride_x) - pad_x;

                /* Out of bounds is only considered for the y axis as it provides a contiguous zero'ing opportunity than
                   along the x axis */
                const int ker_y_start = MAX(0, -base_idx_y);
                /* Condition for kernel end dimension: (base_idx_y + ker_y_end) < input_y */
                const int ker_y_end = MIN(kernel_y, input_y - base_idx_y);

                int32_t index = 0;
                if (ker_y_start != 0)
                {
                    memset(&col_buffer[index], 0, (kernel_x * input_ch) * ker_y_start * sizeof(int16_t));
                    index += (kernel_x * input_ch) * ker_y_start;
                }

                for (int i_ker_y = ker_y_start; i_ker_y < ker_y_end; i_ker_y++)
                {
                    const int32_t idx_y = base_idx_y + i_ker_y;

                    for (int i_ker_x = 0; i_ker_x < kernel_x; i_ker_x++)
                    {
                        const int32_t idx_x = base_idx_x + i_ker_x;

                        if (idx_x < 0 || idx_x >= input_x)
                        {
                            memset(&col_buffer[index], 0, input_ch * sizeof(int16_t));
                        }
                        else
                        {
                            riscv_memcpy_q15(&col_buffer[index],
                                           input + (idx_y * input_x + idx_x) * input_ch,
                                           input_ch * sizeof(int16_t));
                        }
                        index += input_ch;
                    }
                }

                const int diff = kernel_y - ker_y_end;
                if (diff != 0)
                {
                    memset(&col_buffer[index], 0, (kernel_x * input_ch) * diff * sizeof(int16_t));
                }

                row_count = output_ch / 4;
                row_shift = 0;
                bias = bias_start_pos;
                output_mult = out_mult_start_pos;
                output_shift = out_shift_start_pos;

                while (row_count)
                {
                    int32_t sum_1 = 0;
                    int32_t sum_2 = 0;
                    int32_t sum_3 = 0;
                    int32_t sum_4 = 0;

                    int32_t output_mult_1 = REDUCE_MULTIPLIER(output_mult[0]);
                    int32_t output_mult_2 = REDUCE_MULTIPLIER(output_mult[1]);
                    int32_t output_mult_3 = REDUCE_MULTIPLIER(output_mult[2]);
                    int32_t output_mult_4 = REDUCE_MULTIPLIER(output_mult[3]);
                    output_mult += 4;

                    uint16_t col_count = (kernel_x * kernel_y) / 2;
                    int16_t *col_pos = col_buffer_start + row_shift;
                    const int8_t *row_pos = kernel + row_shift;
                    row_shift += 4;

                    while (col_count)
                    {
                        /* General idea is to read 4 + 4 (input, kernel) pair and re-arrange them in the right order to
                        use in a SMLAD instruction . One run of this loop produces 4 partial outputs with 8 MACs. */
                        int32_t row_a1, row_a2, row_b1, row_b2, col_a, row_c, col_b, col_c;

                        /* Read 4 weights */
                        row_b1 = riscv_nn_read_s8x4(row_pos);
                        row_a1 = riscv_nn_read_s8x4(row_pos + input_ch);
                        col_a = riscv_nn_read_s16x2(col_pos);
                        col_b = riscv_nn_read_s16x2(col_pos + input_ch);

                        row_a2 = __SXTB16(row_b1);
                        row_b1 = __SXTB16(__ROR(row_b1, 8));

                        row_b2 = __SXTB16(row_a1);
                        row_a1 = __SXTB16(__ROR(row_a1, 8));

                        col_c = __PKHBT(col_b, col_a, 16);
                        col_a = __PKHTB(col_b, col_a, 16);
                        row_c = __PKHBT(row_b2, row_a2, 16);
                        sum_1 = __SMLAD(col_c, row_c, sum_1);

                        row_c = __PKHBT(row_b1, row_a1, 16);
                        sum_2 = __SMLAD(col_a, row_c, sum_2);

                        col_a = riscv_nn_read_s16x2(col_pos + 2);
                        col_b = riscv_nn_read_s16x2(col_pos + input_ch + 2);

                        col_c = __PKHBT(col_b, col_a, 16);
                        col_a = __PKHTB(col_b, col_a, 16);
                        row_c = __PKHTB(row_a2, row_b2, 16);
                        sum_3 = __SMLAD(col_c, row_c, sum_3);

                        row_c = __PKHTB(row_a1, row_b1, 16);
                        sum_4 = __SMLAD(col_a, row_c, sum_4);

                        row_pos += input_ch << 1;
                        col_pos += input_ch << 1;
                        col_count--;
                    }

                    col_count = (kernel_x * kernel_y) & 0x1;
                    while (col_count)
                    {
                        sum_1 += row_pos[0] * col_pos[0];
                        sum_2 += row_pos[1] * col_pos[1];
                        sum_3 += row_pos[2] * col_pos[2];
                        sum_4 += row_pos[3] * col_pos[3];

                        row_pos += input_ch;
                        col_pos += input_ch;

                        col_count--;
                    }

                    int64_t acc_1 = sum_1;
                    int64_t acc_2 = sum_2;
                    int64_t acc_3 = sum_3;
                    int64_t acc_4 = sum_4;

                    if (bias)
                    {
                        acc_1 += *bias++;
                        acc_2 += *bias++;
                        acc_3 += *bias++;
                        acc_4 += *bias++;
                    }

                    result = riscv_nn_requantize_s64(acc_1, output_mult_1, *output_shift++);
                    result = MAX(result, output_activation_min);
                    result = MIN(result, output_activation_max);
                    *output++ = (int16_t)result;

                    result = riscv_nn_requantize_s64(acc_2, output_mult_2, *output_shift++);
                    result = MAX(result, output_activation_min);
                    result = MIN(result, output_activation_max);
                    *output++ = (int16_t)result;

                    result = riscv_nn_requantize_s64(acc_3, output_mult_3, *output_shift++);
                    result = MAX(result, output_activation_min);
                    result = MIN(result, output_activation_max);
                    *output++ = (int16_t)result;

                    result = riscv_nn_requantize_s64(acc_4, output_mult_4, *output_shift++);
                    result = MAX(result, output_activation_min);
                    result = MIN(result, output_activation_max);
                    *output++ = (int16_t)result;

                    row_count--;
                }

                row_count = output_ch & 0x3;
                while (row_count)
                {
                    int16_t *col_pos = col_buffer_start + row_shift;
                    const int8_t *row_pos = kernel + row_shift;
                    int32_t sum = 0;
                    const uint16_t col_count = (kernel_x * kernel_y);
                    row_shift += 1;

                    for (int i = 0; i < col_count; i++)
                    {
                        sum += row_pos[i * input_ch] * col_pos[i * input_ch];
                    }
                    int64_t acc = sum;
                    if (bias)
                    {
                        acc += *bias++;
                    }
                    result = riscv_nn_requantize_s64(acc, REDUCE_MULTIPLIER(*output_mult), *output_shift++);
                    output_mult++;
                    result = MAX(result, output_activation_min);
                    result = MIN(result, output_activation_max);
                    *output++ = (int16_t)result;

                    row_count--;
                }
                // clear counter and pointers
                col_buffer = col_buffer_start;
            }
        }

        /* Advance to the next batch */
        input += (input_x * input_y * input_ch);
    }
#else
    /* Run the following code as reference implementation for RISC-V Cores */
    return riscv_depthwise_conv_s16(ctx,
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
#endif /* RISCV_MATH_DSP */

    /* Return to application */
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of NNConv group
 */
