
/*
 * SPDX-FileCopyrightText: Copyright 2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in_q7x4 compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in_q7x4 writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_s8_to_s16_unordered_with_offset.c
 * Description:  Converts the elements of the S8 vector to S16 vector with an added offset
 *
 * $Date:        23 Mars 2023
 * $Revision:    V.1.0.0
 *
 * Target :  RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportConversion
 * @{
 */
#if defined(RISCV_MATH_DSP)
void riscv_s8_to_s16_unordered_with_offset(const int8_t *src, int16_t *dst, int32_t block_size, int16_t offset)
{
    int32_t in_s8x4;
    int32_t in_s16x2_1;
    int32_t in_s16x2_2;
    int32_t block_cnt = block_size >> 2;

    /* Compute 4 outputs at a time. */
    const int32_t offset_s16x2 = __PKHBT(offset, offset, 16);
    while (block_cnt > 0)
    {
        in_s8x4 = riscv_nn_read_s8x4_ia(&src);

        in_s16x2_1 = __SXTAB16(offset_s16x2, in_s8x4);
        in_s16x2_2 = __SXTAB16(offset_s16x2, __ROR(in_s8x4, 8));

        riscv_nn_write_q15x2_ia(&dst, in_s16x2_1);
        riscv_nn_write_q15x2_ia(&dst, in_s16x2_2);

        block_cnt--;
    }

    /* Handle left over samples. */
    block_cnt = block_size % 4;
    while (block_cnt > 0)
    {
        *dst++ = (int16_t)*src++ + offset;
        block_cnt--;
    }
}
#endif

/**
 * @} end of Doxygen group
 */
