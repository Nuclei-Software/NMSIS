#include "ref.h"
#include "dsp/matrix_utils.h"

void ref_mat_vec_mult_f32(const riscv_matrix_instance_f32 *pSrcMat,
    const float32_t *pVec, float32_t *pDst) {
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float32_t *pSrcA = pSrcMat->pData;
    const float32_t *pInA; /* input data matrix pointer A of f32 type */
    const float32_t *pInVec; /* input data matrix pointer B of f32 type */
    float32_t *px; /* Temporary output data matrix pointer */
    uint16_t i, row, col; /* loop counters */
    float32_t matData, vecData;
    float32_t sum;
    i = 0u;
    px = pDst;

    for (row = 0; row < numRows; row++) {
      col = numCols;
      float32_t sum = 0.0f;
      pInVec = pVec;
      pInA = pSrcA + row * numCols;

      while (col > 0u) {
      vecData = *(pInVec)++;
      matData = *(pInA)++;
      sum += (float32_t) matData * vecData;
      col--;
      }

      *px++ = sum;
    }
}

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

riscv_status ref_mat_add_f32(const riscv_matrix_instance_f32 *pSrcA,
                           const riscv_matrix_instance_f32 *pSrcB,
                           riscv_matrix_instance_f32 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = pSrcA->pData[i] + pSrcB->pData[i];
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_sub_f32(const riscv_matrix_instance_f32 *pSrcA,
                           const riscv_matrix_instance_f32 *pSrcB,
                           riscv_matrix_instance_f32 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrcA->numRows * pSrcA->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = pSrcA->pData[i] - pSrcB->pData[i];
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_trans_f32(const riscv_matrix_instance_f32 *pSrc,
                             riscv_matrix_instance_f32 *pDst)
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

riscv_status ref_mat_scale_f32(const riscv_matrix_instance_f32 *pSrc,
                             float32_t scale, riscv_matrix_instance_f32 *pDst)
{
    uint32_t i;
    uint32_t numSamples; /* total number of elements in the matrix  */

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t)pSrc->numRows * pSrc->numCols;

    for (i = 0; i < numSamples; i++) {
        pDst->pData[i] = pSrc->pData[i] * scale;
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_inverse_f32(const riscv_matrix_instance_f32 * pSrc,
                                riscv_matrix_instance_f32 * pDst)
{
    float32_t *pIn = pSrc->pData;                  /* input data matrix pointer */
    float32_t *pOut = pDst->pData;                 /* output data matrix pointer */

    float32_t *pTmp;
    uint32_t numRows = pSrc->numRows;              /* Number of rows in the matrix  */
    uint32_t numCols = pSrc->numCols;              /* Number of Cols in the matrix  */


    float32_t pivot = 0.0f, newPivot=0.0f;                /* Temporary input values  */
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
          *pTmp++ = 0.0f;
          j--;
        }

        /* Writing all ones in the diagonal of the destination matrix */
        *pTmp++ = 1.0f;

        /* Writing all zeroes in upper triangle of the destination matrix */
        j = rowCnt - 1U;
        while (j > 0U)
        {
          *pTmp++ = 0.0f;
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

        /* Find maximum pivot in column */

        /* Loop over the number rows present below */

        for (rowNb = column+1; rowNb < numRows; rowNb++)
        {
            /* Update the input and destination pointers */
            pTmp = ELEM(pSrc,rowNb,column);
            newPivot = *pTmp;
            if (fabsf(newPivot) > fabsf(pivot))
            {
              selectedRow = rowNb;
              pivot = newPivot;
            }
        }

        /* Check if there is a non zero pivot element to
         * replace in the rows below */
        if ((pivot != 0.0f) && (selectedRow != column))
        {
            SWAP_ROWS_F32(pSrc,column, pivotRow,selectedRow);
            SWAP_ROWS_F32(pDst,0, pivotRow,selectedRow);

            /* Flag to indicate whether exchange is done or not */
            flag = 1U;
        }

        /* Update the status if the matrix is singular */
        if ((flag != 1U) && (pivot == 0.0f))
        {
            return RISCV_MATH_SINGULAR;
        }

        /* Pivot element of the row */
        pivot = 1.0f / pivot;

        SCALE_ROW_F32(pSrc,column,pivot,pivotRow);
        SCALE_ROW_F32(pDst,0,pivot,pivotRow);

        /* Replace the rows with the sum of that row and a multiple of row i
         * so that each new element in column i above row i is zero.*/

        rowNb = 0;
        for (;rowNb < pivotRow; rowNb++)
        {
            pTmp = ELEM(pSrc,rowNb,column) ;
            pivot = *pTmp;

            MAS_ROW_F32(column,pSrc,rowNb,pivot,pSrc,pivotRow);
            MAS_ROW_F32(0     ,pDst,rowNb,pivot,pDst,pivotRow);
        }

        for (rowNb = pivotRow + 1; rowNb < numRows; rowNb++)
        {
            pTmp = ELEM(pSrc,rowNb,column) ;
            pivot = *pTmp;

            MAS_ROW_F32(column,pSrc,rowNb,pivot,pSrc,pivotRow);
            MAS_ROW_F32(0     ,pDst,rowNb,pivot,pDst,pivotRow);
        }

    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;

    if ((flag != 1U) && (pivot == 0.0f))
    {
        pIn = pSrc->pData;
        for (i = 0; i < numRows * numCols; i++)
        {
            if (pIn[i] != 0.0f)
            break;
        }

        if (i == numRows * numCols)
            status = RISCV_MATH_SINGULAR;
    }

    /* Return to application */
    return (status);
}

riscv_status ref_mat_cmplx_mult_f32(const riscv_matrix_instance_f32 *pSrcA,
                                  const riscv_matrix_instance_f32 *pSrcB,
                                  riscv_matrix_instance_f32 *pDst)
{
    uint32_t r, c, i, outR, outC, innerSize;
    float32_t sumR, sumI;
    float32_t a0, b0, c0, d0;

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

                sumR += a0 * c0 - b0 * d0;
                sumI += b0 * c0 + a0 * d0;
            }

            pDst->pData[2 * (r * outC + c) + 0] = sumR;
            pDst->pData[2 * (r * outC + c) + 1] = sumI;
        }
    }

    return RISCV_MATH_SUCCESS;
}

riscv_status ref_mat_cholesky_f32(
  const riscv_matrix_instance_f32 * pSrc,
        riscv_matrix_instance_f32 * pDst)
{

  riscv_status status;                             /* status of matrix inverse */

  {
    int i,j,k;
    int n = pSrc->numRows;
    float32_t invSqrtVj;
    float32_t *pA,*pG;

    pA = pSrc->pData;
    pG = pDst->pData;


    for(i=0 ; i < n ; i++)
    {
       for(j=i ; j < n ; j++)
       {
          pG[j * n + i] = pA[j * n + i];

          for(k=0; k < i ; k++)
          {
             pG[j * n + i] = pG[j * n + i] - pG[i * n + k] * pG[j * n + k];
          }
       }

       if (pG[i * n + i] <= 0.0f)
       {
         return(RISCV_MATH_DECOMPOSITION_FAILURE);
       }

       invSqrtVj = 1.0f/sqrtf(pG[i * n + i]);
       for(j=i ; j < n ; j++)
       {
         pG[j * n + i] = pG[j * n + i] * invSqrtVj ;
       }
    }

    status = RISCV_MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}

riscv_status ref_mat_solve_upper_triangular_f32(
  const riscv_matrix_instance_f32 * ut,
  const riscv_matrix_instance_f32 * a,
  riscv_matrix_instance_f32 * dst)
{
riscv_status status;                             /* status of matrix inverse */
  {
    int i,j,k,n,cols;

    float32_t *pX = dst->pData;
    float32_t *pUT = ut->pData;
    float32_t *pA = a->pData;

    float32_t *ut_row;
    float32_t *a_col;

    n = dst->numRows;
    cols = dst->numCols;

    for(j=0; j < cols; j ++)
    {
       a_col = &pA[j];

       for(i=n-1; i >= 0 ; i--)
       {
            float32_t tmp=a_col[i * cols];

            ut_row = &pUT[n*i];

            for (k=n-1; k > i; k--)
            {
                tmp -= ut_row[k] * pX[cols*k+j];
            }

            if (ut_row[i]==0.0f)
            {
              return(RISCV_MATH_SINGULAR);
            }
            tmp = tmp / ut_row[i];
            pX[i*cols+j] = tmp;
       }

    }
    status = RISCV_MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}

riscv_status ref_mat_solve_lower_triangular_f32(
  const riscv_matrix_instance_f32 * lt,
  const riscv_matrix_instance_f32 * a,
  riscv_matrix_instance_f32 * dst)
  {
  riscv_status status;                             /* status of matrix inverse */
  {
    /* a1 b1 c1   x1 = a1
          b2 c2   x2   a2
             c3   x3   a3

    x3 = a3 / c3
    x2 = (a2 - c2 x3) / b2

    */
    int i,j,k,n,cols;

    float32_t *pX = dst->pData;
    float32_t *pLT = lt->pData;
    float32_t *pA = a->pData;

    float32_t *lt_row;
    float32_t *a_col;

    n = dst->numRows;
    cols = dst -> numCols;


    for(j=0; j < cols; j ++)
    {
       a_col = &pA[j];

       for(i=0; i < n ; i++)
       {
            float32_t tmp=a_col[i * cols];

            lt_row = &pLT[n*i];

            for(k=0; k < i; k++)
            {
                tmp -= lt_row[k] * pX[cols*k+j];
            }

            if (lt_row[i]==0.0f)
            {
              return(RISCV_MATH_SINGULAR);
            }
            tmp = tmp / lt_row[i];
            pX[i*cols+j] = tmp;
       }

    }
    status = RISCV_MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}

riscv_status ref_mat_ldlt_f32(
  const riscv_matrix_instance_f32 * pSrc,
  riscv_matrix_instance_f32 * pl,
  riscv_matrix_instance_f32 * pd,
  uint16_t * pp)
{

    riscv_status status;                             /* status of matrix inverse */

    const int n=pSrc->numRows;
    int fullRank = 1, diag,k;
    float32_t *pA;
    int row,d;

    memset(pd->pData,0,sizeof(float32_t)*n*n);
    memcpy(pl->pData,pSrc->pData,n*n*sizeof(float32_t));
    pA = pl->pData;

    for(k=0;k < n; k++)
    {
        pp[k] = k;
    }

    for(k=0;k < n; k++)
    {
        /* Find pivot */
        float32_t m=F32_MIN,a;
        int j=k;
        int r;

        for(r=k;r<n;r++)
        {
            if (pA[r*n+r] > m)
            {
                m = pA[r*n+r];
                j = r;
            }
        }

        if(j != k)
        {
            SWAP_ROWS_F32(pl,0,k,j);
            SWAP_COLS_F32(pl,0,k,j);
        }

        pp[k] = j;
        a = pA[k*n+k];

        if (fabsf(a) < 1.0e-8f)
        {
            fullRank = 0;
            break;
        }

        for(int w=k+1;w<n;w++)
        {
            int x;
            for(x=k+1;x<n;x++)
            {
                pA[w*n+x] = pA[w*n+x] - pA[w*n+k] * pA[x*n+k] / a;
            }
        }

        for(int w=k+1;w<n;w++)
        {
            pA[w*n+k] = pA[w*n+k] / a;
        }
    }

    diag=k;
    if (!fullRank)
    {
        diag--;
        for(row=0; row < n;row++)
        {
            int col;
            for(col=k; col < n;col++)
            {
                pl->pData[row*n+col]=0.0;
            }
        }
    }

    for(row=0; row < n;row++)
    {
        int col;
        for(col=row+1; col < n;col++)
        {
            pl->pData[row*n+col] = 0.0;
        }
    }

    for(d=0; d < diag;d++)
    {
        pd->pData[d*n+d] = pl->pData[d*n+d];
        pl->pData[d*n+d] = 1.0;
    }

    status = RISCV_MATH_SUCCESS;

    /* Return to application */
    return (status);
}

riscv_status ref_mat_cholesky_f64(
  const riscv_matrix_instance_f64 * pSrc,
        riscv_matrix_instance_f64 * pDst)
{
  riscv_status status;                             /* status of matrix inverse */

  {
    int i,j,k;
    int n = pSrc->numRows;
    float64_t invSqrtVj;
    float64_t *pA,*pG;

    pA = pSrc->pData;
    pG = pDst->pData;


    for(i=0 ; i < n ; i++)
    {
       for(j=i ; j < n ; j++)
       {
          pG[j * n + i] = pA[j * n + i];

          for(k=0; k < i ; k++)
          {
             pG[j * n + i] = pG[j * n + i] - pG[i * n + k] * pG[j * n + k];
          }
       }

       if (pG[i * n + i] <= 0.0f)
       {
         return(RISCV_MATH_DECOMPOSITION_FAILURE);
       }

       invSqrtVj = 1.0/sqrt(pG[i * n + i]);
       for(j=i ; j < n ; j++)
       {
         pG[j * n + i] = pG[j * n + i] * invSqrtVj ;
       }
    }

    status = RISCV_MATH_SUCCESS;

  }


  /* Return to application */
  return (status);
}

riscv_status ref_mat_solve_upper_triangular_f64(
  const riscv_matrix_instance_f64 * ut,
  const riscv_matrix_instance_f64 * a,
  riscv_matrix_instance_f64 * dst)
{
riscv_status status;                             /* status of matrix inverse */

  {

    int i, j, k, n, cols;

    float64_t *pX = dst->pData;
    float64_t *pUT = ut->pData;
    float64_t *pA = a->pData;

    float64_t *ut_row;
    float64_t *a_col;

    n = dst->numRows;
    cols = dst->numCols;

    for(j = 0; j < cols; j ++)
    {
       a_col = &pA[j];

       for(i = n-1; i >= 0 ; i--)
       {
            float64_t tmp = a_col[i * cols];

            ut_row = &pUT[n * i];


            for(k=n-1; k > i; k--)
            {
                tmp -= ut_row[k] * pX[cols * k + j];
            }

            if (ut_row[i] == 0.0f)
            {
              return(RISCV_MATH_SINGULAR);
            }
            tmp = tmp / ut_row[i];
            pX[i * cols + j] = tmp;
       }

    }
    status = RISCV_MATH_SUCCESS;

  }


  /* Return to application */
  return (status);
}

riscv_status ref_mat_solve_lower_triangular_f64(
  const riscv_matrix_instance_f64 * lt,
  const riscv_matrix_instance_f64 * a,
  riscv_matrix_instance_f64 * dst)
  {
  riscv_status status;                             /* status of matrix inverse */

  {
    /* a1 b1 c1   x1 = a1
          b2 c2   x2   a2
             c3   x3   a3

    x3 = a3 / c3
    x2 = (a2 - c2 x3) / b2

    */
    int i, j, k, n, cols;

    float64_t *pX = dst->pData;
    float64_t *pLT = lt->pData;
    float64_t *pA = a->pData;

    float64_t *lt_row;
    float64_t *a_col;

    n = dst->numRows;
    cols = dst->numCols;

    for(j = 0; j < cols; j ++)
    {
       a_col = &pA[j];

       for(i = 0; i < n ; i++)
       {
            float64_t tmp = a_col[i * cols];

            lt_row = &pLT[n * i];

            for(k = 0; k < i; k++)
            {
                tmp -= lt_row[k] * pX[cols * k + j];
            }

            if (lt_row[i] == 0.0f)
            {
              return(RISCV_MATH_SINGULAR);
            }
            tmp = tmp / lt_row[i];
            pX[i * cols + j] = tmp;
       }

    }
    status = RISCV_MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}

riscv_status ref_mat_ldlt_f64(
  const riscv_matrix_instance_f64 * pSrc,
  riscv_matrix_instance_f64 * pl,
  riscv_matrix_instance_f64 * pd,
  uint16_t * pp)
{

    riscv_status status;                             /* status of matrix inverse */
    const int n=pSrc->numRows;
    int fullRank = 1, diag,k;
    float64_t *pA;

    memset(pd->pData,0,sizeof(float64_t)*n*n);
    memcpy(pl->pData,pSrc->pData,n*n*sizeof(float64_t));
    pA = pl->pData;

    for(k=0;k < n; k++)
    {
        pp[k] = k;
    }

    for(k=0;k < n; k++)
    {
        /* Find pivot */
        float64_t m=F64_MIN,a;
        int r,j=k;

        for(r=k;r<n;r++)
        {
            if (pA[r*n+r] > m)
            {
                m = pA[r*n+r];
                j = r;
            }
        }

        if(j != k)
        {
            SWAP_ROWS_F64(pl,0,k,j);
            SWAP_COLS_F64(pl,0,k,j);
        }

        pp[k] = j;
        a = pA[k*n+k];

        if (fabs(a) < 1.0e-18L)
        {
            fullRank = 0;
            break;
        }

        for(int w=k+1;w<n;w++)
        {
            int x;
            for(x=k+1;x<n;x++)
            {
                pA[w*n+x] = pA[w*n+x] - pA[w*n+k] * pA[x*n+k] / a;
            }
        }

        for(int w=k+1;w<n;w++)
        {
            pA[w*n+k] = pA[w*n+k] / a;
        }
    }

    diag=k;
    if (!fullRank)
    {
        diag--;
        {
            int row;
            for(row=0; row < n;row++)
            {
                int col;
                for(col=k; col < n;col++)
                {
                    pl->pData[row*n+col]=0.0;
                }
            }
        }
    }

    int row;
    for(row=0; row < n;row++)
    {
        int col;
        for(col=row+1; col < n;col++)
        {
            pl->pData[row*n+col] = 0.0;
        }
    }

    int d;
    for(d=0; d < diag;d++)
    {
        pd->pData[d*n+d] = pl->pData[d*n+d];
        pl->pData[d*n+d] = 1.0;
    }

    status = RISCV_MATH_SUCCESS;

    /* Return to application */
    return (status);
}
