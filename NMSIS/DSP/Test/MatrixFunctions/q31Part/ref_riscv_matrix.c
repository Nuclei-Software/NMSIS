#include "ref.h"

void ref_mat_vec_mult_q31(const riscv_matrix_instance_q31 *pSrcMat, const q31_t *pVec, q31_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const q31_t *pSrcA = pSrcMat->pData;
    const q31_t *pInA;      /* input data matrix pointer A of Q31 type */
    const q31_t *pInVec;     /* input data matrix pointer B of Q31 type */
    q31_t *px;               /* Temporary output data matrix pointer */
    uint16_t i, row, col; /* loop counters */
    q31_t matData, vecData;
    q63_t sum;
    i = 0u;
    px = pDst;

    for(row = 0; row < numRows; row++)
    {
	pInVec = pVec;
	sum = 0;
	col = numCols;
	pInA = pSrcA + row*numCols;

	while (col > 0u)
	{
	    vecData = *(pInVec)++;
	    matData = *(pInA)++;
	    sum += (q63_t)matData * vecData;
	    col--;
	}
	*px++ = (q31_t)(sum >> 31);
    }
}

riscv_status ref_mat_mult_q31(const riscv_matrix_instance_q31 *pSrcA,
                            const riscv_matrix_instance_q31 *pSrcB,
                            riscv_matrix_instance_q31 *pDst)
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
                sum += (q63_t)(pSrcA->pData[r * innerSize + i]) *
                       pSrcB->pData[i * outC + c];
            }

            pDst->pData[r * outC + c] = sum >> 31;
        }
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_mult_fast_q31(const riscv_matrix_instance_q31 *pSrcA,
                            const riscv_matrix_instance_q31 *pSrcB,
                            riscv_matrix_instance_q31 *pDst)
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
                sum = (q31_t) (((q63_t) sum << 32) + ((q63_t)(pSrcA->pData[r * innerSize + i]) *
                       pSrcB->pData[i * outC + c]) >> 32);
            }

            pDst->pData[r * outC + c] = sum << 1;
        }
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_mult_opt_q31(
  const riscv_matrix_instance_q31 * pSrcA,
  const riscv_matrix_instance_q31 * pSrcB,
        riscv_matrix_instance_q31 * pDst)
{

	ref_mat_mult_q31(pSrcA, pSrcB, pDst);
    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_add_q31(const riscv_matrix_instance_q31 *pSrcA,
                           const riscv_matrix_instance_q31 *pSrcB,
                           riscv_matrix_instance_q31 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = ref_sat_q31((q63_t)pSrcA->pData[i] + pSrcB->pData[i]);
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_sub_q31(const riscv_matrix_instance_q31 *pSrcA,
                           const riscv_matrix_instance_q31 *pSrcB,
                           riscv_matrix_instance_q31 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = ref_sat_q31((q63_t)pSrcA->pData[i] - pSrcB->pData[i]);
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_trans_q31(const riscv_matrix_instance_q31 *pSrc,
                             riscv_matrix_instance_q31 *pDst)
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

riscv_status ref_mat_scale_q31(const riscv_matrix_instance_q31 *pSrc, q31_t scale,
                             int32_t shift, riscv_matrix_instance_q31 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */
    int32_t totShift = shift + 1;
    q31_t tmp;

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;

    for (i = 0; i < numSamples; i++) {
        tmp = ((q63_t)pSrc->pData[i] * scale) >> 32;
        pDst->pData[i] = ref_sat_q31((q63_t)tmp << totShift);
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_cmplx_mult_q31(const riscv_matrix_instance_q31 *pSrcA,
                                  const riscv_matrix_instance_q31 *pSrcB,
                                  riscv_matrix_instance_q31 *pDst)
{
    uint32_t r, c, i, outR, outC, innerSize;
    q63_t sumR, sumI;
    q31_t a0, b0, c0, d0;

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

                sumR += (q63_t)a0 * c0 - (q63_t)b0 * d0;
                sumI += (q63_t)b0 * c0 + (q63_t)a0 * d0;
            }

            pDst->pData[2 * (r * outC + c) + 0] = clip_q63_to_q31(sumR >> 31);
            pDst->pData[2 * (r * outC + c) + 1] = clip_q63_to_q31(sumI >> 31);
        }
    }

    return RISCV_MATH_SUCCESS;
}
