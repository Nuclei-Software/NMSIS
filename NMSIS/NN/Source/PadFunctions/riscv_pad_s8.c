
/*
 * SPDX-FileCopyrightText: Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2024 Nuclei Limited. All rights reserved.
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
 * Title:        riscv_pad_s8.c
 * Description:  Pad a s8 vector
 *
 * $Date:        19 Sep 2024
 * $Revision:    V.1.0.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nn_types.h"
#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"
/**
 *  @ingroup Public
 */

/**
 * @addtogroup Pad
 * @{
 */

/*
 * Basic s8 pad function.
 *
 * Refer header file for details.
 *
 */

riscv_nmsis_nn_status riscv_pad_s8(const int8_t *input,
                               int8_t *output,
                               const int8_t pad_value,
                               const nmsis_nn_dims *input_size,
                               const nmsis_nn_dims *pre_pad,
                               const nmsis_nn_dims *post_pad)
{

    const nmsis_nn_dims output_size = {pre_pad->n + input_size->n + post_pad->n,
                                       pre_pad->h + input_size->h + post_pad->h,
                                       pre_pad->w + input_size->w + post_pad->w,
                                       pre_pad->c + input_size->c + post_pad->c};

    const int32_t batch_block_size = output_size.h * output_size.w * output_size.c;
    const int32_t row_block_size = output_size.w * output_size.c;
    const int32_t col_block_size = output_size.c;

    riscv_memset_s8(output, pad_value, batch_block_size * pre_pad->n);
    output += batch_block_size * pre_pad->n;
    for (int32_t b = 0; b < input_size->n; b++)
    {

        riscv_memset_s8(output, pad_value, row_block_size * pre_pad->h);
        output += row_block_size * pre_pad->h;
        for (int32_t y = 0; y < input_size->h; y++)
        {

            riscv_memset_s8(output, pad_value, col_block_size * pre_pad->w);
            output += col_block_size * pre_pad->w;
            if (input_size->c == output_size.c)
            {
                riscv_memcpy_s8(output, input, input_size->w * input_size->c);
                output += input_size->w * input_size->c;
                input += input_size->w * input_size->c;
            }
            else
            {
                for (int32_t x = 0; x < input_size->w; x++)
                {

                    riscv_memset_s8(output, pad_value, pre_pad->c);
                    output += pre_pad->c;

                    riscv_memcpy_s8(output, input, input_size->c);
                    output += input_size->c;
                    input += input_size->c;

                    riscv_memset_s8(output, pad_value, post_pad->c);
                    output += post_pad->c;
                }
            }

            riscv_memset_s8(output, pad_value, col_block_size * post_pad->w);
            output += col_block_size * post_pad->w;
        }

        riscv_memset_s8(output, pad_value, row_block_size * post_pad->h);
        output += row_block_size * post_pad->h;
    }
    riscv_memset_s8(output, pad_value, batch_block_size * post_pad->n);

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Pad group
 */
