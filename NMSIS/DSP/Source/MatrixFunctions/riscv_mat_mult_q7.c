/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_mult_q7.c
 * Description:  Q15 matrix multiplication
 *
 * $Date:        23 April 2021
 *
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
 * @brief Q7 matrix multiplication
 * @param[in]       *pSrcA points to the first input matrix structure
 * @param[in]       *pSrcB points to the second input matrix structure
 * @param[out]      *pDst points to output matrix structure
 * @param[in]       *pState points to the array for storing intermediate results (Unused in some versions)
 * @return          The function returns either
 * <code>RISCV_MATH_SIZE_MISMATCH</code> or <code>RISCV_MATH_SUCCESS</code> based on the outcome of size checking.
 *
 * @details
 * <b>Scaling and Overflow Behavior:</b>
 *
 * \par
 * The function is implemented using a 32-bit internal accumulator saturated to 1.7 format.
 *
 *
 */
riscv_status riscv_mat_mult_q7(const riscv_matrix_instance_q7 *pSrcA, const riscv_matrix_instance_q7 *pSrcB, riscv_matrix_instance_q7 *pDst, q7_t *pState)
{
    q31_t sum; /* accumulator */
    q7_t *pIn1 = pSrcA->pData;                    /* input data matrix pointer A */
    q7_t *pIn2 = pSrcB->pData;                    /* input data matrix pointer B */
    q7_t *pInA = pSrcA->pData;                    /* input data matrix pointer A of Q7 type */
    q7_t *pOut = pDst->pData;                     /* output data matrix pointer */
    q7_t *pSrcBT = pState;                        /* Input data matrix pointer for transpose */
    q7_t *px = NULL;                              /* Temporary output data matrix pointer */
    uint16_t numColsB = pSrcB->numCols;           /* number of columns of input matrix B */
    uint16_t numColsA = pSrcA->numCols;           /* number of columns of input matrix A */
    uint16_t numRowsA = pSrcA->numRows;           /* number of rows of input matrix A    */
    uint16_t numRowsB = pSrcB->numRows;            /* Number of rows of input matrix B */
    uint16_t col, i = 0U, row = numRowsA, colCnt; /* loop counters */
    riscv_status status;                            /* status of matrix multiplication */
    riscv_matrix_instance_q7 BT;

#if defined (RISCV_MATH_DSP)
    q31_t tmpVal1, tmpVal2;
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
        BT.numRows = numColsB;
        BT.numCols = numRowsB;
        BT.pData = pSrcBT;

        /* Matrix transpose by exchanging the rows with columns */
	status = riscv_mat_trans_q7(pSrcB, &BT);
	if (status != RISCV_MATH_SUCCESS)
	{
	    return status;
	}

	px = pDst->pData;

        /* The following loop performs the dot-product of each row in pSrcA with each column in pSrcB */
        /* row loop */
        do {
            /* For every row wise process, the column loop counter is to be initiated */
            col = numColsB;

            /* For every row wise process, the pIn2 pointer is set
             ** to the starting address of the pSrcB data */
            pIn2 = pSrcBT;

            /* column loop */
            do {
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
                    /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
#if defined (RISCV_MATH_DSP)
                    tmpVal1 = read_q7x4_ia(&pIn1);
                    tmpVal2 = read_q7x4_ia(&pIn2);
                    sum = __RV_SMAQA(sum, tmpVal1, tmpVal2);
#else
                    sum += (q31_t)*pIn1++ * *pIn2++;
                    sum += (q31_t)*pIn1++ * *pIn2++;
                    sum += (q31_t)*pIn1++ * *pIn2++;
                    sum += (q31_t)*pIn1++ * *pIn2++;
#endif /* defined (RISCV_MATH_DSP) */
                    /* Decrement loop counter */
                    colCnt--;
                }

                /* Loop unrolling: Compute remaining MACs */
                colCnt = numColsA & 0x3U;
#else

                /* Matrix A columns number of MAC operations are to be performed */
                colCnt = numColsA;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

                /* matrix multiplication */
                while (colCnt > 0U) {
                    /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
                    /* Perform the multiply-accumulates */

                    sum += (q31_t)*pIn1++ * *pIn2++;

                    /* Decrement the loop counter */
                    colCnt--;
                }

                /* Convert the result from 34.30 to 1.15 format and store the saturated value in destination buffer */
                /* Saturate and store the result in the destination buffer */
                *px++ = (q7_t)__SSAT((sum >> 7), 8);

                /* Decrement the column loop counter */
                col--;

            } while (col > 0U);

            /* Update the pointer pSrcA to point to the  starting address of the next row */
            i = i + numColsA;

            /* Decrement the row loop counter */
            row--;

        } while (row > 0U);

        /* set status as RISCV_MATH_SUCCESS */
        status = RISCV_MATH_SUCCESS;
    }

    /* Return to application */
    return (status);
}

/**
  @} end of MatrixMult group
 */
