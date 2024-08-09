/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_vec_mult_f16.c
 * Description:  Floating-point matrix and vector multiplication
 *
 * $Date:        23 April 2021
 *
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/matrix_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
 * @ingroup groupMatrix
 */


/**
 * @addtogroup MatrixVectMult
 * @{
 */

/**
 * @brief Floating-point matrix and vector multiplication.
 * @param[in]       *pSrcMat points to the input matrix structure
 * @param[in]       *pVec points to input vector
 * @param[out]      *pDst points to output vector
 */
RISCV_DSP_ATTRIBUTE void riscv_mat_vec_mult_f16(const riscv_matrix_instance_f16 *pSrcMat, const float16_t *pVec, float16_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float16_t *pSrcA = pSrcMat->pData;
    const float16_t *pInA1;      /* input data matrix pointer A of Q31 type */
    const float16_t *pInA2;      /* input data matrix pointer A of Q31 type */
    const float16_t *pInA3;      /* input data matrix pointer A of Q31 type */
    const float16_t *pInA4;      /* input data matrix pointer A of Q31 type */
    const float16_t *pInVec;     /* input data matrix pointer B of Q31 type */
    float16_t *px;               /* Temporary output data matrix pointer */
    uint16_t i, row, colCnt; /* loop counters */
    float16_t matData, matData2, vecData, vecData2;

#if defined(RISCV_MATH_VECTOR)
    uint32_t ii, jj;
    size_t l;
    ptrdiff_t bstride = 2;       //  16bit/8bit = 2
    vfloat16m8_t va0m8, vres0m8;
    px = pDst;
    for (jj = numRows; jj > 0; jj -= l) {
      l = __riscv_vsetvl_e16m8(jj);
      vres0m8 = __riscv_vfmv_v_f_f16m8(0.0, l);
      pInVec = pVec;
      pInA1 = pSrcA;
      for (ii = 0; ii < numCols; ii++) {
        va0m8 = __riscv_vlse16_v_f16m8(pInA1 + ii, numCols * bstride, l);
        vres0m8 = __riscv_vfmacc_vf_f16m8(vres0m8, *(pInVec + ii), va0m8, l);
      }
      __riscv_vse16_v_f16m8(px, vres0m8, l);
      px += l;
      pSrcA += l * numCols;
    }
#else

    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = pDst;

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    while (row > 0) {
        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;

        /* Initialize accumulators */
        float16_t sum1 = 0.0f16;
        float16_t sum2 = 0.0f16;
        float16_t sum3 = 0.0f16;
        float16_t sum4 = 0.0f16;

        /* Loop unrolling: process 2 columns per iteration */
        colCnt = numCols;

        /* Initialize pointers to the starting address of the column being processed */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        pInA4 = pInA3 + numCols;


        // Main loop: matrix-vector multiplication
        while (colCnt > 0u) {
            // Read 2 values from vector
            vecData = *(pInVec)++;
            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = *(pInA1)++;
            sum1 += (_Float16)matData * (_Float16)vecData;
            matData = *(pInA2)++;
            sum2 += (_Float16)matData * (_Float16)vecData;
            matData = *(pInA3)++;
            sum3 += (_Float16)matData * (_Float16)vecData;
            matData = *(pInA4)++;
            sum4 += (_Float16)matData * (_Float16)vecData;

            // Decrement the loop counter
            colCnt--;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = sum1;
        *px++ = sum2;
        *px++ = sum3;
        *px++ = sum4;

        i = i + numCols * 4;

        /* Decrement the row loop counter */
        row--;
    }

    /* process any remaining rows */
    row = numRows & 3u;
    while (row > 0) {

        float16_t sum = 0.0f16;
        pInVec = pVec;
        pInA1 = pSrcA + i;

        colCnt = numCols >> 1;

        while (colCnt > 0) {
            vecData = *(pInVec)++;
            vecData2 = *(pInVec)++;
            matData = *(pInA1)++;
            matData2 = *(pInA1)++;
            sum += (_Float16)matData * (_Float16)vecData;
            sum += (_Float16)matData2 * (_Float16)vecData2;
            colCnt--;
        }
        // process remainder of row
        colCnt = numCols & 1u;
        while (colCnt > 0) {
            sum += (_Float16)*pInA1++ * (_Float16)*pInVec++;
            colCnt--;
        }

        *px++ = sum;
        i = i + numCols;
        row--;
    }
#endif /*defined(RISCV_MATH_VECTOR)*/
}

/**
 * @} end of MatrixMult group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

