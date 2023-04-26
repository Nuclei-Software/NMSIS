/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_mult_q31.c
 * Description:  Q31 matrix multiplication
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
  @brief         Q31 matrix multiplication.
  @param[in]     pSrcA      points to the first input matrix structure
  @param[in]     pSrcB      points to the second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate
                   multiplication results but provides only a single guard bit. There is no saturation
                   on intermediate additions. Thus, if the accumulator overflows it wraps around and
                   distorts the result. The input signals should be scaled down to avoid intermediate
                   overflows. The input is thus scaled down by log2(numColsA) bits
                   to avoid overflows, as a total of numColsA additions are performed internally.
                   The 2.62 accumulator is right shifted by 31 bits and saturated to 1.31 format to yield the final result.
  @remark
                   Refer to \ref riscv_mat_mult_fast_q31() for a faster but less precise implementation of this function.
 */
riscv_status riscv_mat_mult_q31(
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
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
  q31_t tmp131, tmp231;
  q63_t temp164, temp264;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
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
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    uint16_t blkCnt = numColsA;
    size_t l;
    ptrdiff_t bstride = 4;       //  32bit/8bit = 4
    uint16_t colnum, rownum;

    px = pOut;

    uint32_t ii, jj, kk;
    vint32m2_t va0m2, va1m2, va2m2, va3m2;
    vint32m4_t va0m4, va1m4;
    vint64m4_t vres0m4, vres1m4, vres2m4, vres3m4;
    vint64m8_t vres0m8, vres1m8;
    colCnt = numColsB;
    /* ch = 4, mul = 4 */
    for (jj = colCnt / 4; jj > 0; jj--) {
      px = pOut;
      pInA = pIn1;
      for (ii = numRowsA; ii > 0; ii -= l) {
        l = vsetvl_e64m4(ii);
        pInB = pIn2;
        vres0m4 = vmv_v_x_i64m4(0, l);
        vres1m4 = vmv_v_v_i64m4(vres0m4, l);
        vres2m4 = vmv_v_v_i64m4(vres0m4, l);
        vres3m4 = vmv_v_v_i64m4(vres0m4, l);
        for (kk = 0; kk < numColsA; kk++) {
          va0m2 = vlse32_v_i32m2(pInA + kk, numColsA * bstride, l);
          vres0m4 = vwmacc_vx_i64m4 (vres0m4, (int32_t)(*(pInB + 0)), va0m2, l);
          vres1m4 = vwmacc_vx_i64m4 (vres1m4, (int32_t)(*(pInB + 1)), va0m2, l);
          vres2m4 = vwmacc_vx_i64m4 (vres2m4, (int32_t)(*(pInB + 2)), va0m2, l);
          vres3m4 = vwmacc_vx_i64m4 (vres3m4, (int32_t)(*(pInB + 3)), va0m2, l);
          pInB += numColsB;
        }
        va0m2 = vnsra_wx_i32m2(vres0m4, 31, l);
        va1m2 = vnsra_wx_i32m2(vres1m4, 31, l);
        va2m2 = vnsra_wx_i32m2(vres2m4, 31, l);
        va3m2 = vnsra_wx_i32m2(vres3m4, 31, l);
        vssseg4e32_v_i32m2(px, numColsB * bstride, va0m2, va1m2, va2m2, va3m2, l);
        px += l * numColsB;
        pInA += l * numColsA;
      }
      pIn2 += 4;
      pOut += 4;
    }
    /* ch = 2, mul = 8 */
    colCnt = colCnt & 0x3;
    for (jj = colCnt / 2; jj > 0; jj--) {
      px = pOut;
      pInA = pIn1;
      for (ii = numRowsA; ii > 0; ii -= l) {
        l = vsetvl_e64m8(ii);
        pInB = pIn2;
        vres0m8 = vmv_v_x_i64m8(0, l);
        vres1m8 = vmv_v_v_i64m8(vres0m8, l);
        for (kk = 0; kk < numColsA; kk++) {
          va0m4 = vlse32_v_i32m4(pInA + kk, numColsA * bstride, l);
          vres0m8 = vwmacc_vx_i64m8(vres0m8, (int32_t)(*(pInB + 0)), va0m4, l);
          vres1m8 = vwmacc_vx_i64m8(vres1m8, (int32_t)(*(pInB + 1)), va0m4, l);
          pInB += numColsB;
        }
        va0m4 = vnsra_wx_i32m4(vres0m8, 31, l);
        va1m4 = vnsra_wx_i32m4(vres1m8, 31, l);
        vssseg2e32_v_i32m4(px, numColsB * bstride, va0m4, va1m4, l);
        px += l * numColsB;
        pInA += l * numColsA;
      }
      pIn2 += 2;
      pOut += 2;
    }
    /* ch = 1, mul = 8 */
    colCnt = colCnt & 0x1;
    for (jj = colCnt; jj > 0; jj--) {
      px = pOut;
      pInA = pIn1;
      for (ii = numRowsA; ii > 0; ii -= l) {
        l = vsetvl_e64m8(ii);
        pInB = pIn2;
        vres0m8 = vmv_v_x_i64m8(0, l);
        for (kk = 0; kk < numColsA; kk++) {
          va0m4 = vlse32_v_i32m4(pInA + kk, numColsA * bstride, l);
          vres0m8 = vwmacc_vx_i64m8 (vres0m8, (int32_t)(*(pInB + 0)), va0m4, l);
          pInB += numColsB;
        }
        va0m4 = vnsra_wx_i32m4(vres0m8, 31, l);
        vsse32_v_i32m4(px, numColsB * bstride, va0m4, l);
        px += l * numColsB;
        pInA += l * numColsA;
      }
      pIn2 += 1;
      pOut += 1;
    }
  /* Set status as RISCV_MATH_SUCCESS */
  status = RISCV_MATH_SUCCESS;
#else
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

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Loop unrolling: Compute 4 MACs at a time. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
          temp164 = read_q31x2_ia ((q31_t **) &pIn1);
          tmp131 = *pIn2;
          pIn2 += numColsB;
          tmp231 = *pIn2;
          pIn2 += numColsB;
          temp264 = ((q63_t)tmp131 & 0xffffffff) | ((q63_t) tmp231 << 32);
          sum = __RV_SMAR64(sum, temp164, temp264);

          temp164 = read_q31x2_ia ((q31_t **) &pIn1);
          tmp131 = *pIn2;
          pIn2 += numColsB;
          tmp231 = *pIn2;
          pIn2 += numColsB;
          temp264 = ((q63_t)tmp131 & 0xffffffff) | ((q63_t) tmp231 << 32);
          sum = __RV_SMAR64(sum, temp164, temp264);

#else
          /* Perform the multiply-accumulates */
          sum += (q63_t) *pIn1++ * *pIn2;
          pIn2 += numColsB;

          sum += (q63_t) *pIn1++ * *pIn2;
          pIn2 += numColsB;

          sum += (q63_t) *pIn1++ * *pIn2;
          pIn2 += numColsB;

          sum += (q63_t) *pIn1++ * *pIn2;
          pIn2 += numColsB;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

          /* Decrement loop counter */
          colCnt--;
        }

        /* Loop unrolling: Compute remaining MACs */
        colCnt = numColsA & 0x3U;

#else

        /* Initialize cntCnt with number of columns */
        colCnt = numColsA;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

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
#endif /*defined(RISCV_MATH_VECTOR)*/
  }
  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
