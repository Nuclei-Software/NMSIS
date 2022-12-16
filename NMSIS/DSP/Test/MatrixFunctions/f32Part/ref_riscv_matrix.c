#include "ref.h"

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

riscv_status ref_mat_inverse_f32(const riscv_matrix_instance_f32 *pSrc,
                               riscv_matrix_instance_f32 *pDst)
{
    float32_t *pIn = pSrc->pData;                  /* input data matrix pointer */
    float32_t *pOut = pDst->pData;                 /* output data matrix pointer */
    float32_t *pInT1, *pInT2;                      /* Temporary input data matrix pointer */
    float32_t *pOutT1, *pOutT2;                    /* Temporary output data matrix pointer */
    float32_t *pPivotRowIn, *pPRT_in, *pPivotRowDst, *pPRT_pDst;  /* Temporary input and output data matrix pointer */
    uint32_t numRows = pSrc->numRows;              /* Number of rows in the matrix  */
    uint32_t numCols = pSrc->numCols;              /* Number of Cols in the matrix  */
    float32_t Xchg, in = 0.0f, in1;                /* Temporary input values  */
    uint32_t i, rowCnt, flag = 0U, j, loopCnt, k,l;      /* loop counters */
    riscv_status status;

    /* Working pointer for destination matrix */
    pOutT1 = pOut;

    /* Loop over the number of rows */
    rowCnt = numRows;

    /* Making the destination matrix as identity matrix */
    while (rowCnt > 0U)
    {
      /* Writing all zeroes in lower triangle of the destination matrix */
      j = numRows - rowCnt;
      while (j > 0U)
      {
        *pOutT1++ = 0.0f;
        j--;
      }

      /* Writing all ones in the diagonal of the destination matrix */
      *pOutT1++ = 1.0f;

      /* Writing all zeroes in upper triangle of the destination matrix */
      j = rowCnt - 1U;
      while (j > 0U)
      {
        *pOutT1++ = 0.0f;
        j--;
      }

      /* Decrement loop counter */
      rowCnt--;
    }

    /* Loop over the number of columns of the input matrix.
       All the elements in each column are processed by the row operations */
    loopCnt = numCols;

    /* Index modifier to navigate through the columns */
    l = 0U;

    while (loopCnt > 0U)
    {
      /* Check if the pivot element is zero..
       * If it is zero then interchange the row with non zero row below.
       * If there is no non zero element to replace in the rows below,
       * then the matrix is Singular. */

      /* Working pointer for the input matrix that points
       * to the pivot element of the particular row  */
      pInT1 = pIn + (l * numCols);

      /* Working pointer for the destination matrix that points
       * to the pivot element of the particular row  */
      pOutT1 = pOut + (l * numCols);

      /* Temporary variable to hold the pivot value */
      in = *pInT1;



      /* Check if the pivot element is zero */
      if (*pInT1 == 0.0f)
      {
        /* Loop over the number rows present below */

        for (i = 1U; i < numRows - l; i++)
        {
          /* Update the input and destination pointers */
          pInT2 = pInT1 + (numCols * i);
          pOutT2 = pOutT1 + (numCols * i);

          /* Check if there is a non zero pivot element to
           * replace in the rows below */
          if (*pInT2 != 0.0f)
          {
            /* Loop over number of columns
             * to the right of the pilot element */
            j = numCols - l;

            while (j > 0U)
            {
              /* Exchange the row elements of the input matrix */
              Xchg = *pInT2;
              *pInT2++ = *pInT1;
              *pInT1++ = Xchg;

              /* Decrement the loop counter */
              j--;
            }

            /* Loop over number of columns of the destination matrix */
            j = numCols;

            while (j > 0U)
            {
              /* Exchange the row elements of the destination matrix */
              Xchg = *pOutT2;
              *pOutT2++ = *pOutT1;
              *pOutT1++ = Xchg;

              /* Decrement loop counter */
              j--;
            }

            /* Flag to indicate whether exchange is done or not */
            flag = 1U;

            /* Break after exchange is done */
            break;
          }


          /* Decrement loop counter */
        }
      }

      /* Update the status if the matrix is singular */
      if ((flag != 1U) && (in == 0.0f))
      {
        return RISCV_MATH_SINGULAR;
      }

      /* Points to the pivot row of input and destination matrices */
      pPivotRowIn = pIn + (l * numCols);
      pPivotRowDst = pOut + (l * numCols);

      /* Temporary pointers to the pivot row pointers */
      pInT1 = pPivotRowIn;
      pInT2 = pPivotRowDst;

      /* Pivot element of the row */
      in = *pPivotRowIn;

      /* Loop over number of columns
       * to the right of the pilot element */
      j = (numCols - l);

      while (j > 0U)
      {
        /* Divide each element of the row of the input matrix
         * by the pivot element */
        in1 = *pInT1;
        *pInT1++ = in1 / in;

        /* Decrement the loop counter */
        j--;
      }

      /* Loop over number of columns of the destination matrix */
      j = numCols;

      while (j > 0U)
      {
        /* Divide each element of the row of the destination matrix
         * by the pivot element */
        in1 = *pInT2;
        *pInT2++ = in1 / in;

        /* Decrement the loop counter */
        j--;
      }

      /* Replace the rows with the sum of that row and a multiple of row i
       * so that each new element in column i above row i is zero.*/

      /* Temporary pointers for input and destination matrices */
      pInT1 = pIn;
      pInT2 = pOut;

      /* index used to check for pivot element */
      i = 0U;

      /* Loop over number of rows */
      /*  to be replaced by the sum of that row and a multiple of row i */
      k = numRows;

      while (k > 0U)
      {
        /* Check for the pivot element */
        if (i == l)
        {
          /* If the processing element is the pivot element,
             only the columns to the right are to be processed */
          pInT1 += numCols - l;

          pInT2 += numCols;
        }
        else
        {
          /* Element of the reference row */
          in = *pInT1;

          /* Working pointers for input and destination pivot rows */
          pPRT_in = pPivotRowIn;
          pPRT_pDst = pPivotRowDst;

          /* Loop over the number of columns to the right of the pivot element,
             to replace the elements in the input matrix */
          j = (numCols - l);

          while (j > 0U)
          {
            /* Replace the element by the sum of that row
               and a multiple of the reference row  */
            in1 = *pInT1;
            *pInT1++ = in1 - (in * *pPRT_in++);

            /* Decrement the loop counter */
            j--;
          }

          /* Loop over the number of columns to
             replace the elements in the destination matrix */
          j = numCols;

          while (j > 0U)
          {
            /* Replace the element by the sum of that row
               and a multiple of the reference row  */
            in1 = *pInT2;
            *pInT2++ = in1 - (in * *pPRT_pDst++);

            /* Decrement loop counter */
            j--;
          }

        }

        /* Increment temporary input pointer */
        pInT1 = pInT1 + l;

        /* Decrement loop counter */
        k--;

        /* Increment pivot index */
        i++;
      }

      /* Increment the input pointer */
      pIn++;

      /* Decrement the loop counter */
      loopCnt--;

      /* Increment the index modifier */
      l++;
    }

    return RISCV_MATH_SUCCESS;
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

/// @private
#define SWAP_ROWS_F32(A,i,j)     \
  for(int w=0;w < n; w++)    \
  {                          \
     float32_t tmp;          \
     tmp = A[i*n + w];       \
     A[i*n + w] = A[j*n + w];\
     A[j*n + w] = tmp;       \
  }

/// @private
#define SWAP_COLS_F32(A,i,j)     \
  for(int w=0;w < n; w++)    \
  {                          \
     float32_t tmp;          \
     tmp = A[w*n + i];       \
     A[w*n + i] = A[w*n + j];\
     A[w*n + j] = tmp;       \
  }

riscv_status ref_mat_ldlt_f32(
  const riscv_matrix_instance_f32 * pSrc,
  riscv_matrix_instance_f32 * pl,
  riscv_matrix_instance_f32 * pd,
  uint16_t * pp)
{

  riscv_status status;                             /* status of matrix inverse */
  {

    const int n=pSrc->numRows;
    int fullRank = 1, diag,k;
    float32_t *pA;

    memset(pd->pData,0,sizeof(float32_t)*n*n);
    memcpy(pl->pData,pSrc->pData,n*n*sizeof(float32_t));
    pA = pl->pData;

    for(int k=0;k < n; k++)
    {
      pp[k] = k;
    }


    for(k=0;k < n; k++)
    {
        /* Find pivot */
        float32_t m=F32_MIN,a;
        int j=k;


        for(int r=k;r<n;r++)
        {
           if (pA[r*n+r] > m)
           {
             m = pA[r*n+r];
             j = r;
           }
        }

        if(j != k)
        {
          SWAP_ROWS_F32(pA,k,j);
          SWAP_COLS_F32(pA,k,j);
        }


        pp[k] = j;

        a = pA[k*n+k];

        if (fabs(a) < 1.0e-8)
        {

            fullRank = 0;
            break;
        }

        for(int w=k+1;w<n;w++)
        {
          for(int x=k+1;x<n;x++)
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
      for(int row=0; row < n;row++)
      {
        for(int col=k; col < n;col++)
        {
           pl->pData[row*n+col]=0.0;
        }
      }
    }

    for(int row=0; row < n;row++)
    {
       for(int col=row+1; col < n;col++)
       {
         pl->pData[row*n+col] = 0.0;
       }
    }

    for(int d=0; d < diag;d++)
    {
      pd->pData[d*n+d] = pl->pData[d*n+d];
      pl->pData[d*n+d] = 1.0;
    }

    status = RISCV_MATH_SUCCESS;

  }


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

/// @private
#define SWAP_ROWS_F64(A,i,j)     \
  for(int w=0;w < n; w++)    \
  {                          \
     float64_t tmp;          \
     tmp = A[i*n + w];       \
     A[i*n + w] = A[j*n + w];\
     A[j*n + w] = tmp;       \
  }
/// @private
#define SWAP_COLS_F64(A,i,j)     \
  for(int w=0;w < n; w++)    \
  {                          \
     float64_t tmp;          \
     tmp = A[w*n + i];       \
     A[w*n + i] = A[w*n + j];\
     A[w*n + j] = tmp;       \
  }
riscv_status ref_mat_ldlt_f64(
  const riscv_matrix_instance_f64 * pSrc,
  riscv_matrix_instance_f64 * pl,
  riscv_matrix_instance_f64 * pd,
  uint16_t * pp)
{

  riscv_status status;                             /* status of matrix inverse */
  {

    const int n=pSrc->numRows;
    int fullRank = 1, diag,k;
    float64_t *pA;

    memset(pd->pData,0,sizeof(float64_t)*n*n);

    memcpy(pl->pData,pSrc->pData,n*n*sizeof(float64_t));
    pA = pl->pData;

    for(int k=0;k < n; k++)
    {
      pp[k] = k;
    }


    for(k=0;k < n; k++)
    {
        /* Find pivot */
        float64_t m=F64_MIN,a;
        int j=k;


        for(int r=k;r<n;r++)
        {
           if (pA[r*n+r] > m)
           {
             m = pA[r*n+r];
             j = r;
           }
        }

        if(j != k)
        {
          SWAP_ROWS_F64(pA,k,j);
          SWAP_COLS_F64(pA,k,j);
        }


        pp[k] = j;

        a = pA[k*n+k];

        if (fabs(a) < 1.0e-18)
        {

            fullRank = 0;
            break;
        }

        for(int w=k+1;w<n;w++)
        {
          for(int x=k+1;x<n;x++)
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
      for(int row=0; row < n;row++)
      {
        for(int col=k; col < n;col++)
        {
           pl->pData[row*n+col]=0.0;
        }
      }
    }

    for(int row=0; row < n;row++)
    {
       for(int col=row+1; col < n;col++)
       {
         pl->pData[row*n+col] = 0.0;
       }
    }

    for(int d=0; d < diag;d++)
    {
      pd->pData[d*n+d] = pl->pData[d*n+d];
      pl->pData[d*n+d] = 1.0;
    }

    status = RISCV_MATH_SUCCESS;

  }


  /* Return to application */
  return (status);
}
