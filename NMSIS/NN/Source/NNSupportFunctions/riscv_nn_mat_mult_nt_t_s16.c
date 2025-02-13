/*
 * SPDX-FileCopyrightText: Copyright 2020-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_nn_mat_mult_nt_t_s16
 * Description:  Matrix multiplication support function with the right-hand-side (rhs) matrix transposed
 *
 * $Date:        11 April 2024
 * $Revision:    V.1.0.0
 *
 * Target : RISC-V Cores
 *
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
 * s16 matrix multiplication with the right-hand-side matrix transposed
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_mat_mult_nt_t_s16(const int16_t *lhs,
                                             const int8_t *rhs,
                                             const nmsis_nn_bias_data *bias_data,
                                             int16_t *dst,
                                             const int32_t *dst_multipliers,
                                             const int32_t *dst_shifts,
                                             const int32_t lhs_rows,
                                             const int32_t rhs_rows,
                                             const int32_t rhs_cols,
                                             const int32_t activation_min,
                                             const int32_t activation_max)
{
    (void)lhs;
    (void)rhs;
    (void)dst_multipliers;
    (void)dst_shifts;
    (void)dst;
    (void)activation_min;
    (void)activation_max;
    (void)bias_data;
    (void)lhs_rows;
    (void)lhs_rows;
    (void)rhs_rows;
    (void)rhs_cols;

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
