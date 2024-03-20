/*
 * Copyright (C) 2010-2018 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        riscv_nn_add_q7.c
 * Description:  Non saturating addition of elements of a q7 vector.
 *
 * $Date:        20. July 2021
 * $Revision:    V.1.1.1
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nn_tables.h"
#include "riscv_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup NNBasicMath
 * @{
 */

void riscv_nn_add_q7(const q7_t *input, q31_t *output, uint32_t block_size)
{
    uint32_t block_count;
    q31_t result = 0;

#if defined (RISCV_MATH_VECTOR)
    uint32_t blkCnt = block_size & (~RVV_OPT_THRESHOLD);                              /* Loop counter */
    size_t l;
    vint8m4_t a0m4;
    vint32m1_t vtemp;

    l = __riscv_vsetvl_e32m1(1);
    vtemp = __riscv_vmv_v_x_i32m1(0, l);
    for (; (l = __riscv_vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
        a0m4 = __riscv_vle8_v_i8m4(input, l);
        input += l;
        vtemp = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwadd_vx_i16m8(a0m4, 0, l), vtemp, l);
    }
    result += __riscv_vmv_x_s_i32m1_i32(vtemp);
    block_count = block_size & RVV_OPT_THRESHOLD;

#elif defined(RISCV_MATH_DSP)
    /* Loop unrolling: Compute 4 outputs at a time */
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
    block_count = block_size >> 3U;
    q63_t tmp_calc = 0x0101010101010101;
    q63_t in64, out64 = 0;
#else
    block_count = block_size >> 2U;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

    while (block_count > 0U)
    {
#if __RISCV_XLEN == 64
       in64 = riscv_nn_read_q7x8_ia((q7_t **)&input);
       out64 = __RV_SMAQA(out64, in64, tmp_calc);
#else
#if defined (NUCLEI_DSP_N3)
       in64 = riscv_nn_read_q7x8_ia((q7_t **)&input);
       out64 = __RV_DDSMAQA(out64, in64, tmp_calc);
#else
        const int32_t mult_q15x2 = (1UL << 16) | 1UL;
        q31_t in_q7x4 = riscv_nn_read_q7x4_ia(&input);
        q31_t temp_q15x2 = __SXTAB16(__SXTB16(in_q7x4), __ROR((uint32_t)in_q7x4, 8));

        result = __SMLAD(temp_q15x2, mult_q15x2, result);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

        /* Decrement loop counter */
        block_count--;
    }

    /* Loop unrolling: Compute remaining outputs */
#if __RISCV_XLEN == 64
    result = (q31_t)out64 + (q31_t)(out64 >> 32);
    block_count = block_size & 0x7;
#else
#if defined (NUCLEI_DSP_N3)
    result = (q31_t)out64 ;
    block_count = block_size & 0x7;
#else
    block_count = block_size & 0x3;
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
    block_count = block_size;
#endif
    while (block_count > 0U)
    {
        /* Add and store result in destination buffer. */
        result += *input++;

        /* Decrement loop counter */
        block_count--;
    }

    *output = result;
}

/**
 * @} end of NNBasicMath group
 */
