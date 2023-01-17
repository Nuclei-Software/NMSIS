/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_mult_opt_q31.c
 * Description:  Q31 matrix multiplication
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
  @brief         Q31 matrix multiplication.
  @param[in]     pSrcA      points to the first input matrix structure
  @param[in]     pSrcB      points to the second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @param[in]  pState  points to the array for storing intermediate results
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
  @remark
                   This function is a faster implementation of riscv_mat_mult_q31 for MVE but it is requiring
                   additional storage for intermediate results.
 */
riscv_status riscv_mat_mult_opt_q31(
  const riscv_matrix_instance_q31 * pSrcA,
  const riscv_matrix_instance_q31 * pSrcB,
        riscv_matrix_instance_q31 * pDst,
        q31_t *pState)
{
  q31_t *pIn1 = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pIn2 = pSrcB->pData;                    /* Input data matrix pointer B */
  q31_t *pInA = pSrcA->pData;                    /* Input data matrix pointer A */
  q31_t *pOut = pDst->pData;                     /* Output data matrix pointer */
  q31_t *pSrcBT = pState;                        /* Input data matrix pointer for transpose */
  q31_t *px = NULL;                              /* Temporary output data matrix pointer */
  q63_t sum;                                     /* Accumulator */
  uint16_t numRowsA = pSrcA->numRows;            /* Number of rows of input matrix A */
  uint16_t numRowsB = pSrcB->numRows;            /* Number of rows of input matrix B */
  uint16_t numColsB = pSrcB->numCols;            /* Number of columns of input matrix B */
  uint16_t numColsA = pSrcA->numCols;            /* Number of columns of input matrix A */
  uint32_t col, i = 0U, row = numRowsA, colCnt;  /* Loop counters */
  riscv_status status;                             /* Status of matrix multiplication */
  //(void)pState;
  riscv_matrix_instance_q31 BT;

#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
  q63_t tmpVal1, tmpVal2;
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
    BT.numRows = numColsB;
    BT.numCols = numRowsB;
    BT.pData = pSrcBT;

    /* Matrix transpose by exchanging the rows with columns */
    status = riscv_mat_trans_q31(pSrcB, &BT);
    if (status != RISCV_MATH_SUCCESS)
    {
      return status;
    }

    px = pDst->pData;
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pIn2 = pSrcBT;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0;

        /* Initiate the pointer pIn1 to point to the starting address of pSrcA */
        pIn1 = pInA + i;

        /* Initialize cntCnt with number of columns */
        colCnt = numColsA;

        uint16_t blkCnt = colCnt;
        size_t l;
        vint64m1_t vsum;
        vint32m4_t v_inA, v_inB;
        l = vsetvl_e64m1(1);
        vsum = vmv_v_x_i64m1(0, l);
        for (; (l = vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l) {
          v_inA = vle32_v_i32m4(pIn1, l);
          v_inB = vle32_v_i32m4(pIn2, l);
          pIn1 += l;
          pIn2 += l;
          vsum = vredsum_vs_i64m8_i64m1(vsum, vwmul_vv_i64m8(v_inA, v_inB, l), vsum, l);
        }
        sum += vmv_x_s_i64m1_i64(vsum);

        /* Convert result from 2.62 to 1.31 format and store in destination buffer */
        *px++ = (q31_t) (sum >> 31);

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      /* Update the pointer pSrcA to point to the  starting address of the next row */
      i = i + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);

#else

    /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
    /* row loop */
    do
    {
      /* For every row wise process, column loop counter is to be initiated */
      col = numColsB;

      /* For every row wise process, pIn2 pointer is set to starting address of pSrcB data */
      pIn2 = pSrcBT;

      /* column loop */
      do
      {
        /* Set the variable sum, that acts as accumulator, to zero */
        sum = 0;

        /* Initiate the pointer pIn1 to point to the starting address of pSrcA */
        pIn1 = pInA + i;

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Loop unrolling: Compute 4 MACs at a time. */
        colCnt = numColsA >> 2U;

        /* matrix multiplication */
        while (colCnt > 0U)
        {
          /* c(m,n) = a(1,1) * b(1,1) + a(1,2) * b(2,1) + .... + a(m,p) * b(p,n) */

          /* Perform the multiply-accumulates */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
          tmpVal1 = read_q31x2_ia(&pIn1);
          tmpVal2 = read_q31x2_ia(&pIn2);
          sum = __RV_SMAR64(sum, tmpVal1, tmpVal2);

          tmpVal1 = read_q31x2_ia(&pIn1);
          tmpVal2 = read_q31x2_ia(&pIn2);
          sum = __RV_SMAR64(sum, tmpVal1, tmpVal2);
#else
          sum += (q63_t)*pIn1++ * *pIn2++;

          sum += (q63_t)*pIn1++ * *pIn2++;

          sum += (q63_t)*pIn1++ * *pIn2++;

          sum += (q63_t)*pIn1++ * *pIn2++;
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
          sum += (q63_t) *pIn1++ * *pIn2++;

          /* Decrement loop counter */
          colCnt--;
        }

        /* Convert result from 2.62 to 1.31 format and store in destination buffer */
        *px++ = (q31_t) (sum >> 31);

        /* Decrement column loop counter */
        col--;

      } while (col > 0U);

      /* Update the pointer pSrcA to point to the  starting address of the next row */
      i = i + numColsA;

      /* Decrement row loop counter */
      row--;

    } while (row > 0U);
#endif /*defined(RISCV_MATH_VECTOR)*/
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixMult group
 */
