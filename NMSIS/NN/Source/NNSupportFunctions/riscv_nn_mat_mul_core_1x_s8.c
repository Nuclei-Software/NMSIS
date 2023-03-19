/*
 * Copyright (C) 2010-2022 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        riscv_nn_mat_mul_core_1x_s8.c
 * Description:  General Matrix-multiplication function
 *
 * $Date:        19. April 2022
 * $Revision:    V.1.0.3
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup NNBasicMath
 * @{
 */

/*
 * s8 matrix multiplication to process 1 row
 *
 * Refer header file for details.
 *
 */

riscv_nmsis_nn_status riscv_nn_mat_mul_core_1x_s8(int32_t row_elements,
                                     const int8_t *row_base,
                                     const int8_t *col_base,
                                     int32_t *const sum_col,
                                     int32_t *const output)
{
    int32_t acc_n0 = 0;
    int32_t sum_tmp = 0;
    int i;

#if defined (RISCV_MATH_VECTOR)
    uint32_t blkCnt = row_elements & (~RVV_OPT_THRESHOLD);                              /* Loop counter */
    uint32_t temp_i = blkCnt;
    size_t l;
    vint8m4_t v_col, v_row;
    const int8_t *p_row = row_base;
    const int8_t *p_col = col_base;
    vint32m1_t vtemp1, vtemp2;
    l = vsetvl_e32m1(1);
    vtemp1 = vsub_vv_i32m1(vtemp1, vtemp1, l);
    vtemp2 = vsub_vv_i32m1(vtemp2, vtemp2, l);
    for (; (l = vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
        v_col = vle8_v_i8m4(p_col, l);
        v_row = vle8_v_i8m4(p_row, l);
        p_col += l;
        p_row += l;
        vtemp1 = vwredsum_vs_i16m8_i32m1(vtemp1, vwadd_vx_i16m8(v_col, 0, l), vtemp1, l);
        vtemp2 = vwredsum_vs_i16m8_i32m1(vtemp2, vwmul_vv_i16m8(v_col, v_row, l), vtemp2, l);
    }
    sum_tmp += vmv_x_s_i32m1_i32(vtemp1);
    acc_n0 += vmv_x_s_i32m1_i32(vtemp2);
    i = temp_i;
#else
    i = 0;
#endif
    for (; i < row_elements; i++)
    {
        sum_tmp += col_base[i];
        acc_n0 += row_base[i] * col_base[i];
    }

    *sum_col = sum_tmp;
    *output = acc_n0;
    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of NNBasicMath group
 */
