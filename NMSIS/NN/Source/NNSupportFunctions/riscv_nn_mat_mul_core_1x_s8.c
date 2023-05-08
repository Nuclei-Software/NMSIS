/*
 * SPDX-FileCopyrightText: Copyright 2010-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
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
 * Title:        riscv_nn_mat_mul_core_1x_s8.c
 * Description:  General Matrix-multiplication function
 *
 * $Date:        20 January 2023
 * $Revision:    V.3.1.3
 *
 * Target :  RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConvolution
 * @{
 */

/*
 * s8 matrix multiplication to process 1 row
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_mat_mul_core_1x_s8(int32_t row_elements,
                                              const int32_t skipped_row_elements,
                                              const int8_t *row_base_ref,
                                              const int8_t *col_base_ref,
                                              const int32_t out_ch,
                                              const nmsis_nn_conv_params *conv_params,
                                              const nmsis_nn_per_channel_quant_params *quant_params,
                                              const int32_t *bias,
                                              int8_t *output)
{
    (void)row_elements;
    (void)skipped_row_elements;
    (void)row_base_ref;
    (void)col_base_ref;
    (void)out_ch;
    (void)conv_params;
    (void)quant_params;
    (void)bias;
    (void)output;
    return RISCV_NMSIS_NN_NO_IMPL_ERROR;
}

/**
 * @} end of supportConvolution group
 */
