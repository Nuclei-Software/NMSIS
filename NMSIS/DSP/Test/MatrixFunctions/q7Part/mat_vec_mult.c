#include "ref.h"

void ref_mat_vec_mult_q7(const riscv_matrix_instance_q7 *pSrcMat, const q7_t *pVec, q7_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const q7_t *pSrcA = pSrcMat->pData;
    const q7_t *pInA;       /* input data matrix pointer of Q7 type */
    const q7_t *pInA2;       /* input data matrix pointer of Q7 type */
    const q7_t *pInA3;       /* input data matrix pointer of Q7 type */
    const q7_t *pInA4;       /* input data matrix pointer of Q7 type */
    const q7_t *pInVec;      /* input data vector pointer of Q7 type */
    q7_t *px;                /* output data pointer */
    uint32_t i, row, col; /* loop counters */
    q31_t sum;
    q7_t matData, vecData;

    px = pDst;
    /* The following loop performs the dot-product of each row in pSrcA with the vector */
    for (row = 0; row < numRows; row++)
    {
        /* For every row wise process, the pInVec pointer is set
         ** to the starting address of the vector */
        pInVec = pVec;
        /* Initialize accumulators */
        sum = 0;
        /* Initialize row pointers so we can track 4 rows at once */
        pInA = pSrcA + row*numCols;
        col = numCols;
        while (col > 0u)
        {
            vecData = *(pInVec)++;
            matData = *(pInA)++;
            sum += matData * vecData;
            // Decrement the loop counter
            col--;
        }
        /* Saturate and store the result in the destination buffer */
        *px++ = (q7_t)(__SSAT((sum >> 7), 8));
    }
}

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


/* void ref_mat_vec_mult_f16(const riscv_matrix_instance_f16 *pSrcMat, const float16_t *pVec, float16_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float16_t *pSrcA = pSrcMat->pData;
    const float16_t *pInA;     
    const float16_t *pInVec;     
    float16_t *px;              
    uint16_t i, row, col; 
    float16_t matData, matData2, vecData, vecData2;
    float16_t sum;
    i = 0u;
    px = pDst;

    for (row = 0; row < numRows; row++)
    {
        pInVec = pVec;
        sum = 0.0f;
        col = numCols;
        pInA = pSrcA + row*numCols;

        // Main loop: matrix-vector multiplication
        while (col > 0u) {
            // Read 2 values from vector
            vecData = *(pInVec)++;
            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = *(pInA)++;
            sum += matData * vecData;
            // Decrement the loop counter
            col--;
        }

        *px++ = sum;
    }
}

void riscv_mat_vec_mult_f32(const riscv_matrix_instance_f32 *pSrcMat, const float32_t *pVec, float32_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float32_t *pSrcA = pSrcMat->pData;
    const float32_t *pInA;     
    const float32_t *pInVec;    
    float32_t *px;               
    uint16_t i, row, colCnt; 
    float32_t matData, matData2, vecData, vecData2;
    float32_t sum;
    i = 0u;
    px = pDst;

    for (row = 0; row < numRows; row++)
    {
        pInVec = pVec;
        float32_t sum = 0.0f;
        colCnt = numCols;
        pInA = pSrcA + row*numCols;

        // Main loop: matrix-vector multiplication
        while (colCnt > 0u) {
            // Read 2 values from vector
            vecData = *(pInVec)++;
            // Read 8 values from the matrix - 2 values from each of 4 rows, and do multiply accumulate
            matData = *(pInA)++;
            sum += matData * vecData;
            // Decrement the loop counter
            colCnt--;
        }

        *px++ = sum;
    }
} */
