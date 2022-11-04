/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_vec_mult_q31.c
 * Description:  Q31 matrix and vector multiplication
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

#include "dsp/matrix_functions.h"

/**
 * @ingroup groupMatrix
 */



/**
 * @addtogroup MatrixVectMult
 * @{
 */

/**
 * @brief Q31 matrix and vector multiplication.
 * @param[in]       *pSrcMat points to the input matrix structure
 * @param[in]       *pVec points to the input vector
 * @param[out]      *pDst points to the output vector
 */
void riscv_mat_vec_mult_q31(const riscv_matrix_instance_q31 *pSrcMat, const q31_t *pVec, q31_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const q31_t *pSrcA = pSrcMat->pData;
    const q31_t *pInA1;      /* input data matrix pointer A of Q31 type */
    const q31_t *pInA2;      /* input data matrix pointer A of Q31 type */
    const q31_t *pInA3;      /* input data matrix pointer A of Q31 type */
    const q31_t *pInA4;      /* input data matrix pointer A of Q31 type */
    const q31_t *pInVec;     /* input data matrix pointer B of Q31 type */
    q31_t *px;               /* Temporary output data matrix pointer */
    uint16_t i, row, colCnt; /* loop counters */
    q31_t matData2, vecData2;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    q63_t matData, vecData;
#else
    q31_t matData, vecData;
#endif/* #if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#if defined(RISCV_MATH_VECTOR)
    const q31_t *pInA = NULL;
    const q31_t *pInB = NULL;
    // number of matrix columns  numColsA = numrowB
    uint16_t blkCnt = pSrcMat->numCols;
    size_t l;        // max_l is the maximum column elements at a time
    uint16_t rownum; //  How many rowumns and rownum are controlled
    vint32m4_t v_inA, v_inB;
    q63_t sum = 0;
    vint64m1_t vsum;
    px = pDst;
    for (rownum = 0; rownum < numRows; rownum++) {
        l = vsetvl_e64m1(1);
        vsum = vmv_s_x_i64m1(vsum, 0, l);
        blkCnt = numCols;
        pInB = pVec;
        // Pointer to the first element of the next row for matrix A
        pInA = pSrcMat->pData + rownum * pSrcMat->numCols;

        for (; (l = vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l) {
            v_inA = vle32_v_i32m4(pInA, l);
            pInA += l;
            v_inB = vle32_v_i32m4(pInB, l);
            pInB += l;
            vsum = vredsum_vs_i64m8_i64m1(vsum, vwmul_vv_i64m8(v_inA, v_inB, l), vsum, l);
        }
        sum = vmv_x_s_i64m1_i64(vsum);
        *px++ = (q31_t)(sum >> 31);
    }
#else

    /* Process 4 rows at a time */
    row = numRows >> 2;
    i = 0u;
    px = pDst;

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    while (row > 0) {
        /* Initialize accumulators */
        q63_t sum1 = 0;
        q63_t sum2 = 0;
        q63_t sum3 = 0;
        q63_t sum4 = 0;

        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;

#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        colCnt = numCols >> 1;
#else
        /* Loop unrolling: process 2 columns per iteration */
        colCnt = numCols;
#endif/* #if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        /* Initialize pointers to the starting address of the column being processed */
        pInA1 = pSrcA + i;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        pInA4 = pInA3 + numCols;

        // Main loop: matrix-vector multiplication
        while (colCnt > 0u) {
#if defined (RISCV_MATH_DSP) && (RISCV_XLEN == 64)
            // Read 2 values from vector
            vecData = read_q31x2_ia((q31_t **)&pInVec);

            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = read_q31x2_ia((q31_t **)&pInA1);
            sum1 = __RV_SMAR64(sum1, matData, vecData);
            matData = read_q31x2_ia((q31_t **)&pInA2);
            sum2 = __RV_SMAR64(sum2, matData, vecData);
            matData = read_q31x2_ia((q31_t **)&pInA3);
            sum3 = __RV_SMAR64(sum3, matData, vecData);
            matData = read_q31x2_ia((q31_t **)&pInA4);
            sum4 = __RV_SMAR64(sum4, matData, vecData);
#else
            // Read 1 values from vector
            vecData = *(pInVec)++;

            // Read 4 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = *(pInA1)++;
            sum1 += (q63_t)matData * vecData;
            matData = *(pInA2)++;
            sum2 += (q63_t)matData * vecData;
            matData = *(pInA3)++;
            sum3 += (q63_t)matData * vecData;
            matData = *(pInA4)++;
            sum4 += (q63_t)matData * vecData;
#endif/* defined (RISCV_MATH_DSP) && (RISCV_XLEN == 64) */
            // Decrement the loop counter
            colCnt--;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = (q31_t)(sum1 >> 31);
        *px++ = (q31_t)(sum2 >> 31);
        *px++ = (q31_t)(sum3 >> 31);
        *px++ = (q31_t)(sum4 >> 31);

        i = i + numCols * 4;

        /* Decrement the row loop counter */
        row--;
    }

    /* process any remaining rows */
    row = numRows & 3u;
    while (row > 0) {

        q63_t sum = 0;
        pInVec = pVec;
        pInA1 = pSrcA + i;

        colCnt = numCols >> 1;

        while (colCnt > 0) {
            vecData = *(pInVec)++;
            vecData2 = *(pInVec)++;
            matData = *(pInA1)++;
            matData2 = *(pInA1)++;
            sum += (q63_t)matData * vecData;
            sum += (q63_t)matData2 * vecData2;
            colCnt--;
        }

        // process remainder of row
        colCnt = numCols & 1u;
        while (colCnt > 0) {
            sum += (q63_t)*pInA1++ * *pInVec++;
            colCnt--;
        }

        *px++ = (q31_t)(sum >> 31);
        i = i + numCols;
        row--;
    }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
 * @} end of MatrixMult group
 */
