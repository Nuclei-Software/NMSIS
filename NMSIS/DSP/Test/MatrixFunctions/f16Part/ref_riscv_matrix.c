#include "ref.h"
#include "dsp/matrix_utils.h"

#if defined (RISCV_FLOAT16_SUPPORTED)
void ref_mat_vec_mult_f16(const riscv_matrix_instance_f16 *pSrcMat, const float16_t *pVec, float16_t *pDst)
{
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float16_t *pSrcA = pSrcMat->pData;
    const float16_t *pInA1;      /* input data matrix pointer A of Q31 type */
    const float16_t *pInVec;     /* input data matrix pointer B of Q31 type */
    float16_t *px;               /* Temporary output data matrix pointer */
    uint16_t i, row, colCnt; /* loop counters */
    float16_t matData, matData2, vecData, vecData2;


    /* Process 4 rows at a time */
    row = numRows;
    i = 0u;
    px = pDst;

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
            sum += (float16_t)matData * (float16_t)vecData;
            sum += (float16_t)matData2 * (float16_t)vecData2;
            colCnt--;
        }
        // process remainder of row
        colCnt = numCols & 1u;
        while (colCnt > 0) {
            sum += (float16_t)*pInA1++ * (float16_t)*pInVec++;
            colCnt--;
        }

        *px++ = sum;
        i = i + numCols;
        row--;
    }
}

riscv_status ref_mat_mult_f16(
  const riscv_matrix_instance_f16 * pSrcA,
  const riscv_matrix_instance_f16 * pSrcB,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pIn1 = pSrcA->pData;                /* Input data matrix pointer A */
  float16_t *pIn2 = pSrcB->pData;                /* Input data matrix pointer B */
  float16_t *pInA = pSrcA->pData;                /* Input data matrix pointer A */
  float16_t *pInB = pSrcB->pData;                /* Input data matrix pointer B */
  float16_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  float16_t *px;                                 /* Temporary output data matrix pointer */
  float16_t sum;                                 /* Accumulator */
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
      sum = 0.0f16;

      /* Initialize pointer pIn1 to point to starting address of column being processed */
      pIn1 = pInA;

      /* Initialize cntCnt with number of columns */
      colCnt = numColsA;

      while (colCnt > 0U)
      {
        /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

        /* Perform the multiply-accumulates */
        sum += (float16_t)*pIn1++ * (float16_t)*pIn2;
        pIn2 += numColsB;

        /* Decrement loop counter */
        colCnt--;
      }

      /* Store result in destination buffer */
      *px++ = sum;

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

riscv_status ref_mat_add_f16(
  const riscv_matrix_instance_f16 * pSrcA,
  const riscv_matrix_instance_f16 * pSrcB,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pInA = pSrcA->pData;                /* input data matrix pointer A */
  float16_t *pInB = pSrcB->pData;                /* input data matrix pointer B */
  float16_t *pOut = pDst->pData;                 /* output data matrix pointer */

  uint32_t numSamples;                           /* total number of elements in the matrix */
  uint32_t blkCnt;                               /* loop counters */
  riscv_status status;                             /* status of matrix addition */

  /* Total number of samples in input matrix */
  numSamples = (uint32_t) pSrcA->numRows * pSrcA->numCols;

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

  while (blkCnt > 0U)
  {
    /* C(m,n) = A(m,n) + B(m,n) */

    /* Add and store result in destination buffer. */
    *pOut++ = (float16_t)*pInA++ + (float16_t)*pInB++;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;


  /* Return to application */
  return (status);
}

riscv_status ref_mat_sub_f16(
  const riscv_matrix_instance_f16 * pSrcA,
  const riscv_matrix_instance_f16 * pSrcB,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pInA = pSrcA->pData;                /* input data matrix pointer A */
  float16_t *pInB = pSrcB->pData;                /* input data matrix pointer B */
  float16_t *pOut = pDst->pData;                 /* output data matrix pointer */

  uint32_t numSamples;                           /* total number of elements in the matrix */
  uint32_t blkCnt;                               /* loop counters */
  riscv_status status;                             /* status of matrix subtraction */

  /* Total number of samples in input matrix */
  numSamples = (uint32_t) pSrcA->numRows * pSrcA->numCols;

  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

  while (blkCnt > 0U)
  {
    /* C(m,n) = A(m,n) - B(m,n) */

    /* Subtract and store result in destination buffer. */
    *pOut++ = (float16_t)(*pInA++) - (float16_t)(*pInB++);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;

  /* Return to application */
  return (status);
}

riscv_status ref_mat_trans_f16(
  const riscv_matrix_instance_f16 * pSrc,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pIn = pSrc->pData;                  /* input data matrix pointer */
  float16_t *pOut = pDst->pData;                 /* output data matrix pointer */
  float16_t *px;                                 /* Temporary output data matrix pointer */
  uint16_t nRows = pSrc->numRows;                /* number of rows */
  uint16_t nCols = pSrc->numCols;                /* number of columns */
  uint32_t col, row = nRows, i = 0U;             /* Loop counters */
  riscv_status status;                             /* status of matrix transpose */

  /* Matrix transpose by exchanging the rows with columns */
  /* row loop */
  do
  {
    /* Pointer px is set to starting address of column being processed */
    px = pOut + i;

    /* Initialize col with number of samples */
    col = nCols;

    while (col > 0U)
    {
      /* Read and store input element in destination */
      *px = *pIn++;

      /* Update pointer px to point to next row of transposed matrix */
      px += nRows;

      /* Decrement column loop counter */
      col--;
    }

    i++;

    /* Decrement row loop counter */
    row--;

  } while (row > 0U);          /* row loop end */

  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;

  /* Return to application */
  return (status);
}

riscv_status ref_mat_scale_f16(
  const riscv_matrix_instance_f16 * pSrc,
        float16_t                 scale,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pIn = pSrc->pData;                  /* Input data matrix pointer */
  float16_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  uint32_t numSamples;                           /* Total number of elements in the matrix */
  uint32_t blkCnt;                               /* Loop counters */
  riscv_status status;                             /* Status of matrix scaling */

  /* Total number of samples in input matrix */
  numSamples = (uint32_t) pSrc->numRows * pSrc->numCols;
  /* Initialize blkCnt with number of samples */
  blkCnt = numSamples;

  while (blkCnt > 0U)
  {
    /* C(m,n) = A(m,n) * scale */

    /* Scale and store result in destination buffer. */
    *pOut++ = (_Float16)(*pIn++) * (_Float16)scale;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS; 

  /* Return to application */
  return (status);
}

riscv_status ref_mat_inverse_f16(
  const riscv_matrix_instance_f16 * pSrc,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pIn = pSrc->pData;                  /* input data matrix pointer */
  float16_t *pOut = pDst->pData;                 /* output data matrix pointer */

  float16_t *pTmp;
  uint32_t numRows = pSrc->numRows;              /* Number of rows in the matrix  */
  uint32_t numCols = pSrc->numCols;              /* Number of Cols in the matrix  */

  float16_t pivot = 0.0f16, newPivot=0.0f16;                /* Temporary input values  */
  uint32_t selectedRow,pivotRow,i, rowNb, rowCnt, flag = 0U, j,column;      /* loop counters */
  riscv_status status;                             /* status of matrix inverse */

  /* Working pointer for destination matrix */
  pTmp = pOut;

  /* Loop over the number of rows */
  rowCnt = numRows;

  /* Making the destination matrix as identity matrix */
  while (rowCnt > 0U)
  {
    /* Writing all zeroes in lower triangle of the destination matrix */
    j = numRows - rowCnt;
    while (j > 0U)
    {
      *pTmp++ = 0.0f16;
      j--;
    }

    /* Writing all ones in the diagonal of the destination matrix */
    *pTmp++ = 1.0f16;

    /* Writing all zeroes in upper triangle of the destination matrix */
    j = rowCnt - 1U;
    while (j > 0U)
    {
      *pTmp++ = 0.0f16;
      j--;
    }

    /* Decrement loop counter */
    rowCnt--;
  }

  /* Loop over the number of columns of the input matrix.
     All the elements in each column are processed by the row operations */

  /* Index modifier to navigate through the columns */
  for(column = 0U; column < numCols; column++)
  {
    /* Check if the pivot element is zero..
     * If it is zero then interchange the row with non zero row below.
     * If there is no non zero element to replace in the rows below,
     * then the matrix is Singular. */

    pivotRow = column;

    /* Temporary variable to hold the pivot value */
    pTmp = ELEM(pSrc,column,column) ;
    pivot = *pTmp;
    selectedRow = column;

    /* Loop over the number rows present below */
    for (rowNb = column+1; rowNb < numRows; rowNb++)
    {
      /* Update the input and destination pointers */
      pTmp = ELEM(pSrc,rowNb,column);
      newPivot = *pTmp;
      if (fabsf((float32_t)newPivot) > fabsf((float32_t)pivot))
      {
        selectedRow = rowNb;
        pivot = newPivot;
      }

    }

    /* Check if there is a non zero pivot element to
     * replace in the rows below */
    if (((float16_t)pivot != 0.0f16) && (selectedRow != column))
    {
      /* Loop over number of columns
       * to the right of the pilot element */
      SWAP_ROWS_F16(pSrc,column, pivotRow,selectedRow);
      SWAP_ROWS_F16(pDst,0, pivotRow,selectedRow);
      /* Flag to indicate whether exchange is done or not */
      flag = 1U;
    }


    /* Update the status if the matrix is singular */
    if ((flag != 1U) && ((float16_t)pivot == 0.0f16))
    {
      return RISCV_MATH_SINGULAR;
    }

    /* Pivot element of the row */
    pivot = 1.0f16 / (float16_t)pivot;

    SCALE_ROW_F16(pSrc,column,pivot,pivotRow);
    SCALE_ROW_F16(pDst,0,pivot,pivotRow);

    /* Replace the rows with the sum of that row and a multiple of row i
     * so that each new element in column i above row i is zero.*/

    rowNb = 0;
    for (;rowNb < pivotRow; rowNb++)
    {
         pTmp = ELEM(pSrc,rowNb,column) ;
         pivot = *pTmp;

         MAS_ROW_F16(column,pSrc,rowNb,pivot,pSrc,pivotRow);
         MAS_ROW_F16(0     ,pDst,rowNb,pivot,pDst,pivotRow);
    }

    for (rowNb = pivotRow + 1; rowNb < numRows; rowNb++)
    {
      pTmp = ELEM(pSrc,rowNb,column) ;
      pivot = *pTmp;
	  
      MAS_ROW_F16(column,pSrc,rowNb,pivot,pSrc,pivotRow);
      MAS_ROW_F16(0     ,pDst,rowNb,pivot,pDst,pivotRow);
    }
  }

  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;

  if ((flag != 1U) && ((float16_t)pivot == 0.0f16))
  {
    pIn = pSrc->pData;
    for (i = 0; i < numRows * numCols; i++)
    {
      if ((float16_t)pIn[i] != 0.0f16)
          break;
    }

    if (i == numRows * numCols)
      status = RISCV_MATH_SINGULAR;
  }

  /* Return to application */
  return (status);
}

riscv_status ref_mat_cmplx_mult_f16(
  const riscv_matrix_instance_f16 * pSrcA,
  const riscv_matrix_instance_f16 * pSrcB,
        riscv_matrix_instance_f16 * pDst)
{
  float16_t *pIn1 = pSrcA->pData;                /* Input data matrix pointer A */
  float16_t *pIn2 = pSrcB->pData;                /* Input data matrix pointer B */
  float16_t *pInA = pSrcA->pData;                /* Input data matrix pointer A */
  float16_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  float16_t *px;                                 /* Temporary output data matrix pointer */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  float16_t sumReal, sumImag;                    /* Accumulator */
  float16_t a1, b1, c1, d1;
  uint32_t col, i = 0U, j, row = numRowsA, colCnt; /* loop counters */
  riscv_status status;                             /* status of matrix multiplication */

  /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
  /* row loop */
  do
  {
    /* Output pointer is set to starting address of the row being processed */
    px = pOut + 2 * i;

    /* For every row wise process, the column loop counter is to be initiated */
    col = numColsB;

    /* For every row wise process, the pIn2 pointer is set
     ** to the starting address of the pSrcB data */
    pIn2 = pSrcB->pData;

    j = 0U;

    /* column loop */
    do
    {
      /* Set the variable sum, that acts as accumulator, to zero */
      sumReal = 0.0f16;
      sumImag = 0.0f16;

      /* Initiate pointer pIn1 to point to starting address of column being processed */
      pIn1 = pInA;

      /* Initialize blkCnt with number of samples */
      colCnt = numColsA;

      while (colCnt > 0U)
      {
        /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
        a1 = *(pIn1     );
        c1 = *(pIn2     );
        b1 = *(pIn1 + 1U);
        d1 = *(pIn2 + 1U);

        /* Multiply and Accumlates */
        sumReal += a1 * c1;
        sumImag += b1 * c1;

        /* update pointers */
        pIn1 += 2U;
        pIn2 += 2 * numColsB;

        /* Multiply and Accumlates */
        sumReal -= b1 * d1;
        sumImag += a1 * d1;

        /* Decrement loop counter */
        colCnt--;
      }

      /* Store result in destination buffer */
      *px++ = sumReal;
      *px++ = sumImag;

      /* Update pointer pIn2 to point to starting address of next column */
      j++;
      pIn2 = pSrcB->pData + 2U * j;

      /* Decrement column loop counter */
      col--;

    } while (col > 0U);

    /* Update pointer pInA to point to starting address of next row */
    i = i + numColsB;
    pInA = pInA + 2 * numColsA;

    /* Decrement row loop counter */
    row--;

  } while (row > 0U);

  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;

  /* Return to application */
  return (status);
}

riscv_status ref_mat_cholesky_f16(
  const riscv_matrix_instance_f16 * pSrc,
        riscv_matrix_instance_f16 * pDst)
{
  riscv_status status;                             /* status of matrix inverse */
  int i,j,k;
  int n = pSrc->numRows;
  float16_t invSqrtVj;
  float16_t *pA,*pG;

  pA = pSrc->pData;
  pG = pDst->pData;
  
  for(i=0 ; i < n ; i++)
  {
     for(j=i ; j < n ; j++)
     {
        pG[j * n + i] = pA[j * n + i];

        for(k=0; k < i ; k++)
        {
           pG[j * n + i] = (float16_t)pG[j * n + i] - (float16_t)pG[i * n + k] * (float16_t)pG[j * n + k];
        }
     }

     if ((float16_t)pG[i * n + i] <= 0.0f16)
     {
       return(RISCV_MATH_DECOMPOSITION_FAILURE);
     }

     /* The division is done in float32 for accuracy reason and
     because doing it in f16 would not have any impact on the performances.
     */
     invSqrtVj = 1.0f/sqrtf((float32_t)pG[i * n + i]);
     SCALE_COL_F16(pDst,i,invSqrtVj,i);
  }

  status = RISCV_MATH_SUCCESS;

  /* Return to application */
  return (status);
}

riscv_status ref_mat_solve_upper_triangular_f16(
  const riscv_matrix_instance_f16 * ut,
  const riscv_matrix_instance_f16 * a,
  riscv_matrix_instance_f16 * dst)
{
  riscv_status status;                 /* status of matrix inverse */

  int i,j,k,n,cols;

  n = dst->numRows;
  cols = dst->numCols;

  float16_t *pX = dst->pData;
  float16_t *pUT = ut->pData;
  float16_t *pA = a->pData;

  float16_t *ut_row;
  float16_t *a_col;

  for(j=0; j < cols; j ++)
  {
     a_col = &pA[j];

     for(i=n-1; i >= 0 ; i--)
     {
        ut_row = &pUT[n*i];
	    
        float16_t tmp=a_col[i * cols];
	    
        for(k=n-1; k > i; k--)
        {
           tmp -= (float16_t)ut_row[k] * (float16_t)pX[cols*k+j];
        }
	    
        if ((float16_t)ut_row[i]==0.0f16)
        {
           return(RISCV_MATH_SINGULAR);
        }
        tmp = (float16_t)tmp / (float16_t)ut_row[i];
        pX[i*cols+j] = tmp;
     }

  }
  status = RISCV_MATH_SUCCESS;

  /* Return to application */
  return (status);
}

riscv_status ref_mat_solve_lower_triangular_f16(
  const riscv_matrix_instance_f16 * lt,
  const riscv_matrix_instance_f16 * a,
  riscv_matrix_instance_f16 * dst)
{
   riscv_status status;                             /* status of matrix inverse */
   /* a1 b1 c1   x1 = a1
         b2 c2   x2   a2
            c3   x3   a3
   
   x3 = a3 / c3
   x2 = (a2 - c2 x3) / b2
   
   */
   int i,j,k,n,cols;
   
   n = dst->numRows;
   cols = dst->numCols;
   
   float16_t *pX = dst->pData;
   float16_t *pLT = lt->pData;
   float16_t *pA = a->pData;
   
   float16_t *lt_row;
   float16_t *a_col;
   
   for(j=0; j < cols; j ++)
   {
      a_col = &pA[j];
   
      for(i=0; i < n ; i++)
      {
         lt_row = &pLT[n*i];
   
         float16_t tmp=a_col[i * cols];
   
         for(k=0; k < i; k++)
         {
            tmp -= (float16_t)lt_row[k] * (float16_t)pX[cols*k+j];
         }
   
         if ((float16_t)lt_row[i]==0.0f16)
         {
            return(RISCV_MATH_SINGULAR);
         }
         tmp = (float16_t)tmp / (float16_t)lt_row[i];
         pX[i*cols+j] = tmp;
      }
   }
   status = RISCV_MATH_SUCCESS;
   
   /* Return to application */
   return (status);
}

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
