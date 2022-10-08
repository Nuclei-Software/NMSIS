#include "ref.h"

riscv_status ref_mat_mult_f32(const riscv_matrix_instance_f32 *pSrcA,
                            const riscv_matrix_instance_f32 *pSrcB,
                            riscv_matrix_instance_f32 *pDst)
{
    uint32_t r, c, i, outR, outC, innerSize;
    float32_t sum;

    outR = pSrcA->numRows;
    outC = pSrcB->numCols;
    innerSize = pSrcA->numCols;

    for (r = 0; r < outR; r++) {
        for (c = 0; c < outC; c++) {
            sum = 0;

            for (i = 0; i < innerSize; i++) {
                sum += pSrcA->pData[r * innerSize + i] *
                       pSrcB->pData[i * outC + c];
            }

            pDst->pData[r * outC + c] = sum;
        }
    }

    return RISCV_MATH_SUCCESS;
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

            pDst->pData[r * outC + c] = ref_sat_q31(sum >> 31);
        }
    }

    return RISCV_MATH_SUCCESS;
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

riscv_status ref_mat_mult_opt_q31(
  const riscv_matrix_instance_q31 * pSrcA,
  const riscv_matrix_instance_q31 * pSrcB,
        riscv_matrix_instance_q31 * pDst)
{
  q31_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
  q31_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pInB = pSrcB->pData;                    /* Input data matrix pointer B */
  q31_t *pOut = pDst->pData;                     /* Output data matrix pointer */
  q31_t *px;                                     /* Temporary output data matrix pointer */
  q63_t sum;                                     /* Accumulator */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  uint32_t col, i = 0U, row = numRowsA, colCnt;  /* Loop counters */
  riscv_status status;                             /* Status of matrix multiplication */

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of row being processed */
      px = pOut + i;

      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pIn2 = pSrcB->pData;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0;

        /* Initialize pointer pIn1 to point to starting address of column being processed */
        pIn1 = pInA;

        /* Initialize cntCnt with number of columns */
        colCnt = numColsA;


        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform the multiply-accumulates */
          sum += (q63_t) *pIn1++ * *pIn2;
          pIn2 += numColsB;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Convert result from 2.62 to 1.31 format and store in destination buffer */
        *px++ = (q31_t) (sum >> 31);

        /* Decrement column loop counter */
        col--;

        /* Update pointer pIn2 to point to starting address of next column */
        pIn2 = pInB + (numColsB - col);

      } while (col > 0U);

      /* Update pointer pInA to point to starting address of next row */
      i = i + numColsB;
      pInA = pInA + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;


  /* Return to application */
  return (status);
}
