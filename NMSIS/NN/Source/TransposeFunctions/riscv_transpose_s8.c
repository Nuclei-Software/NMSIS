/*
 * SPDX-FileCopyrightText: Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_transpose_s8.c
 * Description:  Transpose a s8 vector
 *
 * $Date:        30 October 2024
 * $Revision:    V.1.0.1
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
 * @addtogroup Transpose
 * @{
 */

static riscv_nmsis_nn_status riscv_transpose_s8_nhcw(const int8_t *input,
                                                 int8_t *const output,
                                                 const nmsis_nn_dims *const input_dims,
                                                 const int32_t *const in_strides,
                                                 const int32_t *const out_strides)
{
    const int32_t n = input_dims->n;
    const int32_t h = input_dims->h;
    const int32_t w = input_dims->w;
    const int32_t c = input_dims->c;

    const int8_t *input_n = input;
    int8_t *output_n = output;

    const uint16_t src_rows = w;
    const uint16_t src_cols = c;

    for (int32_t i = 0; i < n; i++)
    {
        const int8_t *input_h = input_n;
        int8_t *output_h = output_n;

        for (int32_t y = 0; y < h; y++)
        {
            const uint8_t *input_w = (const uint8_t *)input_h;
            uint8_t *output_w = (uint8_t *)output_h;

            for (int32_t src_row_i = 0; src_row_i < src_rows; src_row_i++)
            {
                output_w = (uint8_t *)output + src_row_i;

                for (int32_t x = 0; x < src_cols; x++)
                {
                    *output_w = *input_w++;
                    output_w += src_rows;
                }
            }
            input_h += in_strides[1];
            output_h += out_strides[1];
        }
        input_n += in_strides[0];
        output_n += out_strides[0];
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

static riscv_nmsis_nn_status riscv_transpose_s8_default(const int8_t *input,
                                                    int8_t *const output,
                                                    const nmsis_nn_dims *const input_dims,
                                                    const int32_t *const in_strides,
                                                    const int32_t *const out_strides)
{
    const int32_t n = input_dims->n;
    const int32_t h = input_dims->h;
    const int32_t w = input_dims->w;
    const int32_t c = input_dims->c;

    for (int32_t i = 0; i < n; i++)
    {
        for (int32_t y = 0; y < h; y++)
        {
            for (int32_t x = 0; x < w; x++)
            {
                for (int32_t z = 0; z < c; z++)
                {
                    const int32_t from_index =
                        i * in_strides[0] + y * in_strides[1] + x * in_strides[2] + z * in_strides[3];

                    const int32_t to_index =
                        i * out_strides[0] + y * out_strides[1] + x * out_strides[2] + z * out_strides[3];

                    output[to_index] = input[from_index];
                }
            }
        }
    }

    return RISCV_NMSIS_NN_SUCCESS;
}

/*
 * Basic s8 transpose function.
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_transpose_s8(const int8_t *input,
                                     int8_t *const output,
                                     const nmsis_nn_dims *const input_dims,
                                     const nmsis_nn_dims *const output_dims,
                                     const nmsis_nn_transpose_params *const transpose_params)
{
    int32_t in_strides[4];
    int32_t out_strides[4] = {0};

    const uint32_t *const perm = transpose_params->permutations;

    const int32_t n = input_dims->n;
    const int32_t h = input_dims->h;
    const int32_t w = input_dims->w;
    const int32_t c = input_dims->c;

    in_strides[0] = h * w * c;
    in_strides[1] = w * c;
    in_strides[2] = c;
    in_strides[3] = 1;

    if (transpose_params->num_dims == 1)
    {
        riscv_memcpy_s8(output, input, input_dims->n);

        return RISCV_NMSIS_NN_SUCCESS;
    }
    else if (transpose_params->num_dims == 2)
    {
        const nmsis_nn_dims smaller_input_dims = {1, 1, n, h};

        return riscv_transpose_s8_nhcw(input, output, &smaller_input_dims, in_strides, out_strides);
    }
    else if (transpose_params->num_dims == 3)
    {
        const nmsis_nn_dims smaller_input_dims = {1, n, h, w};

        in_strides[0] = 0;
        in_strides[1] = h * w;
        in_strides[2] = w;
        in_strides[3] = 1;

        if (perm[0] > 2 || perm[1] > 2 || perm[2] > 2)
        {
            return RISCV_NMSIS_NN_ARG_ERROR;
        }

        out_strides[0] = 0;
        out_strides[perm[0] + 1] = output_dims->h * output_dims->w;
        out_strides[perm[1] + 1] = output_dims->w;
        out_strides[perm[2] + 1] = 1;

        return riscv_transpose_s8_default(input, output, &smaller_input_dims, in_strides, out_strides);
    }

    if (perm[0] > 3 || perm[1] > 3 || perm[2] > 3 || perm[3] > 3)
    {
        return RISCV_NMSIS_NN_ARG_ERROR;
    }

    out_strides[perm[0]] = output_dims->h * output_dims->w * output_dims->c;
    out_strides[perm[1]] = output_dims->w * output_dims->c;
    out_strides[perm[2]] = output_dims->c;
    out_strides[perm[3]] = 1;

    return riscv_transpose_s8_default(input, output, input_dims, in_strides, out_strides);
}

/**
 * @} end of Transpose group
 */
