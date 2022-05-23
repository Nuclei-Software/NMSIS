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
 * Title:        riscv_elementwise_mul_s16
 * Description:  Element wise multiplication
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

/**
 * @brief s16 element wise multiplication of two vectors
 *
 * @note   Refer header file for details.
 *
 */
riscv_status riscv_elementwise_mul_s16(const int16_t *input_1_vect,
                                   const int16_t *input_2_vect,
                                   const int32_t input_1_offset,
                                   const int32_t input_2_offset,
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
    int32_t mul_res;
#if defined(RISCV_MATH_VECTOR)
    int32_t blkCnt = block_size & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
    size_t l;
    vint32m8_t in1m8;
    vint32m8_t in2m8;
    vint32m8_t sum;

    for (; (l = vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l) {
        in1m8 = vwadd_vx_i32m8(vle16_v_i16m4(input_1_vect, l), 0, l);
        input_1_vect += l;
        in2m8 = vwadd_vx_i32m8(vle16_v_i16m4(input_2_vect, l), 0, l);
        input_2_vect += l;

        sum = vmul_vv_i32m8(in1m8, in2m8, l);
        if (out_shift < 0) {
            sum = vsra_vx_i32m8(vsmul_vx_i32m8(sum, out_mult, l), -out_shift, l);
        } else {
            sum = vsll_vx_i32m8(vsmul_vx_i32m8(sum, out_mult, l), out_shift, l);
        }

        sum = vmax_vx_i32m8(sum, out_activation_min, l);
        sum = vmin_vx_i32m8(sum, out_activation_max, l);
        vse16_v_i16m4(output, vnsra_wx_i16m4(sum, 0, l), l);
        output += l;
    }
    loop_count = block_size & RVV_OPT_THRESHOLD;
#else
    loop_count = block_size;
#endif

    while (loop_count > 0)
    {
        /* C = A * B */

        input_1 = *input_1_vect++;
        input_2 = *input_2_vect++;

        mul_res = input_1 * input_2;
        mul_res = riscv_nn_requantize(mul_res, out_mult, out_shift);

        mul_res = MAX(mul_res, out_activation_min);
        mul_res = MIN(mul_res, out_activation_max);

        *output++ = (int16_t)mul_res;

        /* Decrement loop counter */
        loop_count--;
    }

    return RISCV_MATH_SUCCESS;
}

/**
 * @} end of BasicMath group
 */
