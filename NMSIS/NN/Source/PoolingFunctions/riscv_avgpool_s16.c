/*
 * SPDX-FileCopyrightText: Copyright 2022-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_avgpool_s16.c
 * Description:  Pooling function implementations
 *
 * $Date:        27 November 2023
 * $Revision:    V.2.5.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

#if defined(RISCV_MATH_DSP)

static void scale_q31_to_q15_and_clamp(const int32_t *buffer,
                                       int16_t *target,
                                       int32_t length,
                                       const int32_t count,
                                       const int act_min,
                                       const int act_max)
{
    const int half_count = count / 2;

    for (int i = 0; i < length; i++)
    {
        int32_t sum = buffer[i] > 0 ? (buffer[i] + half_count) : (buffer[i] - half_count);
        sum = sum / count;
        sum = MAX(sum, act_min);
        sum = MIN(sum, act_max);

        target[i] = (int16_t)sum;
    }
}
#endif

/**
 *  @ingroup Public

 */

/**
 * @addtogroup Pooling
 * @{
 */

/*
 * s16 average pooling function
 *
 * Refer to header file for details.
 *
 */
riscv_nmsis_nn_status riscv_avgpool_s16(const nmsis_nn_context *ctx,
                                    const nmsis_nn_pool_params *pool_params,
                                    const nmsis_nn_dims *input_dims,
                                    const int16_t *src,
                                    const nmsis_nn_dims *filter_dims,
                                    const nmsis_nn_dims *output_dims,
                                    int16_t *dst)
{
    const int32_t input_y = input_dims->h;
    const int32_t input_x = input_dims->w;
    const int32_t output_y = output_dims->h;
    const int32_t output_x = output_dims->w;
    const int32_t stride_y = pool_params->stride.h;
    const int32_t stride_x = pool_params->stride.w;
    const int32_t kernel_y = filter_dims->h;
    const int32_t kernel_x = filter_dims->w;
    const int32_t pad_y = pool_params->padding.h;
    const int32_t pad_x = pool_params->padding.w;
    const int32_t act_min = pool_params->activation.min;
    const int32_t act_max = pool_params->activation.max;
    const int32_t ch_src = input_dims->c;
    const int32_t batch_input = input_x * input_y * ch_src;
    int32_t batch_cnt = input_dims->n;

    if (batch_cnt < 1)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

#if defined(RISCV_MATH_DSP)
    /* Run the following code for CPU's with DSP extension
     */
    int32_t *buffer = (int32_t *)ctx->buf;

    if (buffer == NULL)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    while (batch_cnt)
    {

        for (int i_y = 0, idx_y = -pad_y; i_y < output_y; idx_y += stride_y, i_y++)
        {
            for (int i_x = 0, idx_x = -pad_x; i_x < output_x; idx_x += stride_x, i_x++)
            {
                /* Condition for kernel start dimension:
                   (base_idx_<x,y> + kernel_<x,y>_start) >= 0 */
                const int32_t kernel_y_start = MAX(0, -idx_y);
                const int32_t kernel_x_start = MAX(0, -idx_x);

                /* Condition for kernel end dimension:
                   (base_idx_<x,y> + kernel_<x,y>_end) < dim_src_<width,height> */
                const int32_t kernel_y_end = MIN(kernel_y, input_y - idx_y);
                const int32_t kernel_x_end = MIN(kernel_x, input_x - idx_x);

                int count = 0;

                for (int k_y = kernel_y_start; k_y < kernel_y_end; k_y++)
                {
                    for (int k_x = kernel_x_start; k_x < kernel_x_end; k_x++)
                    {
                        const int16_t *start = src + ch_src * (k_x + idx_x + (k_y + idx_y) * input_x);

                        if (count == 0)
                        {
                            for (int i = 0; i < ch_src; i++)
                            {
                                buffer[i] = start[i];
                            }
                        }
                        else
                        {
                            for (int i = 0; i < ch_src; i++)
                            {
                                buffer[i] = __QADD(start[i], buffer[i]);
                            }
                        }
                        count++;
                    }
                }

                // Prevent static code issue DIVIDE_BY_ZERO.
                if (count == 0)
                {
                    return RISCV_NMSIS_NN_ARG_ERROR;
                }

                scale_q31_to_q15_and_clamp(buffer, dst, ch_src, count, act_min, act_max);
                dst += ch_src;
            }
        }
        src += batch_input;

        batch_cnt--;
    }

#else
    /* Reference C code adapted from NMSIS-NN riscv_avgpool_s8.c.
     */
    const int32_t batch_output = output_x * output_y * ch_src;
    (void)ctx;

    while (batch_cnt)
    {
        for (int i_y = 0, base_idx_y = -pad_y; i_y < output_y; base_idx_y += stride_y, i_y++)
        {
            for (int i_x = 0, base_idx_x = -pad_x; i_x < output_x; base_idx_x += stride_x, i_x++)
            {
                /* Condition for kernel start dimension: (base_idx_<x,y> + kernel_<x,y>_start) >= 0 */
                const int32_t ker_y_start = MAX(0, -base_idx_y);
                const int32_t ker_x_start = MAX(0, -base_idx_x);

                /* Condition for kernel end dimension: (base_idx_<x,y> + kernel_<x,y>_end) < dim_src_<width,height> */
                const int32_t kernel_y_end = MIN(kernel_y, input_y - base_idx_y);
                const int32_t kernel_x_end = MIN(kernel_x, input_x - base_idx_x);

                for (int i_ch_in = 0; i_ch_in < ch_src; i_ch_in++)
                {
                    int sum = 0;
                    int count = 0;

                    for (int k_y = ker_y_start; k_y < kernel_y_end; k_y++)
                    {
                        for (int k_x = ker_x_start; k_x < kernel_x_end; k_x++)
                        {
                            sum += src[i_ch_in + ch_src * (k_x + base_idx_x + (k_y + base_idx_y) * input_x)];
                            count++;
                        }
                    }

                    // Prevent static code issue DIVIDE_BY_ZERO.
                    if (count == 0)
                    {
                        return RISCV_NMSIS_NN_ARG_ERROR;
                    }

                    sum = sum > 0 ? (sum + count / 2) / count : (sum - count / 2) / count;
                    sum = MAX(sum, act_min);
                    sum = MIN(sum, act_max);

                    dst[i_ch_in + ch_src * (i_x + i_y * output_x)] = sum;
                }
            }
        }
        src += batch_input;
        dst += batch_output;

        batch_cnt--;
    }
#endif

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Pooling group
 */
