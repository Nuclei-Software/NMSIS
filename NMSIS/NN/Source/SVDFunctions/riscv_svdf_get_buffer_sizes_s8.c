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
 * Title:        riscv_svdf_get_buffer_sizes_s8.c
 * Description:  Collection of get buffer size functions for svdf s8 layer function.
 *
 * $Date:        5 September 2023
 * $Revision:    V.1.0.0
 *
 * Target : RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"

/**
 *  @ingroup SVDF
 */

/**
 * @addtogroup GetBufferSizeSVDF
 * @{
 */

int32_t riscv_svdf_s8_get_buffer_size_dsp(const nmsis_nn_dims *weights_feature_dims)
{
    (void)weights_feature_dims;
    return 0;
}

int32_t riscv_svdf_s8_get_buffer_size_mve(const nmsis_nn_dims *weights_feature_dims)
{
    return weights_feature_dims->n * sizeof(int32_t);
}

int32_t riscv_svdf_s8_get_buffer_size(const nmsis_nn_dims *weights_feature_dims)
{
    return riscv_svdf_s8_get_buffer_size_dsp(weights_feature_dims);
}

/**
 * @} end of GetBufferSizeSVDF group
 */
