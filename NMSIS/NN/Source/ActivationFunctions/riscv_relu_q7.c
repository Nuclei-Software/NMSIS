/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        riscv_relu_q7.c
 * Description:  Q7 version of ReLU
 *
 * $Date:        20. July 2021
 * $Revision:    V.1.1.3
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
 * @addtogroup Acti
 * @{
 */

/**
 * @brief Q7 RELU function
 * @param[in,out]   data        pointer to input
 * @param[in]       size        number of elements
 *
 * @details
 *
 * Optimized relu with QSUB instructions.
 *
 */

void riscv_relu_q7(q7_t *data, uint16_t size)
{
    uint16_t i;

#if defined(RISCV_MATH_VECTOR)
    uint16_t blkCnt = size & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
    uint16_t tmp_i = blkCnt;
    size_t l;
    vint8m8_t vx;
    int8_t zero = 0;

    for (; (l = vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l) {
        vx = vle8_v_i8m8(data, l);
        /* if data >= zero, return data, else return zero */
        vse8_v_i8m8(data, vmax_vx_i8m8(vx, zero, l), l);
        data += l;
    }
    i = tmp_i;

#elif defined(RISCV_MATH_DSP)
    /* Run the following code for M cores with DSP extension */

    q7_t *input = data;
    q7_t *output = data;
    q31_t in;
    q31_t buf;
    q31_t mask;
    q31_t zero = 0;
#if __RISCV_XLEN == 64
    i = size >> 3;
    uint16_t tmp_i = i;
    q63_t in64;
    while (i)
    {

        in64 = *__SIMD64(input)++;

        *__SIMD64(output)++ = __RV_SMAX8 (in64, zero);

        i--;
    }

    i = tmp_i;
#else
    i = size >> 2;
    uint16_t tmp_i = size;
    while (i)
    {

        in = *__SIMD32(input)++;

        *__SIMD32(output)++ = __RV_SMAX8 (in, zero);

        i--;
    }

    i = tmp_i;
#endif /* __RISCV_XLEN == 64 */

#else
	i = 0;
#endif /* RISCV_MATH_VECTOR */
    /* Run the following code as reference implementation for cores without DSP extension */

    for (; i < size; i++)
    {
        if (data[i] < 0)
            data[i] = 0;
    }
}

/**
 * @} end of Acti group
 */
