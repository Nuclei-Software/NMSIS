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
 * Title:        riscv_svdf_s8.c
 * Description:  S8 basic SVDF layer function with s16 state tensor
 *
 * $Date:        5 January 2023
 * $Revision:    V.3.1.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup Public
 */

/**
 * @addtogroup SVDF
 * @{
 */

/*
 * S8 SVDF layer function for TensorFlow Lite with 16 bit state tensor
 *
 * Refer to header file for details.
 *
 */

riscv_nmsis_nn_status riscv_svdf_state_s16_s8(const nmsis_nn_context *input_ctx,
                                          const nmsis_nn_context *output_ctx,
                                          const nmsis_nn_svdf_params *svdf_params,
                                          const nmsis_nn_per_tensor_quant_params *input_quant_params,
                                          const nmsis_nn_per_tensor_quant_params *output_quant_params,
                                          const nmsis_nn_dims *input_dims,
                                          const int8_t *input_data,
                                          const nmsis_nn_dims *state_dims,
                                          int16_t *state_data,
                                          const nmsis_nn_dims *weights_feature_dims,
                                          const int8_t *weights_feature_data,
                                          const nmsis_nn_dims *weights_time_dims,
                                          const int16_t *weights_time_data,
                                          const nmsis_nn_dims *bias_dims,
                                          const int32_t *bias_data,
                                          const nmsis_nn_dims *output_dims,
                                          int8_t *output_data)
{
    (void)bias_dims;
    (void)state_dims;
    (void)output_dims;

    const int32_t multiplier_in = input_quant_params->multiplier;
    const int32_t shift_in = input_quant_params->shift;
    const int32_t multiplier_out = output_quant_params->multiplier;
    const int32_t shift_2 = output_quant_params->shift;
    const int32_t zp_in = svdf_params->input_offset;
    const int32_t zp_out = svdf_params->output_offset;
    const int32_t in_activation_min = svdf_params->input_activation.min;
    const int32_t in_activation_max = svdf_params->input_activation.max;
    const int32_t out_activation_min = svdf_params->output_activation.min;
    const int32_t out_activation_max = svdf_params->output_activation.max;
    const int16_t rank = svdf_params->rank;

    const int32_t input_batches = input_dims->n;
    const int32_t input_height = input_dims->h;
    const int32_t feature_batches = weights_feature_dims->n;
    const int32_t time_batches = weights_time_dims->h;
    const int32_t unit_count = feature_batches / rank;

    if (input_ctx->buf == NULL)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }
    int32_t *buffer_a = (int32_t *)input_ctx->buf;

    if (output_ctx->buf == NULL)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }
    int32_t *buffer_b = (int32_t *)output_ctx->buf;

    // Left shift state
    memmove((int16_t *)state_data,
            (int16_t *)state_data + 1,
            (size_t)((input_batches * feature_batches * time_batches - 1) * (int32_t)sizeof(int16_t)));

    // Matrix multiplication input * feature weight
    for (int i_batch = 0; i_batch < input_batches; i_batch++)
    {
        int16_t *res_ptr = state_data + (time_batches * i_batch * feature_batches) + (time_batches - 1);
        const int8_t *weight = weights_feature_data;
        const int8_t *input = input_data + i_batch * input_height;

        riscv_nmsis_nn_status res = riscv_nn_vec_mat_mult_t_svdf_s8(input,
                                                                weight,
                                                                res_ptr,
                                                                -zp_in,
                                                                time_batches,
                                                                multiplier_in,
                                                                shift_in,
                                                                input_height,
                                                                feature_batches,
                                                                in_activation_min,
                                                                in_activation_max);

        if (res != RISCV_NMSIS_NN_SUCCESS)
        {
            return res;
        }
    }

    {
        // Matrix multiplication time weight * state tensors
        int32_t *ptr_a = buffer_a;
        const int16_t *v2 = state_data;
        for (int i_batch = 0; i_batch < input_batches; i_batch++)
        {
            const int16_t *v1 = weights_time_data;

            for (int i_feature_batch = 0; i_feature_batch < feature_batches; i_feature_batch++)
            {
                *ptr_a = 0;
                int32_t sum = 0;
#if defined(RISCV_MATH_DSP)
                // Perform matrix multiplication in blocks of two
                int j = 0;
                int32_t block_count = time_batches >> 1;
                for (int i = 0; i < block_count; i++)
                {
                    j += 2;
                    int32_t r1 = riscv_nn_read_q15x2_ia(&v1);
                    int32_t r2 = riscv_nn_read_q15x2_ia(&v2);

                    sum = __SMLAD(r1, r2, sum);
                }

                // Process the remaining data
                for (; j < time_batches; j++)
                {
                    sum += *v1 * *v2;
                    v1++;
                    v2++;
                }
#else
                for (int j = 0; j < time_batches; j++)
                {
                    sum += *v1 * *v2;
                    v1++;
                    v2++;
                }
#endif

                *ptr_a = sum;
                ptr_a++;
            }
        }
    }

    if (bias_data)
    {
        if (unit_count == feature_batches)
        {
            for (int i = 0; i < input_batches; i++)
            {
                int32_t *output_temp = buffer_b + i * feature_batches;
                const int32_t *ptr_a = buffer_a + i * feature_batches;

                const int32_t *bi = bias_data;
                for (int j = 0; j < feature_batches; j++)
                {
                    output_temp[j] = ptr_a[j] + bi[j];
                }
            }
        }
        else
        {
            for (int i_batch = 0; i_batch < input_batches; i_batch++)
            {
                int32_t *output_data_temp = buffer_b + i_batch * unit_count;
                int32_t *ptr_a = buffer_a + i_batch * feature_batches;

                for (int i = 0; i < unit_count; i++)
                {
                    int32_t sum = bias_data[i];
                    for (int j = 0; j < rank; j++)
                    {
                        sum += *ptr_a;
                        ptr_a++;
                    }
                    output_data_temp[i] = sum;
                }
            }
        }
    }
    else
    {
        for (int i_batch = 0; i_batch < input_batches; i_batch++)
        {
            int32_t *output_data_temp = buffer_b + i_batch * unit_count;
            int32_t *ptr_a = buffer_a + i_batch * feature_batches;

            for (int i = 0; i < unit_count; i++)
            {
                int32_t sum = 0;
                for (int j = 0; j < rank; j++)
                {
                    sum += *ptr_a;
                    ptr_a++;
                }
                output_data_temp[i] = sum;
            }
        }
    }

    for (int i = 0; i < input_batches * unit_count; i++)
    {
        output_data[i] = (int8_t)CLAMP(
            riscv_nn_requantize(buffer_b[i], multiplier_out, shift_2) + zp_out, out_activation_max, out_activation_min);
    }

    return (RISCV_NMSIS_NN_SUCCESS);
}

/**
 * @} end of SVDF group
 */
