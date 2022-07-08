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

    l = vsetvl_e32m1(1);
    vtemp = vmv_v_x_i32m1(0, l);
    for (; (l = vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l) {
        a0m4 = vle8_v_i8m4(input, l);
        input += l;
        vtemp = vwredsum_vs_i16m8_i32m1(vtemp, vwadd_vx_i16m8(a0m4, 0, l), vtemp, l);
    }
    result += vmv_x_s_i32m1_i32(vtemp);
    block_count = block_size & RVV_OPT_THRESHOLD;

#elif defined(RISCV_MATH_DSP)
    /* Loop unrolling: Compute 4 outputs at a time */
    block_count = block_size >> 2U;

    while (block_count > 0U)
    {
        const int32_t mult_q15x2 = (1UL << 16) | 1UL;
        q31_t in_q7x4 = riscv_nn_read_q7x4_ia(&input);
        q31_t temp_q15x2 = __SXTAB16(__SXTB16(in_q7x4), __ROR((uint32_t)in_q7x4, 8));

        result = __SMLAD(temp_q15x2, mult_q15x2, result);

        /* Decrement loop counter */
        block_count--;
    }

    /* Loop unrolling: Compute remaining outputs */
    block_count = block_size & 0x3;
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
