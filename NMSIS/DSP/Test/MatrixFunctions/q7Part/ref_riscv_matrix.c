#include "ref.h"

riscv_status ref_mat_mult_q7(const riscv_matrix_instance_q7 *pSrcA, const riscv_matrix_instance_q7 *pSrcB, riscv_matrix_instance_q7 *pDst)
{
    q31_t sum; /* accumulator */
    q7_t *pIn1 = pSrcA->pData;                    /* input data matrix pointer A */
    q7_t *pIn2 = pSrcB->pData;                    /* input data matrix pointer B */
    q7_t *pInA = pSrcA->pData;                    /* input data matrix pointer A of Q7 type */
    q7_t *pInB = pSrcB->pData;                    /* input data matrix pointer B of Q7 type */
    q7_t *pOut = pDst->pData;                     /* output data matrix pointer */
    q7_t *px;                                     /* Temporary output data matrix pointer */
    uint16_t numColsB = pSrcB->numCols;           /* number of columns of input matrix B */
    uint16_t numColsA = pSrcA->numCols;           /* number of columns of input matrix A */
    uint16_t numRowsA = pSrcA->numRows;           /* number of rows of input matrix A    */
    uint16_t col, i = 0U, row = numRowsA, colCnt; /* loop counters */
    riscv_status status;                            /* status of matrix multiplication */

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do {
        /* Output pointer is set to starting address of the row being processed */
        px = pOut + i;

        /* For every row wise process, the column loop counter is to be initiated */
        col = numColsB;

        /* For every row wise process, the pIn2 pointer is set
         ** to the starting address of the pSrcB data */
        pIn2 = pSrcB->pData;

        /* column loop */
        do {
            /* Set the variable sum, that acts as accumulator, to zero */
            sum = 0;

            /* Initiate the pointer pIn1 to point to the starting address of pSrcA */
            pIn1 = pInA;

            /* Matrix A columns number of MAC operations are to be performed */
            colCnt = numColsA;

            /* matrix multiplication */
            while (colCnt > 0U) {
                /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
                /* Perform the multiply-accumulates */
                sum += (q31_t)*pIn1++ * *pIn2;
                pIn2 += numColsB;

                /* Decrement the loop counter */
                colCnt--;
            }

            /* Convert the result from 34.30 to 1.15 format and store the saturated value in destination buffer */
            /* Saturate and store the result in the destination buffer */
            *px++ = (q7_t)__SSAT((sum >> 7), 8);

            /* Decrement the column loop counter */
            col--;

            /* Update the pointer pIn2 to point to the  starting address of the next column */
            pIn2 = pInB + (numColsB - col);

        } while (col > 0U);

        /* Update the pointer pSrcA to point to the  starting address of the next row */
        i = i + numColsB;
        pInA = pInA + numColsA;

        /* Decrement the row loop counter */
        row--;

    } while (row > 0U);

    /* set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;

    /* Return to application */
    return (status);
}

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
