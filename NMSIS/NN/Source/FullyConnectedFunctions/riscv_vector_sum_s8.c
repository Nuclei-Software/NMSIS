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
 * Title:        riscv_vector_sum_s8
 * Description:  Generic function for calculating vector sums
 *
 * $Date:        05 Sep 2024
 * $Revision:    V.3.0.0
 *
 * Target :  RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"
/**
 *  @ingroup Public
 */

/**
 * @addtogroup FC
 * @{
 */

/*
 * S8 vector sum fuction in preparation for e.g. kernel sums in fully connected and matrix multiplication layer function
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_vector_sum_s8(int32_t *vector_sum_buf,
                                      const int32_t vector_cols,
                                      const int32_t vector_rows,
                                      const int8_t *vector_data,
                                      const int32_t lhs_offset,
                                      const int32_t rhs_offset,
                                      const int32_t *bias_data)
{

    if (bias_data)
    {
        memcpy(vector_sum_buf, bias_data, vector_rows * sizeof(int32_t));
    }
    else
    {
        memset(vector_sum_buf, 0, vector_rows * sizeof(int32_t));
    }

    if (lhs_offset)
    {
        for (int i = 0; i < vector_rows; i++)
        {
            int32_t sum = 0;
            for (int j = 0; j < vector_cols; j++)
            {
                sum += *vector_data++;
            }
            if (rhs_offset)
            {
                sum += vector_cols * rhs_offset;
            }
            *vector_sum_buf++ += sum * lhs_offset;
        }
    }

    return (RISCV_NMSIS_NN_SUCCESS);
}

/**
 * @} end of FC group
 */
