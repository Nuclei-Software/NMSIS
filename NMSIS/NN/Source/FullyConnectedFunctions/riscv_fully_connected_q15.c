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
 * Title:        riscv_fully_connected_q15.c
 * Description:  Q15 basic fully-connected layer function
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
 * @brief Q15 opt fully-connected layer function
 * @param[in]       pV          pointer to input vector
 * @param[in]       pM          pointer to matrix weights
 * @param[in]       dim_vec     length of the vector
 * @param[in]       num_of_rows number of rows in weight matrix
 * @param[in]       bias_shift  amount of left-shift for bias
 * @param[in]       out_shift   amount of right-shift for output
 * @param[in]       bias        pointer to bias
 * @param[in,out]   pOut        pointer to output vector
 * @param[in,out]   vec_buffer  pointer to buffer space for input
 * @return     The function returns <code>RISCV_NMSIS_NN_SUCCESS</code>
 *
 *
 * @details
 *
 * <b>Buffer size:</b>
 *
 * vec_buffer size: 0
 *
 */

riscv_nmsis_nn_status riscv_fully_connected_q15(const q15_t *pV,
                                   const q15_t *pM,
                                   const uint16_t dim_vec,
                                   const uint16_t num_of_rows,
                                   const uint16_t bias_shift,
                                   const uint16_t out_shift,
                                   const q15_t *bias,
                                   q15_t *pOut,
                                   q15_t *vec_buffer)
{
    (void)vec_buffer;
#if defined(RISCV_MATH_VECTOR)
    const q15_t *pM1 = pM;
    const q15_t *pInVec = pV;
    const q15_t *qbias = bias;
    q15_t *px = pOut;
    uint16_t ii, jj;
    uint16_t numRows = num_of_rows;
    uint16_t numCols = dim_vec;
    uint16_t colCnt;
    size_t l;
    ptrdiff_t bstride = 2;       //  16bit/8bit = 2
    vint32m8_t vres0m8;
    vint16m4x2_t v_tuple;
    vint16m2_t va0m2, va1m2, va2m2, va3m2;
    vint16m4_t va0m4, va1m4;

    for (jj = numRows; jj > 0; jj -= l) {
      l = __riscv_vsetvl_e32m8(jj);
      vres0m8 = __riscv_vsll_vx_i32m8(__riscv_vwadd_vx_i32m8(__riscv_vle16_v_i16m4(qbias, l), 0, l), bias_shift, l);
      vres0m8 = __riscv_vadd_vx_i32m8(vres0m8, NN_ROUND(out_shift), l);
      qbias += l;
      pInVec = pV;
      pM1 = pM;
      colCnt = numCols;
      for (ii = 0; ii < colCnt / 2; ii++) {
        //vlsseg2e16_v_i16m4(&va0m4, &va1m4, pM1, numCols * bstride, l);
        v_tuple = __riscv_vlsseg2e16_v_i16m4x2 (pM1, numCols * bstride, l);
        va0m4 = __riscv_vget_v_i16m4x2_i16m4 (v_tuple, 0);
        va1m4 = __riscv_vget_v_i16m4x2_i16m4 (v_tuple, 1);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), va0m4, l);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), va1m4, l);
        pM1 += 2;
      }

      if (numCols & 0x1) {
          va0m4 = __riscv_vlse16_v_i16m4(pM1, numCols * bstride, l);
          vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), va0m4, l);
      }
      va0m4 = __riscv_vnclip_wx_i16m4(__riscv_vsra_vx_i32m8(vres0m8, out_shift, l), 0, __RISCV_VXRM_RNU, l);
      __riscv_vse16_v_i16m4(px, va0m4, l);
      px += l;
      pM += l * numCols;
    }
#elif defined (RISCV_MATH_DSP)
    /* Run the following code for RISC-V Core with DSP enabled */
    // NOTE: the accumulated result may overflow, rvp will saturate it but scalar not.

    const q15_t *pB = pM;
    const q15_t *pB2 = pB + dim_vec;
    q15_t *pO = pOut;
    const q15_t *pA;
    const q15_t *pBias = bias;
    uint16_t rowCnt = num_of_rows >> 1;

    /* this loop loops over different output */
    while (rowCnt)
    {
        q31_t sum = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);
        q31_t sum2 = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);

        uint16_t colCnt = dim_vec >> 2;

        pA = pV;
        pB2 = pB + dim_vec;

        while (colCnt)
        {
            q31_t inV1, inM1, inM2;
            inV1 = riscv_nn_read_q15x2_ia(&pA);
            inM1 = riscv_nn_read_q15x2_ia(&pB);
            sum = __SMLAD(inV1, inM1, sum);
            inM2 = riscv_nn_read_q15x2_ia(&pB2);
            sum2 = __SMLAD(inV1, inM2, sum2);

            inV1 = riscv_nn_read_q15x2_ia(&pA);
            inM1 = riscv_nn_read_q15x2_ia(&pB);
            sum = __SMLAD(inV1, inM1, sum);
            inM2 = riscv_nn_read_q15x2_ia(&pB2);
            sum2 = __SMLAD(inV1, inM2, sum2);

            colCnt--;
        }
        colCnt = dim_vec & 0x3;
        while (colCnt)
        {
            q15_t inV = *pA++;
            q15_t inM = *pB++;
            q15_t inM2 = *pB2++;

            sum += inV * inM;
            sum2 += inV * inM2;
            colCnt--;
        } /* while over colCnt */
        *pO++ = (q15_t)(__SSAT((sum >> out_shift), 16));
        *pO++ = (q15_t)(__SSAT((sum2 >> out_shift), 16));

        /* adjust the pointers and counters */
        pB = pB + dim_vec;
        rowCnt--;
    }

    rowCnt = num_of_rows & 0x1;

    while (rowCnt)
    {
        q31_t sum = ((q31_t)(*pBias++) << bias_shift) + NN_ROUND(out_shift);


        pA = pV;

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        uint16_t  colCnt = dim_vec >> 2;
        q63_t sum64 = 0;
#else
        uint16_t  colCnt = dim_vec >> 2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

        while (colCnt) {
#if __RISCV_XLEN == 64
            q63_t inB1 = *__SIMD64(pB)++;
            q63_t inA1 = *__SIMD64(pA)++;
            sum64 = __SMLAD(inA1, inB1, sum64);
#else
#if defined (NUCLEI_DSP_N3)
            q63_t inB1 = *__SIMD64(pB)++;
            q63_t inA1 = *__SIMD64(pA)++;
            sum64 = __RV_DKMADA(sum64, inA1, inB1);
#else
            q31_t     inV1, inM1;
            inV1 = *__SIMD32(pA)++;
            inM1 = *__SIMD32(pB)++;
            sum = __SMLAD(inV1, inM1, sum);

            inV1 = riscv_nn_read_q15x2_ia(&pA);
            inM1 = riscv_nn_read_q15x2_ia(&pB);
            sum = __SMLAD(inV1, inM1, sum);

            // q31_t     inB1 = *__SIMD32(pB)++;
            // q31_t     inA1 = *__SIMD32(pA)++;
            // sum = __SMLAD(inA1, inB1, sum);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
            colCnt--;
        }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        sum += (q31_t)sum64 + (q31_t)(sum64 >>32);
        /* left-over of the vector */
        colCnt = dim_vec & 0x3;
#else
        /* left-over of the vector */
        colCnt = dim_vec & 0x3;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

        while (colCnt)
        {
            q15_t inV = *pA++;
            q15_t inM = *pB++;

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

#endif
    /* Return to application */
    return (RISCV_NMSIS_NN_SUCCESS);
}

/**
 * @} end of FC group
 */
