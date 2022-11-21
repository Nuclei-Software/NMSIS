#include "ref.h"

void ref_mat_vec_mult_q15(const riscv_matrix_instance_q15 *pSrcMat, const q15_t *pVec, q15_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const q15_t *pSrcA = pSrcMat->pData;
    const q15_t *pInA;      /* input data matrix pointer A of Q15 type */
    const q15_t *pInVec;     /* input data matrix pointer B of Q15 type */
    q15_t *px;               /* Temporary output data matrix pointer */
    uint16_t row, col; /* loop counters */
    q15_t matData, vecData;
    q63_t sum;
    /* Process 4 rows at a time */
    px = pDst;

    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    /* row loop */
    for (row = 0; row < numRows; row++)
    {
        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;

        /* Initialize accumulators */
        sum = 0;

        /* Loop unrolling: process 2 columns per iteration */
        col = numCols;

        /* Initialize pointers to the starting address of the column being processed */
        pInA = pSrcA + row*numCols;

        // Main loop: matrix-vector multiplication
        while (col > 0u)
        {
            // Read 2 values from vector
		vecData = *(pInVec)++;
		matData = *(pInA)++;
		sum += matData * vecData;
            // Decrement the loop counter
            col--;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = (q15_t)(__SSAT((sum >> 15), 16));
    }
}

riscv_status ref_mat_mult_q15(const riscv_matrix_instance_q15 *pSrcA,
                            const riscv_matrix_instance_q15 *pSrcB,
                            riscv_matrix_instance_q15 *pDst)
{
    uint32_t r, c, i, outR, outC, innerSize;
    q63_t sum;

    outR = pSrcA->numRows;
    outC = pSrcB->numCols;
    innerSize = pSrcA->numCols;

    for (r = 0; r < outR; r++) {
        for (c = 0; c < outC; c++) {
            sum = 0;

            for (i = 0; i < innerSize; i++) {
                sum += (q31_t)(pSrcA->pData[r * innerSize + i]) *
                       pSrcB->pData[i * outC + c];
            }

            pDst->pData[r * outC + c] = ref_sat_q15(sum >> 15);
        }
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_mult_fast_q15(const riscv_matrix_instance_q15 *pSrcA,
                            const riscv_matrix_instance_q15 *pSrcB,
                            riscv_matrix_instance_q15 *pDst)
{
    uint32_t r, c, i, outR, outC, innerSize;
    q31_t sum;

    outR = pSrcA->numRows;
    outC = pSrcB->numCols;
    innerSize = pSrcA->numCols;

    for (r = 0; r < outR; r++) {
        for (c = 0; c < outC; c++) {
            sum = 0;

            for (i = 0; i < innerSize; i++) {
                sum += (q31_t)(pSrcA->pData[r * innerSize + i]) *
                       pSrcB->pData[i * outC + c];

            }
            pDst->pData[r * outC + c] = sum >> 15;
        }
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_add_q15(const riscv_matrix_instance_q15 *pSrcA,
                           const riscv_matrix_instance_q15 *pSrcB,
                           riscv_matrix_instance_q15 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = ref_sat_q15((q31_t)pSrcA->pData[i] + pSrcB->pData[i]);
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_sub_q15(const riscv_matrix_instance_q15 *pSrcA,
                           const riscv_matrix_instance_q15 *pSrcB,
                           riscv_matrix_instance_q15 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = ref_sat_q15((q31_t)pSrcA->pData[i] - pSrcB->pData[i]);
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_trans_q15(const riscv_matrix_instance_q15 *pSrc,
                             riscv_matrix_instance_q15 *pDst)
{
    uint32_t r, c;
    uint32_t numR = pSrc->numRows;
    uint32_t numC = pSrc->numCols;

    for (r = 0; r < numR; r++) {
        for (c = 0; c < numC; c++) {
            pDst->pData[c * numR + r] = pSrc->pData[r * numC + c];
        }
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_scale_q15(const riscv_matrix_instance_q15 *pSrc, q15_t scale,
                             int32_t shift, riscv_matrix_instance_q15 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */
    int32_t totShift = 15 - shift;

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] =
            ref_sat_q15(((q31_t)pSrc->pData[i] * scale) >> totShift);
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_cmplx_mult_q15(const riscv_matrix_instance_q15 *pSrcA,
                                  const riscv_matrix_instance_q15 *pSrcB,
                                  riscv_matrix_instance_q15 *pDst)
{
    uint32_t r, c, i, outR, outC, innerSize;
    q63_t sumR, sumI;
    q15_t a0, b0, c0, d0;

    outR = pSrcA->numRows;
    outC = pSrcB->numCols;
    innerSize = pSrcA->numCols;

    for (r = 0; r < outR; r++) {
        for (c = 0; c < outC; c++) {
            sumR = 0;
            sumI = 0;

            for (i = 0; i < innerSize; i++) {
                a0 = pSrcA->pData[2 * (r * innerSize + i) + 0];
                b0 = pSrcA->pData[2 * (r * innerSize + i) + 1];
                c0 = pSrcB->pData[2 * (i * outC + c) + 0];
                d0 = pSrcB->pData[2 * (i * outC + c) + 1];

                sumR += (q31_t)a0 * c0 - (q31_t)b0 * d0;
                sumI += (q31_t)b0 * c0 + (q31_t)a0 * d0;
            }

            pDst->pData[2 * (r * outC + c) + 0] = ref_sat_q15(sumR >> 15);
            pDst->pData[2 * (r * outC + c) + 1] = ref_sat_q15(sumI >> 15);
        }
    }

    return RISCV_MATH_SUCCESS;
}
