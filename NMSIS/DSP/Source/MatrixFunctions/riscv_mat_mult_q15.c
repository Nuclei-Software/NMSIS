/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_mult_q15.c
 * Description:  Q15 matrix multiplication
 *
 * $Date:        3 Nov 2021
 * $Revision:    V1.10.0
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
  @brief         Q15 matrix multiplication.
  @param[in]     pSrcA      points to the first input matrix structure
  @param[in]     pSrcB      points to the second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @param[in]     pState     points to the array for storing intermediate results
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator. The inputs to the
                   multiplications are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow.
                   The 34.30 result is then truncated to 34.15 format by discarding the low 15 bits
                   and then saturated to 1.15 format.
  @par
                   Refer to \ref riscv_mat_mult_fast_q15() for a faster but less precise version of this function.
 */
riscv_status riscv_mat_mult_q15(
  const riscv_matrix_instance_q15 * pSrcA,
  const riscv_matrix_instance_q15 * pSrcB,
        riscv_matrix_instance_q15 * pDst,
        q15_t                   * pState)
{
        q63_t sum;                                     /* Accumulator */
        q15_t *pSrcBT = pState;                        /* Input data matrix pointer for transpose */
        q15_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A of Q15 type */
        q15_t *pInB = pSrcB->pData;                    /* Input data matrix pointer B of Q15 type */
        q15_t *px;                                     /* Temporary output data matrix pointer */
        q15_t *pOut = pDst->pData;                     /* Output data matrix pointer */
        uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
        uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
        uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
        uint16_t numRowsB = pSrcB->numRows;            /* Number of rows of input matrix B */
        uint32_t col, i = 0U, row = numRowsB, colCnt;  /* Loop counters */
        riscv_status status;                             /* Status of matrix multiplication */

        q31_t inA1, inB1, inA2, inB2;
        riscv_matrix_instance_q15 BT;
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
        q63_t inA164, inB164;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */

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

#if defined(RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
     q15_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
     q15_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
    (void)pState;
    uint16_t blkCnt = numColsA;  //number of matrix columns  numColsA = numrowB
    size_t l;
    ptrdiff_t bstride = 2;       //  16bit/8bit = 2
    px = pOut;
    uint32_t ii, jj, kk;
    vint16m1_t va0m1, va1m1, va2m1, va3m1;
    vint16m2_t va0m2, va1m2;
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
          va0m1 = vlse16_v_i16m1(pInA + kk, numColsA * bstride, l);
          vres0m4 = vwmacc_vx_i64m4 (vres0m4, (int32_t)(*(pInB + 0)), vwadd_vx_i32m2 (va0m1, 0, l), l);
          vres1m4 = vwmacc_vx_i64m4 (vres1m4, (int32_t)(*(pInB + 1)), vwadd_vx_i32m2 (va0m1, 0, l), l);
          vres2m4 = vwmacc_vx_i64m4 (vres2m4, (int32_t)(*(pInB + 2)), vwadd_vx_i32m2 (va0m1, 0, l), l);
          vres3m4 = vwmacc_vx_i64m4 (vres3m4, (int32_t)(*(pInB + 3)), vwadd_vx_i32m2 (va0m1, 0, l), l);
          pInB += numColsB;
        }
        va0m1 = vnclip_wx_i16m1(vnsra_wx_i32m2(vres0m4, 15, l), 0, l);
        va1m1 = vnclip_wx_i16m1(vnsra_wx_i32m2(vres1m4, 15, l), 0, l);
        va2m1 = vnclip_wx_i16m1(vnsra_wx_i32m2(vres2m4, 15, l), 0, l);
        va3m1 = vnclip_wx_i16m1(vnsra_wx_i32m2(vres3m4, 15, l), 0, l);
        vssseg4e16_v_i16m1(px, numColsB * bstride, va0m1, va1m1, va2m1, va3m1, l);
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
          va0m2 = vlse16_v_i16m2(pInA + kk, numColsA * bstride, l);
          vres0m8 = vwmacc_vx_i64m8 (vres0m8, (int32_t)(*(pInB + 0)), vwadd_vx_i32m4 (va0m2, 0, l), l);
          vres1m8 = vwmacc_vx_i64m8 (vres1m8, (int32_t)(*(pInB + 1)), vwadd_vx_i32m4 (va0m2, 0, l), l);
          pInB += numColsB;
        }
        va0m2 = vnclip_wx_i16m2(vnsra_wx_i32m4(vres0m8, 15, l), 0, l);
        va1m2 = vnclip_wx_i16m2(vnsra_wx_i32m4(vres1m8, 15, l), 0, l);
        vssseg2e16_v_i16m2(px, numColsB * bstride, va0m2, va1m2, l);
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
          va0m2 = vlse16_v_i16m2(pInA + kk, numColsA * bstride, l);
          vres0m8 = vwmacc_vx_i64m8 (vres0m8, (int32_t)(*(pInB + 0)), vwadd_vx_i32m4 (va0m2, 0, l), l);
          pInB += numColsB;
        }
        va0m2 = vnclip_wx_i16m2(vnsra_wx_i32m4(vres0m8, 15, l), 0, l);
        vsse16_v_i16m2(px, numColsB * bstride, va0m2, l);
        px += l * numColsB;
        pInA += l * numColsA;
      }
      pIn2 += 1;
      pOut += 1;
    }
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
#else

#if defined (RISCV_MATH_DSP)
        q15_t *pSrcBT = pState;                        /* Input data matrix pointer for transpose */
        q31_t inA1, inB1, inA2, inB2;
        riscv_matrix_instance_q15 BT;
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
        q63_t inA164, inB164;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */

    BT.numRows = numColsB;
    BT.numCols = numRowsB;
    BT.pData = pSrcBT;

    riscv_mat_trans_q15(pSrcB,&BT);
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
        sum = 0;

        /* Initiate pointer pInA to point to starting address of column being processed */
        pInA = pSrcA->pData + i;

        /* Apply loop unrolling and compute 2 MACs simultaneously. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
          inA164 = read_q15x4_ia(&pInA);
          inB164 = read_q15x4_ia(&pInB);
          /* Multiply and Accumlates */
          sum = __SMLALD(inA164, inB164, sum);

#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
          inA164 = read_q15x4_ia(&pInA);
          inB164 = read_q15x4_ia(&pInB);
          /* Multiply and Accumlates */
          sum = __dsmalda(sum, inA164, inB164);
#else
          /* read real and imag values from pSrcA and pSrcB buffer */
          inA1 = read_q15x2_ia (&pInA);
          inB1 = read_q15x2_ia (&pInB);

          inA2 = read_q15x2_ia (&pInA);
          inB2 = read_q15x2_ia (&pInB);

          /* Multiply and Accumlates */
          sum = __SMLALD(inA1, inB1, sum);
          sum = __SMLALD(inA2, inB2, sum);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

          /* Decrement loop counter */
          colCnt--;
        }

        /* process remaining column samples */
        colCnt = numColsA & 0x3U;

        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */
          sum += *pInA++ * *pInB++;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Saturate and store result in destination buffer */
        *px = (q15_t) (__SSAT((sum >> 15), 16));
        px++;

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      i = i + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
#else /* #if defined (RISCV_MATH_DSP) */

        q15_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
        q15_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
        (void)pState;
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* Output pointer is set to starting address of the row being processed */
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

        /* Initiate pointer pIn1 to point to starting address of pSrcA */
        pIn1 = pInA;

        /* Matrix A columns number of MAC operations are to be performed */
        colCnt = numColsA;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform multiply-accumulates */
          sum += (q31_t) * pIn1++ * *pIn2;
          pIn2 += numColsB;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Convert result from 34.30 to 1.15 format and store saturated value in destination buffer */

        /* Saturate and store result in destination buffer */
        *px++ = (q15_t) __SSAT((sum >> 15), 16);

        /* Decrement column loop counter */
        col--;

        /* Update pointer pIn2 to point to starting address of next column */
        pIn2 = pInB + (numColsB - col);

      } while (col > 0U);

      /* Update pointer pSrcA to point to starting address of next row */
      i = i + numColsB;
      pInA = pInA + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);
#endif /* #if defined (RISCV_MATH_DSP) */
#endif /* defined(RISCV_MATH_VECTOR) */
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }
  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
