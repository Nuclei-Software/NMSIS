/*
 * Copyright (C) 2022 Arm Limited or its affiliates.
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
 * Title:        riscv_elementwise_add_s16
 * Description:  Elementwise add
 *
 * $Date:        14 Februari 2022
 * $Revision:    V.1.0.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup BasicMath
 * @{
 */

/*
 * s16 elementwise add
 *
 * Refer header file for details.
 *
 */

/* Note: __SHIFT is expected to be <=0 */

riscv_nmsis_nn_status riscv_elementwise_add_s16(const int16_t *input_1_vect,
                                   const int16_t *input_2_vect,
                                   const int32_t input_1_offset,
                                   const int32_t input_1_mult,
                                   const int32_t input_1_shift,
                                   const int32_t input_2_offset,
                                   const int32_t input_2_mult,
                                   const int32_t input_2_shift,
                                   const int32_t left_shift,
                                   int16_t *output,
                                   const int32_t out_offset,
                                   const int32_t out_mult,
                                   const int32_t out_shift,
                                   const int32_t out_activation_min,
                                   const int32_t out_activation_max,
                                   const int32_t block_size)
{
    (void)input_1_offset;
    (void)input_2_offset;
    (void)out_offset;
    int32_t loop_count;
    int32_t input_1;
    int32_t input_2;
    int32_t sum;

#if defined(RISCV_MATH_VECTOR)
    int32_t blkCnt = block_size & (~RVV_OPT_THRESHOLD); /* Loop counter */
    size_t l;
    vint32m4_t input_1_m4;
    vint32m4_t input_2_m4;
    vint32m4_t sum0m4;

    for (; (l = vsetvl_e16m2(blkCnt)) > 0; blkCnt -= l) {
        input_1_m4 = vsll_vx_i32m4(vwadd_vx_i32m4(vle16_v_i16m2(input_1_vect, l), 0, l), left_shift, l);
        input_1_vect += l;
        input_2_m4 = vsll_vx_i32m4(vwadd_vx_i32m4(vle16_v_i16m2(input_2_vect, l), 0, l), left_shift, l);
        input_2_vect += l;

        /* input_1 = riscv_nn_requantize(input_1, input_1_mult, input_1_shift); */
        input_1_m4 = riscv_nn_requantize_m4_rvv(input_1_m4, l, input_1_mult, input_1_shift);

        /* input_2 = riscv_nn_requantize(input_2, input_2_mult, input_2_shift); */
        input_2_m4 = riscv_nn_requantize_m4_rvv(input_2_m4, l, input_2_mult, input_2_shift);

        sum0m4 = vadd_vv_i32m4(input_1_m4, input_2_m4, l);
        /* sum = riscv_nn_requantize(sum, out_mult, out_shift); */
        sum0m4 = riscv_nn_requantize_m4_rvv(sum0m4, l, out_mult, out_shift);

        sum0m4 = vmax_vx_i32m4(sum0m4, out_activation_min, l);
        sum0m4 = vmin_vx_i32m4(sum0m4, out_activation_max, l);
        vse16_v_i16m2(output, vnsra_wx_i16m2(sum0m4, 0, l), l);
        output += l;
    }
    loop_count = block_size & RVV_OPT_THRESHOLD;
#else
    loop_count = block_size;
#endif /* defined(RISCV_MATH_VECTOR) */

    while (loop_count > 0)
    {
        /* C = A + B */
        input_1 = *input_1_vect++ << left_shift;
        input_2 = *input_2_vect++ << left_shift;

        input_1 = riscv_nn_requantize(input_1, input_1_mult, input_1_shift);
        input_2 = riscv_nn_requantize(input_2, input_2_mult, input_2_shift);

        sum = input_1 + input_2;
        sum = riscv_nn_requantize(sum, out_mult, out_shift);

        sum = MAX(sum, out_activation_min);
        sum = MIN(sum, out_activation_max);

        *output++ = (int16_t)sum;

        /* Decrement loop counter */
        loop_count--;
    }

    return (RISCV_NMSIS_NN_SUCCESS);
}

/**
 * @} end of BasicMath group
 */
