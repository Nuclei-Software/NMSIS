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
 * Title:        riscv_svdf_s8.c
 * Description:  S8 basic SVDF layer function
 *
 * $Date:        14 Feb 2024
 * $Revision:    V.6.1.0
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
 * S8 SVDF layer function for TensorFlow Lite with 8 bit state tensor
 *
 * Refer to header file for details.
 *
 */

riscv_nmsis_nn_status riscv_svdf_s8(const nmsis_nn_context *ctx,
                                const nmsis_nn_context *input_ctx,
                                const nmsis_nn_context *output_ctx,
                                const nmsis_nn_svdf_params *svdf_params,
                                const nmsis_nn_per_tensor_quant_params *input_quant_params,
                                const nmsis_nn_per_tensor_quant_params *output_quant_params,
                                const nmsis_nn_dims *input_dims,
                                const int8_t *input_data,
                                const nmsis_nn_dims *state_dims,
                                int8_t *state_data,
                                const nmsis_nn_dims *weights_feature_dims,
                                const int8_t *weights_feature_data,
                                const nmsis_nn_dims *weights_time_dims,
                                const int8_t *weights_time_data,
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

    int32_t *kernel_sum_data = (int32_t *)ctx->buf;

    // Left shift state
    memmove((int8_t *)state_data,
            (int8_t *)state_data + 1,
            (size_t)((input_batches * feature_batches * time_batches - 1) * (int32_t)sizeof(int8_t)));

    // Matrix multiplication input * feature weight
    for (int i_batch = 0; i_batch < input_batches; i_batch++)
    {
        int8_t *res_ptr = state_data + (time_batches * i_batch * feature_batches) + (time_batches - 1);
        const int8_t *input = input_data + i_batch * input_height;

        riscv_nmsis_nn_status res = riscv_nn_vec_mat_mult_t_s8(input,
                                                           weights_feature_data,
                                                           NULL,
                                                           NULL,
                                                           res_ptr,
                                                           -zp_in,
                                                           0,
                                                           multiplier_in,
                                                           shift_in,
                                                           input_height,
                                                           feature_batches,
                                                           in_activation_min,
                                                           in_activation_max,
                                                           time_batches,
                                                           0);

        if (res != RISCV_NMSIS_NN_SUCCESS)
        {
            return res;
        }
    }

    // Matrix multiplicate time weight * state tensors
    {
        int32_t *ptr_a = buffer_a;
        const int8_t *v2 = state_data;
        for (int i_batch = 0; i_batch < input_batches; i_batch++)
        {
            const int8_t *v1 = weights_time_data;

            for (int i_feature_batch = 0; i_feature_batch < feature_batches; i_feature_batch++)
            {
                *ptr_a = 0;
                int32_t sum = 0;
                int j;
#if defined(RISCV_MATH_VECTOR)
                uint32_t blkCnt;                               /* Loop counter */
                size_t l;
                vint32m1_t v_temp;
                vint8m4_t a8m4, b8m4;
                blkCnt = time_batches & (~RVV_OPT_THRESHOLD);
                int tmp_j = blkCnt;
                l = __riscv_vsetvl_e32m1(1);
                v_temp = __riscv_vmv_v_x_i32m1(0, l);
                for (; (l = __riscv_vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
                    a8m4 = __riscv_vle8_v_i8m4(v1, l);
                    v1 += l;
                    b8m4 = __riscv_vle8_v_i8m4(v2, l);
                    v2 += l;
                    v_temp = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwmul_vv_i16m8(a8m4, b8m4, l), v_temp, l);
                }
                sum += __riscv_vmv_x_s_i32m1_i32(v_temp);
                j = tmp_j;

#elif defined(RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
                int64_t sum64 = 0;
                int32_t block_count = time_batches >> 3;
#else
                int32_t block_count = time_batches >> 2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
                for (int i = 0; i < block_count; i++)
                {
#if __RISCV_XLEN == 64
                    int64_t r1_1, r1_2, r2_1, r2_2;
                    v1 = read_and_pad_reordered64(v1, &r1_1, &r1_2);
                    v2 = read_and_pad_reordered64(v2, &r2_1, &r2_2);
                    sum64 = __SMLAD(r1_1, r2_1, sum64);
                    sum64 = __SMLAD(r1_2, r2_2, sum64);
#else
#if defined (NUCLEI_DSP_N3)
                    int64_t r1_1, r1_2, r2_1, r2_2;
                    v1 = read_and_pad_reordered64(v1, &r1_1, &r1_2);
                    v2 = read_and_pad_reordered64(v2, &r2_1, &r2_2);
                    sum64 = __RV_DKMADA(sum64, r1_1, r2_1);
		    sum64 = __RV_DKMADA(sum64, r1_2, r2_2);
#else
                    int32_t r1_1, r1_2, r2_1, r2_2;
                    v1 = read_and_pad_reordered(v1, &r1_1, &r1_2);
                    v2 = read_and_pad_reordered(v2, &r2_1, &r2_2);
                    sum = __SMLAD(r1_1, r2_1, sum);
                    sum = __SMLAD(r1_2, r2_2, sum);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
                }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
                sum = (int32_t)sum64 + (int32_t)(sum64 >> 32);
                j = 8 * block_count;
#else
                j = 4 * block_count;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */


#else
                j = 0;
#endif /* defined (RISCV_MATH_DSP) */

                for (; j < time_batches; j++)
                {
                    sum += *v1 * *v2;
                    v1++;
                    v2++;
                }

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
                int j;
#if defined(RISCV_MATH_VECTOR)
                uint32_t blkCnt;                               /* Loop counter */
                size_t l;
                vint32m8_t a32m8, b32m8;
                blkCnt = feature_batches & (~RVV_OPT_THRESHOLD);
                int tmp_j = blkCnt;
                const q31_t *pA = ptr_a;
                const q31_t *pB = bi;
                q31_t *pOUT = output_temp;
                for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
                    a32m8 = __riscv_vle32_v_i32m8(pA, l);
                    pA += l;
                    b32m8 = __riscv_vle32_v_i32m8(pB, l);
                    pB += l;
                    __riscv_vse32_v_i32m8(pOUT, __riscv_vadd_vv_i32m8(a32m8, b32m8, l), l);
                    pOUT += l;
                }
                j = tmp_j;
#else
                j = 0;
#endif
                for (; j < feature_batches; j++)
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
                    int j;
#if defined(RISCV_MATH_VECTOR)
                    uint32_t blkCnt;                               /* Loop counter */
                    size_t l;
                    vint32m1_t v_temp;
                    vint32m8_t a32m8;
                    l = __riscv_vsetvl_e32m1(1);
                    v_temp = __riscv_vmv_v_x_i32m1(0, l);
                    blkCnt = rank & (~RVV_OPT_THRESHOLD);
                    int tmp_j = blkCnt;
                    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
                        a32m8 = __riscv_vle32_v_i32m8(ptr_a, l);
                        v_temp = __riscv_vredsum_vs_i32m8_i32m1(a32m8, v_temp, l);
                        ptr_a += l;
                    }
                    sum += __riscv_vmv_x_s_i32m1_i32(v_temp);
                    j = tmp_j;
#else
                    j = 0;
#endif
                    for (; j < rank; j++)
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
                int j;
#if defined(RISCV_MATH_VECTOR)
                uint32_t blkCnt;                               /* Loop counter */
                size_t l;
                vint32m1_t v_temp;
                vint32m8_t a32m8;
                l = __riscv_vsetvl_e32m1(1);
                v_temp = __riscv_vmv_v_x_i32m1(0, l);
                blkCnt = rank & (~RVV_OPT_THRESHOLD);
                int tmp_j = blkCnt;
                for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
                    a32m8 = __riscv_vle32_v_i32m8(ptr_a, l);
                    v_temp = __riscv_vredsum_vs_i32m8_i32m1(a32m8, v_temp, l);
                    ptr_a += l;
                }
                sum += __riscv_vmv_x_s_i32m1_i32(v_temp);
                j = tmp_j;
#else
                j = 0;
#endif
                for (; j < rank; j++)
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
