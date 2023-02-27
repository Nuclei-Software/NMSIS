/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates.
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
 * Title:        riscv_avgpool_s8.c
 * Description:  Pooling function implementations
 *
 * $Date:        01. March 2021
 * $Revision:    V.2.0.4
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

#if defined(RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)

static void scale_q31_to_q7_and_clamp(const q31_t *buffer,
                                      q7_t *target,
                                      int32_t length,
                                      const int32_t count,
                                      const int act_min,
                                      const int act_max)
{
    const int half_count = count / 2;

    // Prevent static code issue DIVIDE_BY_ZERO.
    if (count == 0)
    {
        return;
    }
#if defined (RISCV_MATH_VECTOR)
    size_t l;
    vint32m8_t vx, vy;
    uint32_t blkCnt = length;
    for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        vx = vle32_v_i32m8(buffer, l);
        buffer += l;
        vbool4_t mask = vmsgt_vx_i32m8_b4(vx, 0, l);
        vy = vadd_vx_i32m8(vx, half_count, l);
        vx = vsub_vx_i32m8(vx, half_count, l);
        vx = vmerge_vvm_i32m8(mask, vx, vy, l);
        vx = vdiv_vx_i32m8(vx, count, l);
        vx = vmin_vx_i32m8(vmax_vx_i32m8(vx, act_min, l), act_max, l);
        vse8_v_i8m2(target, vnsra_wx_i8m2(vnsra_wx_i16m4(vx, 0, l), 0, l), l);
        target += l;
    }
#else
    for (int i = 0; i < length; i++)
    {
        int32_t sum = buffer[i] > 0 ? (buffer[i] + half_count) : (buffer[i] - half_count);
        sum = sum / count;
        sum = MAX(sum, act_min);
        sum = MIN(sum, act_max);

        target[i] = (q7_t)sum;
    }
#endif /* defined (RISCV_MATH_VECTOR) */
}
#endif

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup Pooling
 * @{
 */

/*
 * s8 average pooling function
 *
 * Refer to header file for details.
 *
 */

riscv_status riscv_avgpool_s8(const nmsis_nn_context *ctx,
                          const nmsis_nn_pool_params *pool_params,
                          const nmsis_nn_dims *input_dims,
                          const q7_t *src,
                          const nmsis_nn_dims *filter_dims,
                          const nmsis_nn_dims *output_dims,
                          q7_t *dst)
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

    if (ctx->buf == NULL && riscv_avgpool_s8_get_buffer_size(output_dims->w, input_dims->c))
    {
        return RISCV_MATH_ARGUMENT_ERROR;
    }
    q31_t *buffer = (q31_t *)ctx->buf;

#if defined(RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)

    /* Run the following code for CPU's with DSP extension
     */
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
                    const q7_t *start = src + ch_src * (k_x + idx_x + (k_y + idx_y) * input_x);

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
                return RISCV_MATH_ARGUMENT_ERROR;
            }

            scale_q31_to_q7_and_clamp(buffer, dst, ch_src, count, act_min, act_max);
            dst += ch_src;
        }
    }
#else

    /* Reference C code adapted from NMSIS-NN riscv_avepool_q7_HWC.
     */
    (void)buffer;
    int16_t i_ch_in, i_x, i_y;
    int16_t k_x, k_y;

    for (i_y = 0; i_y < output_y; i_y++)
    {
        for (i_x = 0; i_x < output_x; i_x++)
        {
            for (i_ch_in = 0; i_ch_in < ch_src; i_ch_in++)
            {
                int sum = 0;
                int count = 0;
                for (k_y = i_y * stride_y - pad_y; k_y < i_y * stride_y - pad_y + kernel_y; k_y++)
                {
                    for (k_x = i_x * stride_x - pad_x; k_x < i_x * stride_x - pad_x + kernel_x; k_x++)
                    {
                        if (k_y >= 0 && k_x >= 0 && k_y < input_y && k_x < input_x)
                        {
                            sum += src[i_ch_in + ch_src * (k_x + k_y * input_x)];
                            count++;
                        }
                    }
                }

                // Prevent static code issue DIVIDE_BY_ZERO.
                if (count == 0)
                {
                    return RISCV_MATH_ARGUMENT_ERROR;
                }

                sum = sum > 0 ? (sum + count / 2) / count : (sum - count / 2) / count;
                sum = MAX(sum, act_min);
                sum = MIN(sum, act_max);

                dst[i_ch_in + ch_src * (i_x + i_y * output_x)] = sum;
            }
        }
    }

#endif /* defined(RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR) */
    return RISCV_MATH_SUCCESS;
}


int32_t riscv_avgpool_s8_get_buffer_size(const int output_x, const int ch_src)
{
    (void)output_x;

#if defined(RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)
    return (ch_src * sizeof(int32_t));
#else
    (void)ch_src;
    return 0;
#endif
}
/**
 * @} end of Pooling group
 */
