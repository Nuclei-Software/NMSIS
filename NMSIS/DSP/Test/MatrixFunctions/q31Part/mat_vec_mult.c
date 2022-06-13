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
