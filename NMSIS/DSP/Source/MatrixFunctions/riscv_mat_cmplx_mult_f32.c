/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_cmplx_mult_f32.c
 * Description:  Floating-point matrix multiplication
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
  @defgroup CmplxMatrixMult  Complex Matrix Multiplication

  Complex Matrix multiplication is only defined if the number of columns of the
  first matrix equals the number of rows of the second matrix.
  Multiplying an <code>M x N</code> matrix with an <code>N x P</code> matrix results
  in an <code>M x P</code> matrix.
  @par
  When matrix size checking is enabled, the functions check:
   - that the inner dimensions of <code>pSrcA</code> and <code>pSrcB</code> are equal;
   - that the size of the output matrix equals the outer dimensions of <code>pSrcA</code> and <code>pSrcB</code>.
 */


/**
  @addtogroup CmplxMatrixMult
  @{
 */

/**
  @brief         Floating-point Complex matrix multiplication.
  @param[in]     pSrcA      points to first input complex matrix structure
  @param[in]     pSrcB      points to second input complex matrix structure
  @param[out]    pDst       points to output complex matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed
 */
RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_cmplx_mult_f32(
  const riscv_matrix_instance_f32 * pSrcA,
  const riscv_matrix_instance_f32 * pSrcB,
        riscv_matrix_instance_f32 * pDst)
{
  float32_t *pIn1 = pSrcA->pData;                /* Input data matrix pointer A */
  float32_t *pIn2 = pSrcB->pData;                /* Input data matrix pointer B */
  float32_t *pInA = pSrcA->pData;                /* Input data matrix pointer A */
  float32_t *pInB = pSrcB->pData;                /* Input data matrix pointer B */
  float32_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  float32_t *px;                                 /* Temporary output data matrix pointer */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  float32_t sumReal, sumImag;                    /* Accumulator */
  float32_t a1, b1, c1, d1;
  uint32_t col, i = 0U, j, row = numRowsA, colCnt; /* loop counters */
  riscv_status status;                             /* status of matrix multiplication */

#if defined (RISCV_MATH_LOOPUNROLL)
  float32_t a0, b0, c0, d0;
#endif

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
#if defined(RISCV_MATH_VECTOR)
    size_t ii, jj, kk;
    size_t l;
    vfloat32m4x2_t v_tuple;
    vfloat32m4_t vres0m4, vres1m4, v_inAR, v_inAI;
    colCnt = numRowsA;

    for (jj = colCnt; jj > 0; jj--) {
      px = pOut;
      pInB = pIn2;
      for (ii = numColsB; ii > 0; ii -= l) {
        l = __riscv_vsetvl_e32m4(ii);
        pInA = pIn1;
        vres0m4 = __riscv_vfmv_v_f_f32m4(0.0, l);
        vres1m4 = __riscv_vmv_v_v_f32m4(vres0m4, l);
        for (kk = 0; kk < numColsA; kk++) {
          v_tuple = __riscv_vlseg2e32_v_f32m4x2 (pInB + 2 * kk * numColsB, l);
          v_inAR = __riscv_vget_v_f32m4x2_f32m4(v_tuple, 0);
          v_inAI = __riscv_vget_v_f32m4x2_f32m4(v_tuple, 1);
          vres0m4 = __riscv_vfmacc_vf_f32m4(vres0m4, 1, __riscv_vfsub_vv_f32m4(__riscv_vfmul_vf_f32m4(v_inAR, *(pInA + 0), l), __riscv_vfmul_vf_f32m4(v_inAI, *(pInA + 1), l), l), l);
          vres1m4 = __riscv_vfmacc_vf_f32m4(vres1m4, 1, __riscv_vfadd_vv_f32m4(__riscv_vfmul_vf_f32m4(v_inAR, *(pInA + 1), l), __riscv_vfmul_vf_f32m4(v_inAI, *(pInA + 0), l), l), l);
          pInA += 2;
        }
        v_tuple = __riscv_vset_v_f32m4_f32m4x2 (v_tuple, 0, vres0m4);
        v_tuple = __riscv_vset_v_f32m4_f32m4x2 (v_tuple, 1, vres1m4);
        __riscv_vsseg2e32_v_f32m4x2 (px, v_tuple, l);
        px += l * 2;
        pInB += l * 2;
      }
      pIn1 += 2 * numColsA;
      pOut += 2 * numColsB;
    }
  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;
#else
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
        sumReal = 0.0f;
        sumImag = 0.0f;

        /* Initiate pointer pIn1 to point to starting address of column being processed */
        pIn1 = pInA;

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {

          /* Reading real part of complex matrix A */
          a0 = *pIn1;

          /* Reading real part of complex matrix B */
          c0 = *pIn2;

          /* Reading imaginary part of complex matrix A */
          b0 = *(pIn1 + 1U);

          /* Reading imaginary part of complex matrix B */
          d0 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += a0 * c0;
          sumImag += b0 * c0;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= b0 * d0;
          sumImag += a0 * d0;

          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* read real and imag values from pSrcA and pSrcB buffer */
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

          a0 = *(pIn1     );
          c0 = *(pIn2     );
          b0 = *(pIn1 + 1U);
          d0 = *(pIn2 + 1U);

          /* Multiply and Accumlates */
          sumReal += a0 * c0;
          sumImag += b0 * c0;

          /* update pointers */
          pIn1 += 2U;
          pIn2 += 2 * numColsB;

          /* Multiply and Accumlates */
          sumReal -= b0 * d0;
          sumImag += a0 * d0;

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

          /* Decrement loop count */
          colCnt--;
        }

        /* If the columns of pSrcA is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        colCnt = numColsA & 0x3U;

#else

        /* Initialize blkCnt with number of samples */
        colCnt = numColsA;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

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
#endif /* defined(RISCV_MATH_VECTOR) */
  }
  /* Return to application */
  return (status);
}


/**
  @} end of MatrixMult group
 */
