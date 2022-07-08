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
 * Title:        riscv_fully_connected_q7.c
 * Description:  Q7 basic fully-connected layer function
 *
 * $Date:        July 20, 2021
 * $Revision:    V.1.1.2
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
 * @addtogroup FC
 * @{
 */

/**
 * @brief Q7 basic fully-connected layer function
 * @param[in]       pV          pointer to input vector
 * @param[in]       pM          pointer to matrix weights
 * @param[in]       dim_vec     length of the vector
 * @param[in]       num_of_rows number of rows in weight matrix
 * @param[in]       bias_shift  amount of left-shift for bias
 * @param[in]       out_shift   amount of right-shift for output
 * @param[in]       bias        pointer to bias
 * @param[in,out]   pOut        pointer to output vector
 * @param[in,out]   vec_buffer  pointer to buffer space for input
 * @return     The function returns <code>RISCV_MATH_SUCCESS</code>
 *
 * @details
 *
 * <b>Buffer size:</b>
 *
 * vec_buffer size: dim_vec
 *
 * This basic function is designed to work with regular weight
 * matrix without interleaving.
 *
 */

riscv_status riscv_fully_connected_q7(const q7_t *pV,
                                  const q7_t *pM,
                                  const uint16_t dim_vec,
                                  const uint16_t num_of_rows,
                                  const uint16_t bias_shift,
                                  const uint16_t out_shift,
                                  const q7_t *bias,
                                  q7_t *pOut,
                                  q15_t *vec_buffer)
{
#if defined (RISCV_MATH_VECTOR)
    (void)vec_buffer;
    int i, j;
    uint16_t rowCnt = num_of_rows / 0x3;
    q7_t *pO = pOut;

    q31_t sum, sum2, sum3;
    const q7_t *pA, *pB, *pB2, *pB3;
    int32_t blkCnt;
    vint8m4_t a8m4, b8m4, c8m4, d8m4;
    vint32m1_t v_sum, v_sum2, v_sum3;
    size_t l;

    pA = pV;
    pB = pM;
    pB3 = pB;
    for (i = 0; i < rowCnt; i++)
    {
        sum = ((q31_t)(*bias++) << bias_shift) + NN_ROUND(out_shift);
        sum2 = ((q31_t)(*bias++) << bias_shift) + NN_ROUND(out_shift);
        sum3 = ((q31_t)(*bias++) << bias_shift) + NN_ROUND(out_shift);
        pA = pV;
        pB = pB3;
        pB2 = pB + dim_vec;
        pB3 = pB2 + dim_vec;
        blkCnt = dim_vec & (~RVV_OPT_THRESHOLD);                             /* Loop counter */
        l = vsetvl_e32m1(1);
        v_sum = vmv_v_x_i32m1(0, l);
        v_sum2 = vmv_v_x_i32m1(0, l);
        v_sum3 = vmv_v_x_i32m1(0, l);
        for (; (l = vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l)
        {
            a8m4 = vle8_v_i8m4(pA, l);
            pA += l;
            b8m4 = vle8_v_i8m4(pB, l);
            pB += l;
            c8m4 = vle8_v_i8m4(pB2, l);
            pB2 += l;
            d8m4 = vle8_v_i8m4(pB3, l);
            pB3 += l;
            v_sum = vwredsum_vs_i16m8_i32m1(v_sum, vwmul_vv_i16m8(a8m4, b8m4, l), v_sum, l);
            v_sum2 = vwredsum_vs_i16m8_i32m1(v_sum2, vwmul_vv_i16m8(a8m4, c8m4, l), v_sum2, l);
            v_sum3 = vwredsum_vs_i16m8_i32m1(v_sum3, vwmul_vv_i16m8(a8m4, d8m4, l), v_sum3, l);
        }
        sum += vmv_x_s_i32m1_i32(v_sum);
        sum2 += vmv_x_s_i32m1_i32(v_sum2);
        sum3 += vmv_x_s_i32m1_i32(v_sum3);
        j = dim_vec & RVV_OPT_THRESHOLD;
        while (j--)
        {
            sum += *pA * *(pB++);
            sum2 += *pA * *(pB2++);
            sum3 += *pA * *(pB3++);
            pA++;
        }
        *pO++ = (q7_t)__SSAT((sum >> out_shift), 8);
        *pO++ = (q7_t)__SSAT((sum2 >> out_shift), 8);
        *pO++ = (q7_t)__SSAT((sum3 >> out_shift), 8);
    }

    rowCnt = num_of_rows % 0x3;
    while (rowCnt) {
        sum = ((q31_t)(*bias++) << bias_shift) + NN_ROUND(out_shift);
        pA = pV;
        blkCnt = dim_vec & (~RVV_OPT_THRESHOLD);                               /* Loop counter */
        l = vsetvl_e32m1(1);
        v_sum = vmv_v_x_i32m1(0, l);
        for (; (l = vsetvl_e8m4(blkCnt)) > 0; blkCnt -= l)
        {
            a8m4 = vle8_v_i8m4(pA, l);
            pA += l;
            b8m4 = vle8_v_i8m4(pB3, l);
            pB3 += l;
            v_sum = vwredsum_vs_i16m8_i32m1(v_sum, vwmul_vv_i16m8(a8m4, b8m4, l), v_sum, l);
        }
        sum += vmv_x_s_i32m1_i32(v_sum);
        j = dim_vec & RVV_OPT_THRESHOLD;
        while (j--)
        {
            sum += *(pA++) * *(pB3++);
        }
        *pO++ = (q7_t)__SSAT((sum >> out_shift), 8);

        rowCnt--;
    }
#elif defined (RISCV_MATH_DSP)
    /* Run the following code for RISC-V Core with DSP enabled */

    const q7_t *pB = pM;
    const q7_t *pB2;
    q7_t *pO = pOut;
    const q7_t *pBias = bias;
    const q7_t *pA;
    uint16_t  rowCnt = num_of_rows >> 1;

    /* expand the vector into the buffer */
    riscv_q7_to_q7_reordered_no_shift(pV, (q7_t *)vec_buffer, dim_vec);

    while (rowCnt)
    {
        pA = (q7_t *)vec_buffer;
        pB2 = pB + dim_vec;

        q31_t sum = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum2 = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);
#if __RISCV_XLEN == 64
        uint16_t  colCnt = dim_vec >> 3;
        q63_t sum64 = 0;
        q63_t sum642 = 0;
        while (colCnt)
        {
            q63_t inB1 = *__SIMD64(pB)++;
            q63_t inB2 = *__SIMD64(pB2)++;
            q63_t inA1 = *__SIMD64(pA)++;

            sum64  = __RV_SMAQA(sum64 , inA1, inB1);
            sum642 = __RV_SMAQA(sum642, inA1, inB2);

            colCnt--;
        }
        sum = sum + (q31_t)(sum64 & 0xFFFFFFFF) + (q31_t)((sum64 & 0xFFFFFFFF00000000)>>32);
        sum2 = sum2 + (q31_t)(sum642 & 0xFFFFFFFF) + (q31_t)((sum642 & 0xFFFFFFFF00000000)>>32);
        colCnt = dim_vec & 0x7;
#else
        uint16_t  colCnt = dim_vec >> 2;

        while (colCnt)
        {
            /*
            q31_t     inV, inM11, inM12, inM21, inM22;
            pB = read_and_pad_reordered(pB, &inM11, &inM12);
            pB2 = read_and_pad_reordered(pB2, &inM21, &inM22);

            inV = riscv_nn_read_q15x2_ia(&pA);

            sum = __SMLAD(inV, inM11, sum);
            sum2 = __SMLAD(inV, inM21, sum2);

            inV = riscv_nn_read_q15x2_ia(&pA);

            sum = __SMLAD(inV, inM12, sum);
            sum2 = __SMLAD(inV, inM22, sum2);
            */
            q31_t inB1 = *__SIMD32(pB)++;
            q31_t inB2 = *__SIMD32(pB2)++;

            q31_t inA1 = *__SIMD32(pA)++;

            sum  = __RV_SMAQA(sum , inA1, inB1);
            sum2 = __RV_SMAQA(sum2, inA1, inB2);

            colCnt--;
        }
        colCnt = dim_vec & 0x3;
#endif /* __RISCV_XLEN == 64 */
        while (colCnt)
        {
            q7_t inV = *pA++;
            q15_t inM = *pB++;
            q15_t inM2 = *pB2++;

            sum += inV * inM;
            sum2 += inV * inM2;
            colCnt--;
        } /* while over colCnt */
        *pO++ = (q7_t)(__SSAT((sum >> out_shift), 8));
        *pO++ = (q7_t)(__SSAT((sum2 >> out_shift), 8));

        /* adjust the pointers and counters */
        pB += dim_vec;
        rowCnt--;
    }

    /* left-over part of the rows */
    rowCnt = num_of_rows & 0x1;

    while (rowCnt)
    {
        uint16_t colCnt = dim_vec >> 2;
        q31_t sum = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);

        pA = (q7_t *)vec_buffer;

        while (colCnt)
        {
            /*
            q31_t     inV1, inV2, inM11, inM12;

            pB = read_and_pad_reordered(pB, &inM11, &inM12);

            inV1 = riscv_nn_read_q15x2_ia(&pA);
            sum = __SMLAD(inV1, inM11, sum);

            inV2 = riscv_nn_read_q15x2_ia(&pA);
            sum = __SMLAD(inV2, inM12, sum);
            */

            q31_t inB1 = *__SIMD32(pB)++;
            q31_t inA1 = *__SIMD32(pA)++;
            sum  = __RV_SMAQA(sum, inA1, inB1);

            colCnt--;
        }

        /* left-over of the vector */
        colCnt = dim_vec & 0x3;
        while (colCnt)
        {
            q7_t inV = *pA++;
            q15_t inM = *pB++;
            sum += inV * inM;
            colCnt--;
        }

        *pO++ = (q7_t)(__SSAT((sum >> out_shift), 8));

        rowCnt--;
    }

#else
    (void)vec_buffer;
    int i, j;

    /* Run the following code as reference implementation for RISC-V Core without DSP */
    for (i = 0; i < num_of_rows; i++)
    {
        int ip_out = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
        for (j = 0; j < dim_vec; j++)
        {
            ip_out += pV[j] * pM[i * dim_vec + j];
        }
        pOut[i] = (q7_t)__SSAT((ip_out >> out_shift), 8);
    }

#endif /* RISCV_MATH_DSP */

    /* Return to RISCV_MATH_SUCCESS */
    return (RISCV_MATH_SUCCESS);
}

/**
 * @} end of FC group
 */
