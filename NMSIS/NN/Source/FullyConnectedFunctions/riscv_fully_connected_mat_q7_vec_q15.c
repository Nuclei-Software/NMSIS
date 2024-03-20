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
 * @return     The function returns <code>RISCV_NMSIS_NN_SUCCESS</code>
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

riscv_nmsis_nn_status riscv_fully_connected_mat_q7_vec_q15(const q15_t *pV,
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
    const q7_t *pM1 = pM;
    const q15_t *pInVec = pV;
    const q7_t *qbias = bias;
    q15_t *px = pOut;
    uint16_t ii, jj;
    uint16_t numRows = num_of_rows;
    uint16_t numCols = dim_vec;
    uint16_t colCnt;
    size_t l;
    ptrdiff_t bstride = 1;       //  8bit/8bit = 1
    vint32m8_t vres0m8;
    vint8m2x4_t v_tuple;
    vint8m2x2_t v_tuple2;
    vint8m2_t va0m2, va1m2, va2m2, va3m2;
    vint16m4_t va0m4;

    for (jj = numRows; jj > 0; jj -= l) {
      l = __riscv_vsetvl_e32m8(jj);
      vres0m8 = __riscv_vsll_vx_i32m8(__riscv_vsext_vf4_i32m8(__riscv_vle8_v_i8m2(qbias, l), l), bias_shift, l);
      vres0m8 = __riscv_vadd_vx_i32m8(vres0m8, NN_ROUND(out_shift), l);
      qbias += l;
      pInVec = pV;
      pM1 = pM;
      colCnt = numCols;
      for (ii = 0; ii < colCnt / 4; ii++) {
        //vlsseg4e8_v_i8m2 (&va0m2, &va1m2, &va2m2, &va3m2, pM1, numCols, l);
        v_tuple = __riscv_vlsseg4e8_v_i8m2x4 (pM1, numCols, l);
        va0m2 = __riscv_vget_v_i8m2x4_i8m2 (v_tuple, 0);
        va1m2 = __riscv_vget_v_i8m2x4_i8m2 (v_tuple, 1);
        va2m2 = __riscv_vget_v_i8m2x4_i8m2 (v_tuple, 2);
        va3m2 = __riscv_vget_v_i8m2x4_i8m2 (v_tuple, 3);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va0m2, 0, l), l);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va1m2, 0, l), l);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va2m2, 0, l), l);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va3m2, 0, l), l);
        pM1 += 4;
      }
      colCnt = numCols & 0x3;

      for (ii = 0; ii < colCnt / 2; ii++) {
        //vlsseg2e8_v_i8m2(&va0m2, &va1m2, pM1, numCols, l);
        v_tuple2 = __riscv_vlsseg2e8_v_i8m2x2 (pM1, numCols, l);
        va0m2 = __riscv_vget_v_i8m2x2_i8m2 (v_tuple2, 0);
        va1m2 = __riscv_vget_v_i8m2x2_i8m2 (v_tuple2, 1);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va0m2, 0, l), l);
        vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va1m2, 0, l), l);
        pM1 += 2;
      }

      if (numCols & 0x1) {
          va0m2 = __riscv_vlse8_v_i8m2(pM1, numCols, l);
          vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pInVec++), __riscv_vwadd_vx_i16m4(va0m2, 0, l), l);
      }
      va0m4 = __riscv_vnclip_wx_i16m4(__riscv_vsra_vx_i32m8(vres0m8, out_shift, l), 0, __RISCV_VXRM_RNU, l);
      __riscv_vse16_v_i16m4(px, va0m4, l);
      px += l;
      pM += l * numCols;
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

        pA = pV;
        pB2 = pB + dim_vec;

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        uint64_t inV;
        uint64_t sum64 = 0;
        uint64_t sum64_2 = 0;
        uint64_t inM11, inM12, inM21, inM22;
        uint16_t colCnt = dim_vec >> 3;
#else
        q31_t inV;
        uint16_t colCnt = dim_vec >> 2;
        q31_t inM11, inM12, inM21, inM22;
#endif /* (__RISCV_XLEN == 64) */
        while (colCnt)
        {
#if (__RISCV_XLEN == 64)
            inV = riscv_nn_read_q15x4_ia((q15_t **)&pA);

            pB = read_and_pad64(pB, &inM11, &inM12);
            sum64 = __SMLAD(inV, inM11, sum64);
            pB2 = read_and_pad64(pB2, &inM21, &inM22);
            sum64_2 = __SMLAD(inV, inM21, sum64_2);
            inV = riscv_nn_read_q15x4_ia((q15_t **)&pA);
            sum64 = __SMLAD(inV, inM12, sum64);
            sum64_2 = __SMLAD(inV, inM22, sum64_2);
#else
#if defined (NUCLEI_DSP_N3)
            inV = riscv_nn_read_q15x4_ia((q15_t **)&pA);

            pB = read_and_pad_n32(pB, &inM11, &inM12);
            sum64 = __RV_DKMADA(sum64, inV, inM11);
            pB2 = read_and_pad_n32(pB2, &inM21, &inM22);
            sum64_2 = __RV_DKMADA(sum64_2, inV, inM21);
            inV = riscv_nn_read_q15x4_ia((q15_t **)&pA);
            sum64 = __RV_DKMADA(sum64, inV, inM12);
            sum64_2 = __RV_DKMADA(sum64_2, inV, inM22);
#else
            pB = read_and_pad(pB, &inM11, &inM12);
            pB2 = read_and_pad(pB2, &inM21, &inM22);

            inV = riscv_nn_read_q15x2_ia(&pA);

            sum = __SMLAD(inV, inM11, sum);
            sum2 = __SMLAD(inV, inM21, sum2);

            inV = riscv_nn_read_q15x2_ia(&pA);

            sum = __SMLAD(inV, inM12, sum);
            sum2 = __SMLAD(inV, inM22, sum2);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* (__RISCV_XLEN == 64) */

            colCnt--;
        }

#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
       sum += (q31_t)sum64 + (q31_t)(sum64 >> 32);
       sum2 += (q31_t)sum64_2 + (q31_t)(sum64_2 >> 32);
       colCnt = dim_vec & 0x7;
#else
       colCnt = dim_vec & 0x3;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
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

        pA = pV;
#if __RISCV_XLEN == 64
        uint16_t colCnt = dim_vec >> 3;
        uint64_t sum64 = 0;
        while (colCnt)
        {
            uint64_t inV1;
            uint64_t inM11, inM12;
            pB = (q7_t *) read_and_pad64((void *)pB, &inM11, &inM12);
            inV1 = *__SIMD64(pA)++;
            sum64 = __SMLAD(inV1, inM11, sum64);
            inV1 = *__SIMD64(pA)++;
            sum64 = __SMLAD(inV1, inM12, sum64);

            colCnt--;

        }
        sum += (q31_t)sum64 + (q31_t)(sum64 >> 32);
        /* left-over of the vector */
        colCnt = dim_vec & 0x7;
#else
#if defined (NUCLEI_DSP_N3)
        uint16_t colCnt = dim_vec >> 3;
        uint64_t sum64 = 0;
        while (colCnt)
        {
            uint64_t inV1;
            uint64_t inM11, inM12;
            pB = (q7_t *) read_and_pad_n32((void *)pB, &inM11, &inM12);
            inV1 = *__SIMD64(pA)++;
            sum64 = __RV_DKMADA(sum64, inV1, inM11);
            inV1 = *__SIMD64(pA)++;
            sum64 = __RV_DKMADA(sum64, inV1, inM12);

            colCnt--;

        }
        sum += (q31_t)sum64 + (q31_t)(sum64 >> 32);
        /* left-over of the vector */
        colCnt = dim_vec & 0x7;
#else
        uint16_t colCnt = dim_vec >> 2;
        while (colCnt)
        {
            q31_t inV1, inV2, inM11, inM12;

            pB = read_and_pad(pB, &inM11, &inM12);

            inV1 = *__SIMD32(pA)++;
            sum = __SMLAD(inV1, inM11, sum);

            inV2 = *__SIMD32(pA)++;
            sum = __SMLAD(inV2, inM12, sum);

            colCnt--;
        }
        /* left-over of the vector */
        colCnt = dim_vec & 0x3;
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

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

    /* Return to RISCV_NMSIS_NN_SUCCESS */
    return (RISCV_NMSIS_NN_SUCCESS);
}

/**
 * @} end of FC group
 */
