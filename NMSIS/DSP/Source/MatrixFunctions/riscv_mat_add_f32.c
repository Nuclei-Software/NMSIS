/* --------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_add_f32.c
 * Description:  Floating-point matrix addition
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
  @defgroup MatrixAdd Matrix Addition

  Adds two matrices.
  @par Addition of two 3 x 3 matrices

  \f[
  \begin{pmatrix}
   a_{1,1} & a_{1,2} & a_{1,3} \\
   a_{2,1} & a_{2,2} & a_{2,3} \\
   a_{3,1} & a_{3,2} & a_{3,3} \\
  \end{pmatrix}
  +
  \begin{pmatrix}
   b_{1,1} & b_{1,2} & b_{1,3} \\
   b_{2,1} & b_{2,2} & b_{2,3} \\
   b_{3,1} & b_{3,2} & b_{3,3} \\
  \end{pmatrix}
  =
  \begin{pmatrix}
   a_{1,1}+b_{1,1} & a_{1,2}+b_{1,2} & a_{1,3}+b_{1,3} \\
   a_{2,1}+b_{2,1} & a_{2,2}+b_{2,2} & a_{2,3}+b_{2,3} \\
   a_{3,1}+b_{3,1} & a_{3,2}+b_{3,2} & a_{3,3}+b_{3,3} \\
  \end{pmatrix}
  \f]

  The functions check to make sure that
  <code>pSrcA</code>, <code>pSrcB</code>, and <code>pDst</code> have the same
  number of rows and columns.
 */

/**
  @addtogroup MatrixAdd
  @{
 */


/**
  @brief         Floating-point matrix addition.
  @param[in]     pSrcA      points to first input matrix structure
  @param[in]     pSrcB      points to second input matrix structure
  @param[out]    pDst       points to output matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed
 */

RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_add_f32(
  const riscv_matrix_instance_f32 * pSrcA,
  const riscv_matrix_instance_f32 * pSrcB,
        riscv_matrix_instance_f32 * pDst)
{
  float32_t *pInA = pSrcA->pData;                /* input data matrix pointer A */
  float32_t *pInB = pSrcB->pData;                /* input data matrix pointer B */
  float32_t *pOut = pDst->pData;                 /* output data matrix pointer */

  uint32_t numSamples;                           /* total number of elements in the matrix */
  uint32_t blkCnt;                               /* loop counters */
  riscv_status status;                             /* status of matrix addition */

#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrcA->numRows != pSrcB->numRows) ||
      (pSrcA->numCols != pSrcB->numCols) ||
      (pSrcA->numRows != pDst->numRows)  ||
      (pSrcA->numCols != pDst->numCols)    )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */

  {
    /* Total number of samples in input matrix */
    numSamples = (uint32_t) pSrcA->numRows * pSrcA->numCols;
#if defined(RISCV_MATH_VECTOR)
    blkCnt = numSamples;
    size_t l;
    vfloat32m8_t vx, vy;
    for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
      vx = __riscv_vle32_v_f32m8(pInA, l);
      pInA += l;
      vy = __riscv_vle32_v_f32m8(pInB, l);
      pInB += l;
      __riscv_vse32_v_f32m8(pOut, __riscv_vfadd_vv_f32m8(vy, vx, l), l);
      pOut += l;
    }
#else
#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) + B(m,n) */

      /* Add and store result in destination buffer. */
      *pOut++ = *pInA++ + *pInB++;

      *pOut++ = *pInA++ + *pInB++;

      *pOut++ = *pInA++ + *pInB++;

      *pOut++ = *pInA++ + *pInB++;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples & 0x3U;

#else

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) + B(m,n) */

      /* Add and store result in destination buffer. */
      *pOut++ = *pInA++ + *pInB++;

      /* Decrement loop counter */
      blkCnt--;
    }
#endif /* defined(RISCV_MATH_VECTOR) */
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixAdd group
 */
