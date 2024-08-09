/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mat_mult_q15.c
 * Description:  Q15 complex matrix multiplication
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
  @addtogroup CmplxMatrixMult
  @{
 */

/**
  @brief         Q15 Complex matrix multiplication.
  @param[in]     pSrcA      points to first input complex matrix structure
  @param[in]     pSrcB      points to second input complex matrix structure
  @param[out]    pDst       points to output complex matrix structure
  @param[in]     pScratch   points to an array for storing intermediate results
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Conditions for optimum performance
                   Input, output and state buffers should be aligned by 32-bit

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator. The inputs to the
                   multiplications are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow. The 34.30 result is then
                   truncated to 34.15 format by discarding the low 15 bits and then saturated to 1.15 format.
 */
RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_cmplx_mult_q15(
  const riscv_matrix_instance_q15 * pSrcA,
  const riscv_matrix_instance_q15 * pSrcB,
        riscv_matrix_instance_q15 * pDst,
        q15_t                   * pScratch)
{
        q15_t *pSrcBT = pScratch;                      /* input data matrix pointer for transpose */
        q15_t *pInA = pSrcA->pData;                    /* input data matrix pointer A of Q15 type */
        q15_t *pInB = pSrcB->pData;                    /* input data matrix pointer B of Q15 type */
        q15_t *px;                                     /* Temporary output data matrix pointer */
        uint16_t numRowsA = pSrcA->numRows;            /* number of rows of input matrix A */
        uint16_t numColsB = pSrcB->numCols;            /* number of columns of input matrix B */
        uint16_t numColsA = pSrcA->numCols;            /* number of columns of input matrix A */
        uint16_t numRowsB = pSrcB->numRows;            /* number of rows of input matrix A */
        q63_t sumReal, sumImag;                        /* accumulator */
        uint32_t col, i = 0U, row = numRowsB, colCnt;  /* Loop counters */
        riscv_status status;                             /* Status of matrix multiplication */

#if defined (RISCV_MATH_DSP) && !defined (RISCV_MATH_VECTOR)
#if __RISCV_XLEN == 64
        q63_t prod164, prod264, pSourceA64, pSourceB64;
        q15_t a, b, c, d;
#endif /* __RISCV_XLEN == 64 */
#else
        q15_t a, b, c, d;
#endif /* #if defined (RISCV_MATH_DSP) */
#if defined (RISCV_MATH_DSP)
        q31_t prod1, prod2;
        q31_t pSourceA, pSourceB;
#if defined (NUCLEI_DSP_N3)
        q63_t pSourceA64_n, pSourceB64_n;
#endif
#endif /* #if defined (RISCV_MATH_DSP) */

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
#if defined (RISCV_MATH_VECTOR)
    q15_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
    q15_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
    q15_t *pOut = pDst->pData;                     /* Output data matrix pointer */
    size_t ii, jj, kk;
    size_t l;
    vint16m4x2_t v_tuple;
    vint16m4_t va0m4, va1m4, v_inAR, v_inAI;
    vint32m8_t v_inAR2, v_inAI2, vres0m8, vres1m8;
    colCnt = numRowsA;

    for (jj = colCnt; jj > 0; jj--) {
      px = pOut;
      pInB = pIn2;
      for (ii = numColsB; ii > 0; ii -= l) {
        l = __riscv_vsetvl_e32m8(ii);
        pInA = pIn1;
        vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
        vres1m8 = __riscv_vmv_v_v_i32m8(vres0m8, l);
        for (kk = 0; kk < numColsA; kk++) {
          v_tuple = __riscv_vlseg2e16_v_i16m4x2 (pInB + 2 * kk * numColsB, l);
          v_inAR = __riscv_vget_v_i16m4x2_i16m4(v_tuple, 0);
          v_inAI = __riscv_vget_v_i16m4x2_i16m4(v_tuple, 1);

          v_inAR2 = __riscv_vsub_vv_i32m8(__riscv_vwmul_vx_i32m8(v_inAR, *(pInA + 0), l), __riscv_vwmul_vx_i32m8(v_inAI, *(pInA + 1), l), l);
          v_inAI2 = __riscv_vadd_vv_i32m8(__riscv_vwmul_vx_i32m8(v_inAR, *(pInA + 1), l), __riscv_vwmul_vx_i32m8(v_inAI, *(pInA + 0), l), l);
          vres0m8 = __riscv_vmacc_vx_i32m8 (vres0m8, 1, v_inAR2, l);
          vres1m8 = __riscv_vmacc_vx_i32m8 (vres1m8, 1, v_inAI2, l);
          pInA += 2;
        }
        va0m4 = __riscv_vnclip_wx_i16m4(vres0m8, 15, __RISCV_VXRM_RNU, l);
        va1m4 = __riscv_vnclip_wx_i16m4(vres1m8, 15, __RISCV_VXRM_RNU, l);

        v_tuple = __riscv_vset_v_i16m4_i16m4x2 (v_tuple, 0, va0m4);
        v_tuple = __riscv_vset_v_i16m4_i16m4x2 (v_tuple, 1, va1m4);
        __riscv_vsseg2e16_v_i16m4x2 (px, v_tuple, l);
        px += l * 2;
        pInB += l * 2;
      }
      pIn1 += 2 * numColsA;
      pOut += 2 * numColsB;
    }
  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;
#else

    /* Matrix transpose */
    do
    {
      /* The pointer px is set to starting address of column being processed */
      px = pSrcBT + i;

#if defined (RISCV_MATH_LOOPUNROLL)

      /* Apply loop unrolling and exchange the columns with row elements */
      col = numColsB >> 2;

      /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
         a second loop below computes the remaining 1 to 3 samples. */
      while (col > 0U)
      {
        /* Read two elements from row */
        write_q15x2 (px, read_q15x2_ia (&pInB));

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB * 2;

        /* Read two elements from row */
        write_q15x2 (px, read_q15x2_ia (&pInB));

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB * 2;

        /* Read two elements from row */
        write_q15x2 (px, read_q15x2_ia (&pInB));

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB * 2;

        /* Read two elements from row */
        write_q15x2 (px, read_q15x2_ia (&pInB));

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB * 2;

        /* Decrement column loop counter */
        col--;
      }

      /* If the columns of pSrcB is not a multiple of 4, compute any remaining output samples here.
       ** No loop unrolling is used. */
      col = numColsB & 0x3U;

#else

        /* Initialize blkCnt with number of samples */
        col = numColsB;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

      while (col > 0U)
      {
        /* Read two elements from row */
        write_q15x2 (px, read_q15x2_ia (&pInB));

        /* Update pointer px to point to next row of transposed matrix */
        px += numRowsB * 2;

        /* Decrement column loop counter */
        col--;
      }

      i = i + 2U;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

    /* Reset variables for usage in following multiplication process */
    row = numRowsA;
    i = 0U;
    px = pDst->pData;

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of transposed pSrcB data */
      pInB = pSrcBT;

      /* column loop */
      do
      {
        /* Set variable sum, that acts as accumulator, to zero */
        sumReal = 0;
        sumImag = 0;

        /* Initiate pointer pInA to point to starting address of column being processed */
        pInA = pSrcA->pData + i * 2;

        /* Apply loop unrolling and compute 2 MACs simultaneously. */
        colCnt = numColsA >> 1U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
          pSourceA64 = read_q15x4_ia((q15_t**)&pInA);
          pSourceB64 = read_q15x4_ia((q15_t**)&pInB);

          prod164 = __SMUSD(pSourceA64, pSourceB64);
          prod264 = __SMUADX(pSourceA64, pSourceB64);

          sumReal += (q63_t) ((q31_t) prod164);
          sumReal += (q63_t) ((q31_t) (prod164 >> 32));
          sumImag += (q63_t) ((q31_t) prod264);
          sumImag += (q63_t) ((q31_t) (prod264 >> 32));
#else
#if defined (NUCLEI_DSP_N3)
          /* read real and imag values from pSrcA and pSrcB buffer*/
          pSourceA64_n = read_q15x4_ia((q15_t**)&pInA);
          pSourceB64_n = read_q15x4_ia((q15_t**)&pInB);

          /* Multiply and Accumlates */
          // Signed Multiply Two Words and Reverse Subtract, bottom * bottom - top * top
          sumReal = __RV_DSMALDRS(sumReal, pSourceA64_n, pSourceB64_n);
          // SIMD Signed Crossed Multiply Two Halfs and Add, top * bottom + bottom * top
          sumImag = __RV_DSMALXDA(sumImag, pSourceA64_n, pSourceB64_n);
#else
          /* read real and imag values from pSrcA and pSrcB buffer */
          pSourceA = read_q15x2_ia (&pInA);
          pSourceB = read_q15x2_ia (&pInB);

          /* Multiply and Accumlates */
          prod1 = __SMUSD(pSourceA, pSourceB);
          prod2 = __SMUADX(pSourceA, pSourceB);
          sumReal += (q63_t) prod1;
          sumImag += (q63_t) prod2;

          /* read real and imag values from pSrcA and pSrcB buffer */
          pSourceA = read_q15x2_ia (&pInA);
          pSourceB = read_q15x2_ia (&pInB);

          /* Multiply and Accumlates */
          prod1 = __SMUSD(pSourceA, pSourceB);
          prod2 = __SMUADX(pSourceA, pSourceB);
          sumReal += (q63_t) prod1;
          sumImag += (q63_t) prod2;
#endif /* defined(NUCLEI_DSP_N3) */

#endif /* __RISCV_XLEN == 64 */
#else /* #if defined (RISCV_MATH_DSP) */

          /* read real and imag values from pSrcA buffer */
          a = *pInA;
          b = *(pInA + 1U);
          /* read real and imag values from pSrcB buffer */
          c = *pInB;
          d = *(pInB + 1U);

          /* Multiply and Accumlates */
          sumReal += (q31_t) a *c;
          sumImag += (q31_t) a *d;
          sumReal -= (q31_t) b *d;
          sumImag += (q31_t) b *c;

          /* read next real and imag values from pSrcA buffer */
          a = *(pInA + 2U);
          b = *(pInA + 3U);
          /* read next real and imag values from pSrcB buffer */
          c = *(pInB + 2U);
          d = *(pInB + 3U);

          /* update pointer */
          pInA += 4U;

          /* Multiply and Accumlates */
          sumReal += (q31_t) a * c;
          sumImag += (q31_t) a * d;
          sumReal -= (q31_t) b * d;
          sumImag += (q31_t) b * c;
          /* update pointer */
          pInB += 4U;

#endif /* #if defined (RISCV_MATH_DSP) */

          /* Decrement loop counter */
          colCnt--;
        }

        /* process odd column samples */
        if ((numColsA & 0x1U) > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

#if defined (RISCV_MATH_DSP)
          /* read real and imag values from pSrcA and pSrcB buffer */
          pSourceA = read_q15x2_ia (&pInA);
          pSourceB = read_q15x2_ia (&pInB);

          /* Multiply and Accumlates */
          prod1 = __SMUSD(pSourceA, pSourceB);
          prod2 = __SMUADX(pSourceA, pSourceB);
          sumReal += (q63_t) prod1;
          sumImag += (q63_t) prod2;

#else /* #if defined (RISCV_MATH_DSP) */

          /* read real and imag values from pSrcA and pSrcB buffer */
          a = *pInA++;
          b = *pInA++;
          c = *pInB++;
          d = *pInB++;

          /* Multiply and Accumlates */
          sumReal += (q31_t) a * c;
          sumImag += (q31_t) a * d;
          sumReal -= (q31_t) b * d;
          sumImag += (q31_t) b * c;

#endif /* #if defined (RISCV_MATH_DSP) */

        }

        /* Saturate and store result in destination buffer */
        *px++ = (q15_t) (__SSAT(sumReal >> 15, 16));
        *px++ = (q15_t) (__SSAT(sumImag >> 15, 16));

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      i = i + numColsA;

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
