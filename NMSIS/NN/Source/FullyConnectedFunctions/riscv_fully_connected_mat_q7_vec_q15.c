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
 * Title:        riscv_fully_connected_mat_q7_vec_q15.c
 * Description:  Mixed Q15-Q7 fully-connected layer function
 *
 * $Date:        20. July 2021
 * $Revision:    V.1.1.1
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
 * @brief Mixed Q15-Q7 fully-connected layer function
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
 * vec_buffer size: 0
 *
 *  Q7_Q15 version of the fully connected layer
 *
 *  Weights are in q7_t and Activations are in q15_t
 *
 */

riscv_status riscv_fully_connected_mat_q7_vec_q15(const q15_t *pV,
                                              const q7_t *pM,
                                              const uint16_t dim_vec,
                                              const uint16_t num_of_rows,
                                              const uint16_t bias_shift,
                                              const uint16_t out_shift,
                                              const q7_t *bias,
                                              q15_t *pOut,
                                              q15_t *vec_buffer)
{
    (void)vec_buffer;
#if defined(RISCV_MATH_VECTOR)
    (void)vec_buffer;
    int i, j;
    uint16_t rowCnt = num_of_rows / 0x3;
    q15_t *pO = pOut;

    q31_t sum, sum2, sum3;
    const q15_t *pA;
    const q7_t *pB, *pB2, *pB3;
    int32_t blkCnt;
    vint16m4_t a16m4, b16m4, c16m4, d16m4;

    vint32m1_t v_temp, v_temp1;
    size_t l;
    l = vsetvl_e32m1(1);
    v_temp = vsub_vv_i32m1(v_temp, v_temp, l);

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
        for (; (l = vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
        {
            a16m4 = vle16_v_i16m4(pA, l);
            pA += l;
            b16m4 = vwadd_vx_i16m4(vle8_v_i8m2(pB, l), 0, l);
            pB += l;
            c16m4 = vwadd_vx_i16m4(vle8_v_i8m2(pB2, l), 0, l);
            pB2 += l;
            d16m4 = vwadd_vx_i16m4(vle8_v_i8m2(pB3, l), 0, l);
            pB3 += l;
            sum += vmv_x_s_i32m1_i32(vredsum_vs_i32m8_i32m1(v_temp, vwmul_vv_i32m8(a16m4, b16m4, l), v_temp, l));
            sum2 += vmv_x_s_i32m1_i32(vredsum_vs_i32m8_i32m1(v_temp, vwmul_vv_i32m8(a16m4, c16m4, l), v_temp, l));
            sum3 += vmv_x_s_i32m1_i32(vredsum_vs_i32m8_i32m1(v_temp, vwmul_vv_i32m8(a16m4, d16m4, l), v_temp, l));
        }
        j = dim_vec & RVV_OPT_THRESHOLD;
        while (j--)
        {
            sum += *pA * *(pB++);
            sum2 += *pA * *(pB2++);
            sum3 += *pA * *(pB3++);
            pA++;
        }
        *pO++ = (q15_t)__SSAT((sum >> out_shift), 16);
        *pO++ = (q15_t)__SSAT((sum2 >> out_shift), 16);
        *pO++ = (q15_t)__SSAT((sum3 >> out_shift), 16);
    }

    rowCnt = num_of_rows % 0x3;
    while (rowCnt) {
        sum = ((q31_t)(*bias++) << bias_shift) + NN_ROUND(out_shift);
        pA = pV;
        blkCnt = dim_vec & (~RVV_OPT_THRESHOLD);
        l = vsetvl_e32m1(1);
        v_temp1 = vsub_vv_i32m1(v_temp1, v_temp1, l);
        for (; (l = vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
        {
            a16m4 = vle16_v_i16m4(pA, l);
            pA += l;
            b16m4 = vwadd_vx_i16m4(vle8_v_i8m2(pB3, l), 0, l);
            pB3 += l;
            v_temp1 = vredsum_vs_i32m8_i32m1(v_temp1, vwmul_vv_i32m8(a16m4, b16m4, l), v_temp1, l);
        }
        sum += vmv_x_s_i32m1_i32(v_temp1);
        j = dim_vec & RVV_OPT_THRESHOLD;
        while (j--)
        {
            sum += *(pA++) * *(pB3++);
        }
        *pO++ = (q15_t)__SSAT((sum >> out_shift), 16);

        rowCnt--;
    }
#elif defined (RISCV_MATH_DSP)
    /* Run the following code for RISC-V Core with DSP enabled */

    const q7_t *pB = pM;
    const q7_t *pB2;
    q15_t *pO = pOut;
    const q7_t *pBias = bias;
    const q15_t *pA = pV;

    uint16_t rowCnt = num_of_rows >> 1;

    while (rowCnt)
    {
        q31_t sum = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum2 = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);
        uint16_t colCnt = dim_vec >> 2;

        pA = pV;
        pB2 = pB + dim_vec;

        while (colCnt)
        {
            q31_t inV, inM11, inM12, inM21, inM22;
            pB = read_and_pad(pB, &inM11, &inM12);
            pB2 = read_and_pad(pB2, &inM21, &inM22);

            inV = riscv_nn_read_q15x2_ia(&pA);

            sum  = __RV_KMADA(sum , inV, inM11);
            sum2 = __RV_KMADA(sum2, inV, inM21);

            inV = riscv_nn_read_q15x2_ia(&pA);

            sum  = __RV_KMADA(sum , inV, inM12);
            sum2 = __RV_KMADA(sum2, inV, inM22);

            colCnt--;
        }
        colCnt = dim_vec & 0x3;
        while (colCnt)
        {
            q15_t inV = *pA++;
            q7_t inM = *pB++;
            q7_t inM2 = *pB2++;

            sum += inV * inM;
            sum2 += inV * inM2;
            colCnt--;
        } /* while over colCnt */
        *pO++ = (q15_t)(__SSAT((sum >> out_shift), 16));
        *pO++ = (q15_t)(__SSAT((sum2 >> out_shift), 16));

        /*adjust the pointers and counters */
        pB += dim_vec;
        rowCnt--;
    }

    /* left-over part of the rows */
    rowCnt = num_of_rows & 0x1;

    while (rowCnt)
    {
        q31_t sum = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);
        uint16_t colCnt = dim_vec >> 2;

        pA = pV;
#if __RISCV_XLEN == 64
        q63_t sum64 = 0;
        while (colCnt)
        {
            q63_t inV1, inV2;
            q31_t inM11, inM12;
            pB = (q7_t *) read_and_pad((void *)pB, &inM11, &inM12);
            inV2 = __RV_PKBB32(inM12,inM11);
            inV1 = *__SIMD64(pA)++;
            sum64 = __RV_KMADA(sum64, inV1, inV2);

            // inV2 = *__SIMD32(pA)++;
            // sum = __RV_KMADA(sum, inV2, inM12);
            colCnt--;

        }
        sum = sum + (q31_t)(sum64 & 0xFFFFFFFF) + (q31_t)((sum64 & 0xFFFFFFFF00000000)>>32);

#else

        while (colCnt)
        {
            q31_t inV1, inV2, inM11, inM12;

            pB = read_and_pad(pB, &inM11, &inM12);

            inV1 = *__SIMD32(pA)++;
            sum = __RV_KMADA(sum, inV1, inM11);

            inV2 = *__SIMD32(pA)++;
            sum = __RV_KMADA(sum, inV2, inM12);
            /*
            q31_t     inB1 = *__SIMD32(pB)++;
            q31_t     inA1 = *__SIMD32(pA)++;
            sum  = __RV_SMALDA(sum, inA1, inB1); */

            colCnt--;
        }

#endif /* __RISCV_XLEN == 64 */
        /* left-over of the vector */
        colCnt = dim_vec & 0x3;
        while (colCnt)
        {
            q15_t inV = *pA++;
            q7_t inM = *pB++;
            sum += inV * inM;
            colCnt--;
        }

        *pO++ = (q15_t)(__SSAT((sum >> out_shift), 16));

        rowCnt--;
    }

#else
    int i, j;
    /* Run the following code as reference implementation for RISC-V Core without DSP */
    for (i = 0; i < num_of_rows; i++)
    {
        int ip_out = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
        for (j = 0; j < dim_vec; j++)
        {
            ip_out += pV[j] * pM[i * dim_vec + j];
        }
        pOut[i] = (q15_t)__SSAT((ip_out >> out_shift), 16);
    }

#endif /* RISCV_MATH_DSP */

    /* Return to RISCV_MATH_SUCCESS */
    return (RISCV_MATH_SUCCESS);
}

/**
 * @} end of FC group
 */
