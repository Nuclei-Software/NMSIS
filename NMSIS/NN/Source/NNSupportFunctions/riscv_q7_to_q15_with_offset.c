/*
 * SPDX-FileCopyrightText: Copyright 2010-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
 * Title:        riscv_q7_to_q15_with_offset.c
 * Description:  Converts the elements of the Q7 vector to Q15 vector with an added offset
 *
 * $Date:        22 March 2023
 * $Revision:    V.2.2.0
 *
 * Target Processor: RISC-V Cores
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

void riscv_q7_to_q15_with_offset(const int8_t *src, int16_t *dst, int32_t block_size, int16_t offset)
{
    int32_t block_cnt;
#if defined(RISCV_MATH_VECTOR)
    block_cnt = block_size & (~RVV_OPT_THRESHOLD);                              /* Loop counter */
    size_t l;

    for (; (l = __riscv_vsetvl_e8m4(block_cnt)) > 0; block_cnt -= l) {
        __riscv_vse16_v_i16m8(dst, __riscv_vadd_vx_i16m8(__riscv_vwadd_vx_i16m8(__riscv_vle8_v_i8m4(src, l), 0, l), offset, l), l);
        src += l;
        dst += l;
    }
    block_cnt = block_size & RVV_OPT_THRESHOLD;
#elif defined(RISCV_MATH_DSP)
    /* Run the below code for cores that support SIMD instructions  */
#if defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)
    q63_t in_q15x4_1;
    q63_t in_q15x4_2;
    q63_t out_q15x4_1;
    q63_t out_q15x4_2;

    /*loop unrolling */
    block_cnt = block_size >> 3u;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time. */
    const int32_t offset_q15x2 = __NN_PKHBT(offset, offset, 16);
#if (__RISCV_XLEN == 64)
    const q63_t offset_q15x4 = __RV_PKBB32(offset_q15x2, offset_q15x2);
#else
    const q63_t offset_q15x4 = __RV_DPACK32(offset_q15x2, offset_q15x2);
#endif /* (__RISCV_XLEN == 64) */
#else
    int32_t in_q7x4;
    int32_t in_q15x2_1;
    int32_t in_q15x2_2;
    int32_t out_q15x2_1;
    int32_t out_q15x2_2;

    /*loop unrolling */
    block_cnt = block_size >> 2;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time. */
    const int32_t offset_q15x2 = __NN_PKHBT(offset, offset, 16);

#endif /* defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64) */

    while (block_cnt > 0)
    {
#if (__RISCV_XLEN == 64)
        q63_t inA = riscv_nn_read_s8x8_ia(&src);
        in_q15x4_1 = __SXTAB16(offset_q15x4, __ROR64((uint64_t)inA, 8));
        in_q15x4_2 = __SXTAB16(offset_q15x4, inA);

        out_q15x4_2 = __RV_PKTT16(in_q15x4_1, in_q15x4_2);
        out_q15x4_1 = __RV_PKBB16(in_q15x4_1, in_q15x4_2);

        riscv_nn_write_q15x4_ia(&dst, __RV_PKBB32(out_q15x4_2, out_q15x4_1));
        riscv_nn_write_q15x4_ia(&dst, __RV_PKTT32(out_q15x4_2, out_q15x4_1));
#else
#if defined (NUCLEI_DSP_N2)
        q63_t inA = riscv_nn_read_s8x8_ia(&src);
        in_q15x4_1 = __RV_DADD16(offset_q15x4, __RV_DSUNPKD820(__ROR64((uint64_t)inA, 8)));
        in_q15x4_2 = __RV_DADD16(offset_q15x4, __RV_DSUNPKD820(inA));

        out_q15x4_2 = __RV_DPKTT16(in_q15x4_1, in_q15x4_2);
        out_q15x4_1 = __RV_DPKBB16(in_q15x4_1, in_q15x4_2);

        riscv_nn_write_q15x4_ia(&dst, __RV_DPKBB32(out_q15x4_2, out_q15x4_1));
        riscv_nn_write_q15x4_ia(&dst, __RV_DPKTT32(out_q15x4_2, out_q15x4_1));
#else
        /* convert from s8 to s16 and then store the results in the destination buffer */
        in_q7x4 = riscv_nn_read_s8x4_ia(&src);

        /* Extract and sign extend each of the four s8 values to s16 */
        in_q15x2_1 = __SXTAB16(offset_q15x2, __ROR(in_q7x4, 8));
        in_q15x2_2 = __SXTAB16(offset_q15x2, in_q7x4);

        out_q15x2_2 = __NN_PKHTB(in_q15x2_1, in_q15x2_2, 16);
        out_q15x2_1 = __NN_PKHBT(in_q15x2_2, in_q15x2_1, 16);

        riscv_nn_write_q15x2_ia(&dst, out_q15x2_1);
        riscv_nn_write_q15x2_ia(&dst, out_q15x2_2);
#endif /* defined (NUCLEI_DSP_N2) */
#endif /* (__RISCV_XLEN == 64) */

        block_cnt--;
    }
#if defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)
    /* Handle left over samples */
    block_cnt = block_size & 0x7;
#else
    /* Handle left over samples */
    block_cnt = block_size & 0x3;
#endif /* defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64) */

#else
    /* Run the below code for RISC-V Core without DSP */
    /* Loop over block_size number of values */
    block_cnt = block_size;
#endif

    while (block_cnt > 0)
    {
        *dst++ = (int16_t)*src++ + offset;

        /* Decrement the loop counter */
        block_cnt--;
    }
}

/**
 * @} end of Doxygen group
 */
