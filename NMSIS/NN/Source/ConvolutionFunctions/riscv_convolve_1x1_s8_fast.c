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
 * Title:        riscv_convolve_1x1_s8_fast.c
 * Description:  Fast s8 version of 1x1 convolution (non-square shape)
 *
 * $Date:        05 November 2024
 * $Revision:    V.3.6.0
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
 * Fast s8 version for 1x1 convolution (non-square shape)
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_convolve_1x1_s8_fast(const nmsis_nn_context *ctx,
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

    (void)filter_dims;
    (void)bias_dims;

    const int32_t rhs_cols = input_dims->c;
    const int32_t rhs_rows = output_dims->c;
    int32_t lhs_rows = input_dims->w * input_dims->h * input_dims->n;

#if defined(RISCV_MATH_DSP) && !defined(RISCV_MATH_VECTOR)
    if (ctx->buf != NULL) /* Fall back to non buffered version if no additional memory buffer provided */
    {
        const int32_t batch = input_dims->n;
        const int32_t output_h = output_dims->h;
        const int32_t output_w = output_dims->w;
        const int32_t input_inc = input_dims->w * rhs_cols;

        for (int i_batch = 0; i_batch < batch; i_batch++)
        {
            const int32_t output_ch = output_dims->c;
            const int8_t *ip = input_data;
            int16_t *buffer_a = (int16_t *)ctx->buf;
            int16_t *im2col_buf = (int16_t *)ctx->buf;
            int8_t *out = output_data;
            lhs_rows = 0;

            for (int i_out_y = 0; i_out_y < output_h; i_out_y++, ip += input_inc)
            {
                for (int32_t k_x = 0, i_out_x = 0; i_out_x < output_w; i_out_x++, k_x += rhs_cols)
                {
                    riscv_s8_to_s16_unordered_with_offset(ip + k_x, im2col_buf, rhs_cols, conv_params->input_offset);
                    im2col_buf += rhs_cols;
                    lhs_rows++;
                    if (lhs_rows == 2)
                    {
                        out = riscv_nn_mat_mult_kernel_s8_s16(filter_data,
                                                            buffer_a,
                                                            output_ch,
                                                            quant_params->shift,
                                                            quant_params->multiplier,
                                                            conv_params->output_offset,
                                                            conv_params->activation.min,
                                                            conv_params->activation.max,
                                                            rhs_cols,
                                                            rhs_cols,
                                                            bias_data,
                                                            out);
                        im2col_buf = buffer_a;
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
                for (int i = 0; i < output_ch; i++)
                {
                    /* Load the accumulator with bias first */
                    int32_t sum = 0;
                    if (bias_data)
                    {
                        sum = bias_data[i];
                    }
                    const int16_t *ip_as_col = buffer_a;

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
                    int64_t ker_a64, ip_b64, ker_a1, ker_a2;
                    int64_t sum64 = 0;
                    /* 8 multiply and accumulates are done in one loop. */
                    uint16_t col_count = rhs_cols >> 3;
#else
                    /* 4 multiply and accumulates are done in one loop. */
                    uint16_t col_count = rhs_cols >> 2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
                    while (col_count)
                    {
#if __RISCV_XLEN == 64
                        ker_a =
                            read_and_pad_reordered64(ker_a, &ker_a1, &ker_a2);
                        ip_b64 = riscv_nn_read_s16x4_ia(&ip_as_col);
                        sum64 = __SMLAD(ker_a1, ip_b64, sum64);
                        ip_b64 = riscv_nn_read_s16x4_ia(&ip_as_col);
                        sum64 = __SMLAD(ker_a2, ip_b64, sum64);
#else
#if defined(NUCLEI_DSP_N3)
                        ker_a =
                            read_and_pad_reordered64(ker_a, &ker_a1, &ker_a2);
                        ip_b64 = riscv_nn_read_s16x4_ia(&ip_as_col);
                        sum64 = __RV_DKMADA(sum64, ker_a1, ip_b64);
                        ip_b64 = riscv_nn_read_s16x4_ia(&ip_as_col);
                        sum64 = __RV_DKMADA(sum64, ker_a2, ip_b64);
#else
                        int32_t ker_a1, ker_a2;
                        int32_t ip_b1, ip_b2;

                        ker_a = read_and_pad_reordered(ker_a, &ker_a1, &ker_a2);
                        ip_b1 = riscv_nn_read_q15x2_ia(&ip_as_col);
                        sum = __SMLAD(ker_a1, ip_b1, sum);
                        ip_b2 = riscv_nn_read_q15x2_ia(&ip_as_col);
                        sum = __SMLAD(ker_a2, ip_b2, sum);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

                        col_count--;
                    }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
                    sum += (int32_t)sum64 + (int32_t)(sum64 >> 32);
                    /* Handle left over mac */
                    col_count = rhs_cols & 0x7;
#else
                    /* Handle left over mac */
                    col_count = rhs_cols & 0x3;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
                    while (col_count)
                    {
                        int8_t ker_a1 = *ker_a++;
                        int16_t ip_b1 = *ip_as_col++;
                        sum += ker_a1 * ip_b1;
                        col_count--;
                    }
                    sum = riscv_nn_requantize(sum, quant_params->multiplier[i], quant_params->shift[i]);
                    sum += conv_params->output_offset;
                    sum = MAX(sum, conv_params->activation.min);
                    sum = MIN(sum, conv_params->activation.max);
                    *out++ = (int8_t)sum;
                }
            }
            /* Advance to the next batch */
            input_data += (input_dims->w * input_dims->h * rhs_cols);
            output_data += (output_w * output_h * output_ch);
        }
        return RISCV_NMSIS_NN_SUCCESS;
    }
#else
    (void)ctx;
#endif

    riscv_nn_mat_mult_nt_t_s8(input_data,
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
                            rhs_rows,
                            rhs_cols);

    /* Return to application */
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of NNConv group
 */
