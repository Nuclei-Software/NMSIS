/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_mult_fast_q15.c
 * Description:  Q15 matrix multiplication (fast variant)
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: RISC-V Cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/matrix_functions.h"

/**
  @ingroup groupMatrix
 */

/**
  @addtogroup MatrixMult
  @{
 */

/**
  @brief         Q15 matrix multiplication (fast variant).
  @param[in]     pSrcA      points to the first input matrix structure
  @param[in]     pSrcB      points to the second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @param[in]     pState     points to the array for storing intermediate results
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The difference between the function \ref riscv_mat_mult_q15() and this fast variant is that
                   the fast variant use a 32-bit rather than a 64-bit accumulator.
                   The result of each 1.15 x 1.15 multiplication is truncated to
                   2.30 format. These intermediate results are accumulated in a 32-bit register in 2.30
                   format. Finally, the accumulator is saturated and converted to a 1.15 result.
  @par
                   The fast version has the same overflow behavior as the standard version but provides
                   less precision since it discards the low 16 bits of each multiplication result.
                   In order to avoid overflows completely the input signals must be scaled down.
                   Scale down one of the input matrices by log2(numColsA) bits to avoid overflows,
                   as a total of numColsA additions are computed internally for each output element.
  @remark
                   Refer to \ref riscv_mat_mult_q15() for a slower implementation of this function
                   which uses 64-bit accumulation to provide higher precision.
 */

RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_mult_fast_q15(
  const riscv_matrix_instance_q15 * pSrcA,
  const riscv_matrix_instance_q15 * pSrcB,
        riscv_matrix_instance_q15 * pDst,
        q15_t                   * pState)
{
        q31_t sum;                                     /* Accumulator */
        q15_t *pSrcBT = pState;                        /* Input data matrix pointer for transpose */
        q15_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A of Q15 type */
        q15_t *pInB = pSrcB->pData;                    /* Input data matrix pointer B of Q15 type */
        q15_t *px;                                     /* Temporary output data matrix pointer */
        uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
        uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
        uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
        uint16_t numRowsB = pSrcB->numRows;            /* Number of rows of input matrix B */
        uint32_t col, i = 0U, row = numRowsB, colCnt;  /* Loop counters */
        riscv_status status;                             /* Status of matrix multiplication */

#if defined(RISCV_MATH_VECTOR)
    q15_t in;                                      /* Temporary variable to hold the input value */
    q15_t inA1, inB1, inA2, inB2;
    uint16_t blkCnt;
    size_t l;
    ptrdiff_t bstride;
    uint16_t colnum, rownum;
    vint16m4_t v_inA, v_inB;
    vint32m8_t v_sum;

#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrcA->numCols != pSrcB->numRows) ||
      (pSrcA->numRows != pDst->numRows)  ||
      (pSrcB->numCols != pDst->numCols)    )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */

  {
    /* Matrix transpose */
    do
    {
      /* The pointer px is set to starting address of column being processed */
      px = pSrcBT + i;

      col = numColsB;
      blkCnt = col;
      bstride = numRowsB * 2;
      for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
      {
        __riscv_vsse16_v_i16m8(px, bstride, __riscv_vle16_v_i16m8(pInB, l), l);
        px += l * numRowsB;
        pInB += l;
      }

      i++;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Reset variables for usage in following multiplication process */
    row = numRowsA;
    i = 0U;
    px = pDst->pData;

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    while (row > 0U)
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of transposed pSrcB data */
      pInB = pSrcBT;

      /* column loop */
      while (col > 0U)
      {
        /* Set variable sum, that acts as accumulator, to zero */
        sum = 0;

        /* Initiate pointer pInA to point to starting address of column being processed */
        pInA = pSrcA->pData + i;

        /* process odd column samples */
        colCnt = numColsA;

        blkCnt = colCnt;
        l = __riscv_vsetvl_e16m4(blkCnt);
        vint32m1_t v_sum = __riscv_vsub_vv_i32m1(v_sum, v_sum, l);
        for (; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)   // Multiply a row by a column
        {
          v_inA = __riscv_vle16_v_i16m4(pInA, l);
          pInA += l;
          v_inB = __riscv_vle16_v_i16m4(pInB, l);
          pInB += l;
          v_sum = __riscv_vredsum_vs_i32m8_i32m1(__riscv_vwmul_vv_i32m8(v_inA, v_inB, l), v_sum, l);
        }
        sum = __riscv_vmv_x_s_i32m1_i32(v_sum);

        /* Store result in destination buffer */
        *px++  = (q15_t) (sum >> 15);

        /* Decrement column loop counter */
        col--;

      }

      i = i + numColsA;

      /* Decrement row loop counter */
      row--;

    }
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }
#else
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
        q63_t in64;                                      /* Temporary variable to hold the input value */
        q63_t sum64;
        q63_t inA164, inB164;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
#if defined (RISCV_MATH_DSP)
        q31_t in;                                      /* Temporary variable to hold the input value */
        q31_t inA1, inB1, inA2, inB2;
        q31_t sum2, sum3, sum4;
        q15_t *pInA2, *pInB2, *px2;
        uint32_t j = 0;
#else
        q15_t in;                                      /* Temporary variable to hold the input value */
        q15_t inA1, inB1, inA2, inB2;
#endif /* defined (RISCV_MATH_DSP) */

#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrcA->numCols != pSrcB->numRows) ||
      (pSrcA->numRows != pDst->numRows)  ||
      (pSrcB->numCols != pDst->numCols)    )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */

  {
    /* Matrix transpose */
    do
    {
      /* The pointer px is set to starting address of column being processed */
      px = pSrcBT + i;

      /* Apply loop unrolling and exchange columns with row elements */
      col = numColsB >> 2U;

      /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
       ** a second loop below computes the remaining 1 to 3 samples. */
      while (col > 0U)
      {

#if defined (RISCV_MATH_DSP)
        /* Read two elements from row */
        in = read_q15x2_ia (&pInB);

        /* Unpack and store one element in destination */
        *px = (q15_t) in;

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB;

        /* Unpack and store second element in destination */
        *px = (q15_t) ((in & (q31_t) 0xffff0000) >> 16);

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB;

        in = read_q15x2_ia (&pInB);
        *px = (q15_t) in;
        px += numRowsB;

        *px = (q15_t) ((in & (q31_t) 0xffff0000) >> 16);
        px += numRowsB;
#else /* #if defined (RISCV_MATH_DSP) */

        /* Read one element from row */
        in = *pInB++;

        /* Store one element in destination */
        *px = in;

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB;

        in = *pInB++;
        *px = in;
        px += numRowsB;

        in = *pInB++;
        *px = in;
        px += numRowsB;

        in = *pInB++;
        *px = in;
        px += numRowsB;

#endif /* #if defined (RISCV_MATH_DSP) */

        /* Decrement column loop counter */
        col--;
      }

      /* If the columns of pSrcB is not a multiple of 4, compute any remaining output samples here.
       ** No loop unrolling is used. */
      col = numColsB & 0x3U;

      while (col > 0U)
      {
        /* Read and store input element in destination */
        *px = *pInB++;

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB;

        /* Decrement column loop counter */
        col--;
      }

      i++;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Reset variables for usage in following multiplication process */
    row = numRowsA;
    i = 0U;
    px = pDst->pData;

#if defined (RISCV_MATH_DSP)
    /* Process two rows from matrix A at a time and output two rows at a time */
    row = row >> 1U;
    px2 = px + numColsB;
#endif

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    while (row > 0U)
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of transposed pSrcB data */
      pInB = pSrcBT;

#if defined (RISCV_MATH_DSP)
      /* Process two (transposed) columns from matrix B at a time */
      col = col >> 1U;
      j = 0;
#endif

      /* column loop */
      while (col > 0U)
      {
        /* Set variable sum, that acts as accumulator, to zero */
        sum = 0;

        /* Initiate pointer pInA to point to starting address of column being processed */
        pInA = pSrcA->pData + i;

#if defined (RISCV_MATH_DSP)
        sum2 = 0;
        sum3 = 0;
        sum4 = 0;
        pInB  = pSrcBT + j;
        pInA2 = pInA + numColsA;
        pInB2 = pInB + numRowsB;

        /* Read in two elements at once - allows dual MAC instruction */
        colCnt = numColsA >> 1U;
#else
        colCnt = numColsA >> 2U;
#endif

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

#if defined (RISCV_MATH_DSP)
          /* read real and imag values from pSrcA and pSrcB buffer */
          inA1 = read_q15x2_ia (&pInA);
          inB1 = read_q15x2_ia (&pInB);

          inA2 = read_q15x2_ia (&pInA2);
          inB2 = read_q15x2_ia (&pInB2);

          /* Multiply and Accumulates */
          sum  = __SMLAD(inA1, inB1, sum);
          sum2 = __SMLAD(inA1, inB2, sum2);
          sum3 = __SMLAD(inA2, inB1, sum3);
          sum4 = __SMLAD(inA2, inB2, sum4);
#else
          /* read real and imag values from pSrcA and pSrcB buffer */
          inA1 = *pInA++;
          inB1 = *pInB++;
          /* Multiply and Accumulates */
          sum += inA1 * inB1;

          inA2 = *pInA++;
          inB2 = *pInB++;
          sum += inA2 * inB2;

          inA1 = *pInA++;
          inB1 = *pInB++;
          sum += inA1 * inB1;

          inA2 = *pInA++;
          inB2 = *pInB++;
          sum += inA2 * inB2;
#endif /* #if defined (RISCV_MATH_DSP) */

          /* Decrement loop counter */
          colCnt--;
        }

        /* process odd column samples */
#if defined (RISCV_MATH_DSP)
        if (numColsA & 1U) {
          inA1 = *pInA++;
          inB1 = *pInB++;
          inA2 = *pInA2++;
          inB2 = *pInB2++;
          sum  += inA1 * inB1;
          sum2 += inA1 * inB2;
          sum3 += inA2 * inB1;
          sum4 += inA2 * inB2;
        }
#else
        colCnt = numColsA & 0x3U;

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
          sum += (q31_t) *pInA++ * *pInB++;

          /* Decrement loop counter */
          colCnt--;
        }
#endif /* #if defined (RISCV_MATH_DSP) */

        /* Saturate and store result in destination buffer */
        *px++  = (q15_t) (sum >> 15);

#if defined (RISCV_MATH_DSP)
        *px++  = (q15_t) (sum2 >> 15);
        *px2++ = (q15_t) (sum3 >> 15);
        *px2++ = (q15_t) (sum4 >> 15);
        j += numRowsB * 2;
#endif

        /* Decrement column loop counter */
        col--;

      }

      i = i + numColsA;

#if defined (RISCV_MATH_DSP)
      i = i + numColsA;
      px = px2 + (numColsB & 1U);
      px2 = px + numColsB;
#endif

      /* Decrement row loop counter */
      row--;

    }

    /* Compute any remaining odd row/column below */

#if defined (RISCV_MATH_DSP)

    /* Compute remaining output column */
    if (numColsB & 1U) {

      /* Avoid redundant computation of last element */
      row = numRowsA & (~0x1);

      /* Point to remaining unfilled column in output matrix */
      px = pDst->pData + numColsB-1;
      pInA = pSrcA->pData;

      /* row loop */
      while (row > 0)
      {

        /* point to last column in matrix B */
        pInB  = pSrcBT + numRowsB * (numColsB-1);
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        sum64 = 0;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
        /* Set variable sum, that acts as accumulator, to zero */
        sum  = 0;

        /* Compute 4 columns at once */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
#if __RISCV_XLEN == 64
          inA164 = read_q15x4_ia (&pInA);
          inB164 = read_q15x4_ia (&pInB);

          sum64 = __SMLAD(inA164, inB164, sum64);
#else
#ifdef NUCLEI_DSP_N3
          inA164 = read_q15x4_ia(&pInA);
          inB164 = read_q15x4_ia(&pInB);

          sum64 = __dkmada(sum64, inA164, inB164);
#else
          inA1 = read_q15x2_ia (&pInA);
          inA2 = read_q15x2_ia (&pInA);
          inB1 = read_q15x2_ia (&pInB);
          inB2 = read_q15x2_ia (&pInB);

          sum = __SMLAD(inA1, inB1, sum);
          sum = __SMLAD(inA2, inB2, sum);
#endif /* NUCLEI_DSP_N3 */
#endif /* __RISCV_XLEN == 64 */

          /* Decrement loop counter */
          colCnt--;
        }
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        sum = (q31_t)((q31_t)(sum64 & 0xffffffff)) + ((q31_t)((sum64 & 0xffffffff00000000) >> 32));
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
        colCnt = numColsA & 3U;
        while (colCnt > 0U) {
          sum += (q31_t) (*pInA++) * (*pInB++);
          colCnt--;
        }

        /* Store result in destination buffer */
        *px = (q15_t) (sum  >> 15);
        px += numColsB;

        /* Decrement row loop counter */
        row--;
      }
    }

    /* Compute remaining output row */
    if (numRowsA & 1U) {

      /* point to last row in output matrix */
      px = pDst->pData + (numColsB) * (numRowsA-1);

      pInB  = pSrcBT;
      col = numColsB;
      i = 0U;

      /* col loop */
      while (col > 0)
      {
        /* point to last row in matrix A */
        pInA = pSrcA->pData + (numRowsA-1) * numColsA;
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        sum64 = 0;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
        /* Set variable sum, that acts as accumulator, to zero */
        sum  = 0;

        /* Compute 4 columns at once */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
#if __RISCV_XLEN == 64
          inA164 = read_q15x4_ia (&pInA);
          inB164 = read_q15x4_ia (&pInB);

          sum64 = __SMLAD(inA164, inB164, sum64);
#else
#ifdef NUCLEI_DSP_N3
          inA164 = read_q15x4_ia(&pInA);
          inB164 = read_q15x4_ia(&pInB);

          sum64 = __dkmada(sum64, inA164, inB164);
#else
          inA1 = read_q15x2_ia (&pInA);
          inA2 = read_q15x2_ia (&pInA);
          inB1 = read_q15x2_ia (&pInB);
          inB2 = read_q15x2_ia (&pInB);

          sum = __SMLAD(inA1, inB1, sum);
          sum = __SMLAD(inA2, inB2, sum);
#endif /* NUCLEI_DSP_N3 */
#endif /* __RISCV_XLEN == 64 */
          /* Decrement loop counter */
          colCnt--;
        }
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
          sum = (q31_t)((q31_t)(sum64) + (q31_t)(sum64 >> 32));
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
        colCnt = numColsA & 3U;
        while (colCnt > 0U) {
          sum += (q31_t) (*pInA++) * (*pInB++);

          colCnt--;
        }

        /* Store result in destination buffer */
        *px++ = (q15_t) (sum  >> 15);

        /* Decrement column loop counter */
        col--;
      }
    }

#endif /* #if defined (RISCV_MATH_DSP) */

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }
#endif

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
