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
 * Target : RISC-V Cores
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
#if defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)
    int64_t in_s8x8;
    int64_t in_s16x4_1;
    int64_t in_s16x4_2;
    int32_t block_cnt = block_size >> 3;

    /* Compute 4 outputs at a time. */
    const int32_t offset_s16x2 = __PKHBT(offset, offset, 16);
#if __RISCV_XLEN == 64
    int64_t offset_s16x4 = __RV_PKBB32(offset_s16x2, offset_s16x2);
#else
    int64_t offset_s16x4 = __RV_DPACK32(offset_s16x2, offset_s16x2);
#endif /* __RISCV_XLEN == 64 */
#else
    int32_t in_s8x4;
    int32_t in_s16x2_1;
    int32_t in_s16x2_2;
    int32_t block_cnt = block_size >> 2;

    /* Compute 4 outputs at a time. */
    const int32_t offset_s16x2 = __PKHBT(offset, offset, 16);
#endif /* defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64) */

    while (block_cnt > 0)
    {
#if __RISCV_XLEN == 64
        in_s8x8 = riscv_nn_read_s8x8_ia(&src);
        in_s16x4_1 = __SXTAB16(offset_s16x4, in_s8x8);
        in_s16x4_2 = __SXTAB16(offset_s16x4, __ROR64(in_s8x8, 8));
        riscv_nn_write_s16x4_ia(&dst, __RV_PKBB32(in_s16x4_2, in_s16x4_1));
        riscv_nn_write_s16x4_ia(&dst, __RV_PKTT32(in_s16x4_2, in_s16x4_1));
#else
#if defined (NUCLEI_DSP_N2)
        in_s8x8 = riscv_nn_read_s8x8_ia(&src);
        in_s16x4_1 = __RV_DADD16(offset_s16x4, __RV_DSUNPKD820(in_s8x8));
        in_s16x4_2 = __RV_DADD16(offset_s16x4, __RV_DSUNPKD820(__ROR64(in_s8x8, 8)));
        riscv_nn_write_s16x4_ia(&dst, __RV_DPKBB32(in_s16x4_2, in_s16x4_1));
        riscv_nn_write_s16x4_ia(&dst, __RV_DPKTT32(in_s16x4_2, in_s16x4_1));
#else
        in_s8x4 = riscv_nn_read_s8x4_ia(&src);

        in_s16x2_1 = __SXTAB16(offset_s16x2, in_s8x4);
        in_s16x2_2 = __SXTAB16(offset_s16x2, __ROR(in_s8x4, 8));

        riscv_nn_write_s16x2_ia(&dst, in_s16x2_1);
        riscv_nn_write_s16x2_ia(&dst, in_s16x2_2);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

        block_cnt--;
    }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
    /* Handle left over samples. */
    block_cnt = block_size & 7;
#else
    /* Handle left over samples. */
    block_cnt = block_size & 3;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
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
