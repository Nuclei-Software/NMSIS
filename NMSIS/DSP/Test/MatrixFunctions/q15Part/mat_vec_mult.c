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
