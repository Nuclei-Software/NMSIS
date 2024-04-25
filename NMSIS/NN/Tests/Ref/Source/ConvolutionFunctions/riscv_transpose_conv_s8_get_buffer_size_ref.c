/*
 * SPDX-FileCopyrightText: Copyright 2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_transpose_conv_s8_get_buffer_size_ref.c
 * Description:  Collection of get buffer size functions for the various s8 convolution layer functions.
 *
 * $Date:        5 October 2023
 * $Revision:    V.1.0.0
 *
 * Target :  RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"
#include "riscv_nnfunctions.h"

/**
 *  @ingroup NNConv
 */

/**
 * @addtogroup GetBufferSizeNNConv
 * @{
 */

/*
 * Get the required buffer size for riscv_transpose_conv_s8. This is the recommended transpose conv s8 get buffer size
 * function.
 *
 * Refer to header file for details.
 *
 */
int32_t riscv_transpose_conv_s8_get_buffer_size_ref(const nmsis_nn_dims *input_dims,
                                              const nmsis_nn_dims *filter_dims,
                                              const nmsis_nn_dims *out_dims)
{
    const int32_t size =
        filter_dims->w * filter_dims->h * input_dims->w * input_dims->h * out_dims->c * sizeof(int32_t);
    return size;
}

/**
 * @} end of GetBufferSizeNNConv group
 */
