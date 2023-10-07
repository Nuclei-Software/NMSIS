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
 * Title:        riscv_nn_mat_mult_kernel_q7_q15_reordered.c
 * Description:  Matrix-multiplication function for convolution with reordered columns
 *
 * $Date:        17. January 2018
 * $Revision:    V.1.0.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */

#include "riscv_nnfunctions.h"
#include "riscv_nnsupportfunctions.h"

  /**
   * @brief Matrix-multiplication function for convolution with reordered columns
   * @param[in]       pA          pointer to operand A
   * @param[in]       pInBuffer   pointer to operand B, always conssists of 2 vectors
   * @param[in]       ch_im_out   numRow of A
   * @param[in]       numCol_A    numCol of A
   * @param[in]       bias_shift  amount of left-shift for bias
   * @param[in]       out_shift   amount of right-shift for output
   * @param[in]       bias        the bias
   * @param[in,out]   pOut        pointer to output
   * @return     The function returns the incremented output pointer
   *
   * @details
   *
   * This function assumes that data in pInBuffer are reordered
   */

q7_t *riscv_nn_mat_mult_kernel_q7_reordered(const q7_t * pA,
                                                  const q7_t * pInBuffer,
                                                  const uint16_t ch_im_out,
                                                  const uint16_t numCol_A,
                                                  const uint16_t bias_shift,
                                                  const uint16_t out_shift,
                                                  const q7_t * bias,
                                                  q7_t * pOut)
{

#if defined (RISCV_MATH_DSP) || defined(RISCV_MATH_VECTOR)
    /* set up the second output pointers */
    q7_t *pOut2 = pOut + ch_im_out;
    int i;

    /* this loop over rows in A */
    for (i = 0; i < ch_im_out; i += 2)
    {
        /* setup pointers for B */
        const q7_t *pB = pInBuffer;
        const q7_t *pB2 = pB + numCol_A;

        /* align the second pointer for A */
        const q7_t *pA2 = pA + numCol_A;

        /* init the sum with bias */
        q31_t sum = ((q31_t)(bias[i]) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum2 = sum;
        q31_t sum3 = ((q31_t)(bias[i + 1]) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum4 = sum3;

#if defined(RISCV_MATH_VECTOR)
        /* accumulate over the vector */
        size_t l;
        uint16_t  colCnt = numCol_A & (~RVV_OPT_THRESHOLD);

        vint8m4_t va1m4,va2m4,vb1m4,vb2m4;
        vint32m1_t v_sum, v_sum2, v_sum3, v_sum4;

        l = __riscv_vsetvl_e32m1(1);
        v_sum = __riscv_vmv_v_x_i32m1(0, l);
        v_sum2 = __riscv_vmv_v_x_i32m1(0, l);
        v_sum3 = __riscv_vmv_v_x_i32m1(0, l);
        v_sum4 = __riscv_vmv_v_x_i32m1(0, l);

        for (; (l = __riscv_vsetvl_e8m4(colCnt)) > 0; colCnt -= l) {
            va1m4 = __riscv_vle8_v_i8m4(pA , l);
            pA += l;
            va2m4 = __riscv_vle8_v_i8m4(pA2, l);
            pA2 += l;
            vb1m4 = __riscv_vle8_v_i8m4(pB , l);
            pB  += l;
            vb2m4 = __riscv_vle8_v_i8m4(pB2, l);
            pB2 += l;

            v_sum  = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwmul_vv_i16m8(va1m4, vb1m4, l), v_sum, l);
            v_sum2 = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwmul_vv_i16m8(va1m4, vb2m4, l), v_sum2, l);
            v_sum3 = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwmul_vv_i16m8(va2m4, vb1m4, l), v_sum3, l);
            v_sum4 = __riscv_vwredsum_vs_i16m8_i32m1(__riscv_vwmul_vv_i16m8(va2m4, vb2m4, l), v_sum4, l);
        }
        sum += (q31_t)__riscv_vmv_x_s_i32m1_i32(v_sum);
        sum2 += (q31_t)__riscv_vmv_x_s_i32m1_i32(v_sum2);
        sum3 += (q31_t)__riscv_vmv_x_s_i32m1_i32(v_sum3);
        sum4 += (q31_t)__riscv_vmv_x_s_i32m1_i32(v_sum4);
        colCnt = numCol_A & RVV_OPT_THRESHOLD;
#elif defined(RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
        uint16_t  colCnt = numCol_A >> 3;
        q63_t sum64 = 0, sum642 = 0, sum643 = 0, sum644 = 0;
        /* accumulate over the vector */
        while (colCnt)
        {
            q63_t inB1 = *__SIMD64(pB)++;
            q63_t inB2 = *__SIMD64(pB2)++;
            q63_t inA1 = *__SIMD64(pA)++;
            q63_t inA2 = *__SIMD64(pA2)++;

            sum64 = __RV_SMAQA(sum64 , inA1, inB1);
            sum642 = __RV_SMAQA(sum642, inA1, inB2);
            sum643 = __RV_SMAQA(sum643, inA2, inB1);
            sum644 = __RV_SMAQA(sum644, inA2, inB2);

            colCnt--;
        }                       /* while over colCnt */
        sum  += (q31_t)(sum64  & 0xFFFFFFFF) + (q31_t)((sum64  & 0xFFFFFFFF00000000)>>32);
        sum2 += (q31_t)(sum642 & 0xFFFFFFFF) + (q31_t)((sum642 & 0xFFFFFFFF00000000)>>32);
        sum3 += (q31_t)(sum643 & 0xFFFFFFFF) + (q31_t)((sum643 & 0xFFFFFFFF00000000)>>32);
        sum4 += (q31_t)(sum644 & 0xFFFFFFFF) + (q31_t)((sum644 & 0xFFFFFFFF00000000)>>32);

        colCnt = numCol_A & 0x7;
#else
        uint16_t  colCnt = numCol_A >> 2;
        /* accumulate over the vector */
        while (colCnt)
        {
            /*
            q31_t     inA11, inA12, inA21, inA22;
            q31_t     inB1 = *__SIMD32(pB)++;
            q31_t     inB2 = *__SIMD32(pB2)++;

            pA = (q7_t *) read_and_pad_reordered((void *)pA, &inA11, &inA12);
            pA2 = (q7_t *) read_and_pad_reordered((void *)pA2, &inA21, &inA22);

            sum = __SMLAD(inA11, inB1, sum);
            sum2 = __SMLAD(inA11, inB2, sum2);
            sum3 = __SMLAD(inA21, inB1, sum3);
            sum4 = __SMLAD(inA21, inB2, sum4);

            inB1 = *__SIMD32(pB)++;
            inB2 = *__SIMD32(pB2)++;

            sum = __SMLAD(inA12, inB1, sum);
            sum2 = __SMLAD(inA12, inB2, sum2);
            sum3 = __SMLAD(inA22, inB1, sum3);
            sum4 = __SMLAD(inA22, inB2, sum4);
            */

            q31_t inB1 = *__SIMD32(pB)++;
            q31_t inB2 = *__SIMD32(pB2)++;

            q31_t inA1 = *__SIMD32(pA)++;
            q31_t inA2 = *__SIMD32(pA2)++;

            sum  = __RV_SMAQA(sum , inA1, inB1);
            sum2 = __RV_SMAQA(sum2, inA1, inB2);
            sum3 = __RV_SMAQA(sum3, inA2, inB1);
            sum4 = __RV_SMAQA(sum4, inA2, inB2);

            colCnt--;
        }                       /* while over colCnt */
        colCnt = numCol_A & 0x3;
#endif /* __RISCV_XLEN == 64 */
#endif /* defined(RISCV_MATH_VECTOR) */
        while (colCnt)
        {
            q7_t inA1 = *pA++;
            q7_t inB1 = *pB++;
            q7_t inA2 = *pA2++;
            q7_t inB2 = *pB2++;

            sum += inA1 * inB1;
            sum2 += inA1 * inB2;
            sum3 += inA2 * inB1;
            sum4 += inA2 * inB2;
            colCnt--;
        }  
        *pOut++ = (q7_t) __SSAT((sum >> out_shift), 8);
        *pOut++ = (q7_t) __SSAT((sum3 >> out_shift), 8);
        *pOut2++ = (q7_t) __SSAT((sum2 >> out_shift), 8);
        *pOut2++ = (q7_t) __SSAT((sum4 >> out_shift), 8);
        /* skip the row computed with A2 */
        pA += numCol_A;
    }                           /* for over ch_im_out */

    pOut += ch_im_out;

    /* return the new output pointer with offset */
    return pOut;
#else
    /* To be completed */
    return NULL;
#endif                          /* RISCV_MATH_DSP */
}
