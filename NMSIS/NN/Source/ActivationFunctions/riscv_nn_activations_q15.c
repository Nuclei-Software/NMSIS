/*
 * Copyright (C) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        riscv_nn_activations_q15.c
 * Description:  Q15 neural network activation function using direct table look-up
 *
 * $Date:        09. October 2020
 * $Revision:    V.1.0.1
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nn_tables.h"
#include "riscv_nnfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup Acti
 * @{
 */

/**
 * @brief neural network activation function using direct table look-up
 *
 * @note  Refer header file for details.
 *
 */

void riscv_nn_activations_direct_q15(q15_t *data, uint16_t size, uint16_t int_width, riscv_nn_activation_type type)
{
    uint16_t i = size;
    q15_t *pIn = data;
    q15_t *pOut = data;
    uint16_t shift_size = 8 + 3 - int_width;
    uint32_t bit_mask = 0x7FF >> int_width;
    uint32_t full_frac = bit_mask + 1;
    const q15_t *lookup_table;

    switch (type)
    {
    case RISCV_SIGMOID:
        lookup_table = sigmoidTable_q15;
        break;
    case RISCV_TANH:
    default:
        lookup_table = tanhTable_q15;
        break;
    }

#if defined(RISCV_MATH_VECTOR)
    uint16_t blkCnt = i;                               /* Loop counter */
    size_t l;
    vint8m2_t vxshit;
    vint16m4_t vx, val, val2;
    vint32m8_t valm8, val2m8, frac;
    vuint8m2_t bindex;
    vbool4_t mask;
    for (; (l = vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l) {
        vx = vle16_v_i16m4(pIn, l);
        pIn += l;
        vxshit = vnsra_wx_i8m2(vx, shift_size, l);
        bindex = vreinterpret_v_i8m2_u8m2(vxshit);
        vloxseg2ei16_v_i16m4 (&val, &val2, lookup_table, vwmulu_vx_u16m4(bindex, 2, l), l);
        frac = vand_vx_i32m8(vwadd_vx_i32m8(vx, 0, l), (int32_t)bit_mask, l);
        valm8 = vmul_vv_i32m8(vwadd_vx_i32m8(val, 0, l), vrsub_vx_i32m8(frac, (int32_t)full_frac, l), l);
        val2m8 = vmul_vv_i32m8(vwadd_vx_i32m8(val2, 0, l), frac, l);
        val2 = vnsra_wx_i16m4(vadd_vv_i32m8(valm8, val2m8, l), shift_size, l);
        mask = vmseq_vx_i8m2_b4(vxshit, 0x7f, l);
        vx = vmerge_vvm_i16m4 (mask, val2, val, l);
        vse16_v_i16m4(pOut, vx, l);
        pOut += l;
    }
#else
    while (i)
    {
        q15_t out;
        q15_t in = *pIn++;
        q15_t frac = (uint32_t)in & bit_mask;
        q15_t value = lookup_table[(uint8_t)(in >> shift_size)];
        if ((in >> shift_size) != 0x7f)
        {
            q15_t value2 = lookup_table[(uint8_t)(1 + ((uint8_t)(in >> shift_size)))];
            /* doing the interpolation here for better accuracy */
            out = ((q31_t)(full_frac - frac) * value + (q31_t)value2 * frac) >> shift_size;
        }
        else
        {
            /* the largest positive value does not have a right side for linear interpolation */
            out = value;
        }

        *pOut++ = out;
        i--;
    }
#endif /* #if defined(RISCV_MATH_VECTOR) */
}

/**
 * @} end of Acti group
 */
