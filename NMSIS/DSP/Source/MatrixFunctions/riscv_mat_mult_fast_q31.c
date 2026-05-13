/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_mult_fast_q31.c
 * Description:  Q31 matrix multiplication (fast variant)
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
  @brief         Q31 matrix multiplication (fast variant).
  @param[in]     pSrcA      points to the first input matrix structure
  @param[in]     pSrcB      points to the second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The difference between the function \ref riscv_mat_mult_q31() and this fast variant is that
                   the fast variant use a 32-bit rather than a 64-bit accumulator.
                   The result of each 1.31 x 1.31 multiplication is truncated to
                   2.30 format. These intermediate results are accumulated in a 32-bit register in 2.30
                   format. Finally, the accumulator is saturated and converted to a 1.31 result.
  @par
                   The fast version has the same overflow behavior as the standard version but provides
                   less precision since it discards the low 32 bits of each multiplication result.
                   In order to avoid overflows completely the input signals must be scaled down.
                   Scale down one of the input matrices by log2(numColsA) bits to avoid overflows,
                   as a total of numColsA additions are computed internally for each output element.
  @remark
                   Refer to \ref riscv_mat_mult_q31() for a slower implementation of this function
                   which uses 64-bit accumulation to provide higher precision.
 */

RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_mult_fast_q31(
  const riscv_matrix_instance_q31 * pSrcA,
  const riscv_matrix_instance_q31 * pSrcB,
        riscv_matrix_instance_q31 * pDst)
{
  q31_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pInB = pSrcB->pData;                    /* Input data matrix pointer B */
  q31_t *pInA2;
  q31_t *px;                                     /* Temporary output data matrix pointer */
  q31_t *px2;
  q31_t sum1, sum2, sum3, sum4;                  /* Accumulator */
  q31_t inA1, inA2, inB1, inB2;
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  uint32_t col, i = 0U, j, row = numRowsA, colCnt;  /* Loop counters */
  riscv_status status;                             /* Status of matrix multiplication */


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
#if defined(RISCV_MATH_VECTOR_ZVE32X)
    /*
     * Note: RVV use is vmulh, which may not be exactly the same as the scalar version, likes:
     * golden: sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32)
     * rvv: sum1 = sum1 + (q31_t)(((q63_t) *pInA++ * *pInB) >> 32)
     * If overflow happens, please use riscv_mat_mult_q31.
     */
    q31_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
    q31_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
    q31_t *pOut = pDst->pData;                     /* Output data matrix pointer */
    size_t ii, jj, kk;
    size_t l;
    vint32m4_t va0m4, va1m4, va2m4, va3m4;
    vint32m8_t va0m8, va1m8;
    vint32m4_t vres0m4, vres1m4, vres2m4, vres3m4;
    vint32m8_t vres0m8, vres1m8;
    colCnt = numRowsA;
    /* ch = 4, mul = 4 */
    for (jj = colCnt / 4; jj > 0; jj--) {
      px = pOut;
      pInB = pIn2;
      for (ii = numColsB; ii > 0; ii -= l) {
        l = __riscv_vsetvl_e32m4(ii);
        pInA = pIn1;
        vres0m4 = __riscv_vmv_v_x_i32m4(0, l);
        vres1m4 = __riscv_vmv_v_v_i32m4(vres0m4, l);
        vres2m4 = __riscv_vmv_v_v_i32m4(vres0m4, l);
        vres3m4 = __riscv_vmv_v_v_i32m4(vres0m4, l);
        for (kk = 0; kk < numColsA; kk++) {
          va0m4 = __riscv_vle32_v_i32m4(pInB + kk * numColsB, l);
          vres0m4 = __riscv_vadd_vv_i32m4(vres0m4, __riscv_vmulh_vx_i32m4(va0m4, (int32_t)(*(pInA)), l), l);
          vres1m4 = __riscv_vadd_vv_i32m4(vres1m4, __riscv_vmulh_vx_i32m4(va0m4, (int32_t)(*(pInA + numColsA)), l), l);
          vres2m4 = __riscv_vadd_vv_i32m4(vres2m4, __riscv_vmulh_vx_i32m4(va0m4, (int32_t)(*(pInA + 2 * numColsA)), l), l);
          vres3m4 = __riscv_vadd_vv_i32m4(vres3m4, __riscv_vmulh_vx_i32m4(va0m4, (int32_t)(*(pInA + 3 * numColsA)), l), l);
          pInA++;
        }
        va0m4 = __riscv_vsll_vx_i32m4(vres0m4, 1, l);
        va1m4 = __riscv_vsll_vx_i32m4(vres1m4, 1, l);
        va2m4 = __riscv_vsll_vx_i32m4(vres2m4, 1, l);
        va3m4 = __riscv_vsll_vx_i32m4(vres3m4, 1, l);

        __riscv_vse32_v_i32m4(px, va0m4, l);
        __riscv_vse32_v_i32m4(px + numColsB, va1m4, l);
        __riscv_vse32_v_i32m4(px + 2 * numColsB, va2m4, l);
        __riscv_vse32_v_i32m4(px + 3 * numColsB, va3m4, l);
        px += l;
        pInB += l;
      }
      pIn1 += 4 * numColsA;
      pOut += 4 * numColsB;
    }

    /* ch = 2, mul = 8 */
    colCnt = colCnt & 0x3;
    for (jj = colCnt / 2; jj > 0; jj--) {
      px = pOut;
      pInB = pIn2;
      for (ii = numColsB; ii > 0; ii -= l) {
        l = __riscv_vsetvl_e32m8(ii);
        pInA = pIn1;
        vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
        vres1m8 = __riscv_vmv_v_v_i32m8(vres0m8, l);
        for (kk = 0; kk < numColsA; kk++) {
          va0m8 = __riscv_vle32_v_i32m8(pInB + kk * numColsB, l);
          vres0m8 = __riscv_vadd_vv_i32m8(vres0m8, __riscv_vmulh_vx_i32m8(va0m8, (int32_t)(*(pInA)), l), l);
          vres1m8 = __riscv_vadd_vv_i32m8(vres1m8, __riscv_vmulh_vx_i32m8(va0m8, (int32_t)(*(pInA + numColsA)), l), l);
          pInA++;
        }
        va0m8 = __riscv_vsll_vx_i32m8(vres0m8, 1, l);
        va1m8 = __riscv_vsll_vx_i32m8(vres1m8, 1, l);

        __riscv_vse32_v_i32m8 (px, va0m8, l);
        __riscv_vse32_v_i32m8 (px + numColsB, va1m8, l);
        px += l;
        pInB += l;
      }
      pIn1 += 2 * numColsA;
      pOut += 2 * numColsB;
    }
    /* ch = 1, mul = 8 */
    colCnt = colCnt & 0x1;
    for (jj = colCnt; jj > 0; jj--) {
      px = pOut;
      pInB = pIn2;
      for (ii = numColsB; ii > 0; ii -= l) {
        l = __riscv_vsetvl_e32m8(ii);
        pInA = pIn1;
        vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
        for (kk = 0; kk < numColsA; kk++) {
          va0m8 = __riscv_vle32_v_i32m8(pInB + kk * numColsB, l);
          vres0m8 = __riscv_vadd_vv_i32m8(vres0m8, __riscv_vmulh_vx_i32m8(va0m8, (int32_t)(*(pInA)), l), l);
        }
        va0m8 = __riscv_vsll_vx_i32m8(vres0m8, 1, l);
        __riscv_vse32_v_i32m8(px, va0m8, l);
        px += l;
        pInB += l;
      }
      pIn1 += numColsA;
      pOut += numColsB;
    }
  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;
#else
    px = pDst->pData;

    row = row >> 1U;
    px2 = px + numColsB;

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    while (row > 0U)
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pInB = pSrcB->pData;

      j = 0U;

      col = col >> 1U;

      /* column loop */
      while (col > 0U)
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum1 = 0;
        sum2 = 0;
        sum3 = 0;
        sum4 = 0;
        
        /* Initiate data pointers */
        pInA = pSrcA->pData + i;
        pInB = pSrcB->pData + j;
        pInA2 = pInA + numColsA;
        
        colCnt = numColsA;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          inA1 = *pInA++;
          inB1 = pInB[0];
          inA2 = *pInA2++;
          inB2 = pInB[1];
          pInB += numColsB;

#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(inA1, inB1, sum1);
          sum2 = __SMMLA(inA1, inB2, sum2);
          sum3 = __SMMLA(inA2, inB1, sum3);
          sum4 = __SMMLA(inA2, inB2, sum4);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) inA1 * inB1)) >> 32);
          sum2 = (q31_t) ((((q63_t) sum2 << 32) + ((q63_t) inA1 * inB2)) >> 32);
          sum3 = (q31_t) ((((q63_t) sum3 << 32) + ((q63_t) inA2 * inB1)) >> 32);
          sum4 = (q31_t) ((((q63_t) sum4 << 32) + ((q63_t) inA2 * inB2)) >> 32);
#endif

          /* Decrement loop counter */
          colCnt--;
        }

        /* Convert the result from 2.30 to 1.31 format and store in destination buffer */
        *px++  = sum1 << 1;
        *px++  = sum2 << 1;
        *px2++ = sum3 << 1;
        *px2++ = sum4 << 1;

        j += 2;

        /* Decrement column loop counter */
        col--;
      }

      i = i + (numColsA << 1U);
      px  = px2 + (numColsB & 1U);
      px2 = px  +  numColsB;

      /* Decrement row loop counter */
      row--;
    }

    /* Compute any remaining odd row/column below */

    /* Compute remaining output column */
    if (numColsB & 1U) {

      /* Avoid redundant computation of last element */
      row = numRowsA & (~1U);

      /* Point to remaining unfilled column in output matrix */
      px = pDst->pData + numColsB-1;
      pInA = pSrcA->pData;

      /* row loop */
      while (row > 0)
      {

        /* point to last column in matrix B */
        pInB  = pSrcB->pData + numColsB-1;

        /* Set variable sum1, that acts as accumulator, to zero */
        sum1  = 0;

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Loop unrolling: Compute 4 columns at a time. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(*pInA++, *pInB, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32);
#endif
          pInB += numColsB;

#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(*pInA++, *pInB, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32);
#endif
          pInB += numColsB;

#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(*pInA++, *pInB, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32);
#endif
          pInB += numColsB;

#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(*pInA++, *pInB, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32);
#endif
          pInB += numColsB;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Loop unrolling: Compute remaining column */
        colCnt = numColsA % 4U;

#else

        /* Initialize colCnt with number of columns */
        colCnt = numColsA;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

        while (colCnt > 0U) {
#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(*pInA++, *pInB, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32);
#endif
          pInB += numColsB;
          
          colCnt--;
        }

        /* Convert the result from 2.30 to 1.31 format and store in destination buffer */
        *px = sum1 << 1;
        px += numColsB;

        /* Decrement row loop counter */
        row--;
      }
    }

    /* Compute remaining output row */
    if (numRowsA & 1U) {

      /* point to last row in output matrix */
      px = pDst->pData + (numColsB) * (numRowsA-1);

      col = numColsB;
      i = 0U;

      /* col loop */
      while (col > 0)
      {

        /* point to last row in matrix A */
        pInA = pSrcA->pData + (numRowsA-1) * numColsA;
        pInB  = pSrcB->pData + i;

        /* Set variable sum1, that acts as accumulator, to zero */
        sum1  = 0;

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Loop unrolling: Compute 4 columns at a time. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          inA1 = *pInA++;
          inA2 = *pInA++;
          inB1 = *pInB;
          pInB += numColsB;
          inB2 = *pInB;
          pInB += numColsB;
#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(inA1, inB1, sum1);
          sum1 = __SMMLA(inA2, inB2, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) inA1 * inB1)) >> 32);
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) inA2 * inB2)) >> 32);
#endif

          inA1 = *pInA++;
          inA2 = *pInA++;
          inB1 = *pInB;
          pInB += numColsB;
          inB2 = *pInB;
          pInB += numColsB;
#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(inA1, inB1, sum1);
          sum1 = __SMMLA(inA2, inB2, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) inA1 * inB1)) >> 32);
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) inA2 * inB2)) >> 32);
#endif

          /* Decrement loop counter */
          colCnt--;
        }

        /* Loop unrolling: Compute remaining column */
        colCnt = numColsA % 4U;

#else

        /* Initialize colCnt with number of columns */
        colCnt = numColsA;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

        while (colCnt > 0U) {
#if defined (RISCV_MATH_DSP)
          sum1 = __SMMLA(*pInA++, *pInB, sum1);
#else
          sum1 = (q31_t) ((((q63_t) sum1 << 32) + ((q63_t) *pInA++ * *pInB)) >> 32);
#endif
          pInB += numColsB;

          colCnt--;
        }

        /* Saturate and store the result in the destination buffer */
        *px++ = sum1 << 1;
        i++;

        /* Decrement col loop counter */
        col--;
      }
    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
#endif /*defined(RISCV_MATH_VECTOR_ZVE32X)*/
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
