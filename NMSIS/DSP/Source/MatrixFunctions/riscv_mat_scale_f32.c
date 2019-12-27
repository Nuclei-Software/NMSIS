/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_scale_f32.c
 * Description:  Multiplies a floating-point matrix by a scalar
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: RISC-V cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
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

#include "riscv_math.h"

/**
  @ingroup groupMatrix
 */

/**
  @defgroup MatrixScale Matrix Scale

  Multiplies a matrix by a scalar.  This is accomplished by multiplying each element in the
  matrix by the scalar.  For example:
  \image html MatrixScale.png "Matrix Scaling of a 3 x 3 matrix"

  The function checks to make sure that the input and output matrices are of the same size.

  In the fixed-point Q15 and Q31 functions, <code>scale</code> is represented by
  a fractional multiplication <code>scaleFract</code> and an arithmetic shift <code>shift</code>.
  The shift allows the gain of the scaling operation to exceed 1.0.
  The overall scale factor applied to the fixed-point data is
  <pre>
      scale = scaleFract * 2^shift.
  </pre>
 */

/**
  @addtogroup MatrixScale
  @{
 */

/**
  @brief         Floating-point matrix scaling.
  @param[in]     pSrc       points to input matrix
  @param[in]     scale      scale factor to be applied
  @param[out]    pDst       points to output matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed
 */
#if defined(RISCV_MATH_NEON_EXPERIMENTAL)
riscv_status riscv_mat_scale_f32(
  const riscv_matrix_instance_f32 * pSrc,
  float32_t scale,
  riscv_matrix_instance_f32 * pDst)
{
  float32_t *pIn = pSrc->pData;                  /* input data matrix pointer */
  float32_t *pOut = pDst->pData;                 /* output data matrix pointer */
  uint32_t numSamples;                           /* total number of elements in the matrix */
  uint32_t blkCnt;                               /* loop counters */
  riscv_status status;                             /* status of matrix scaling     */


  float32_t in1, in2, in3, in4;                  /* temporary variables */
  float32_t out1, out2, out3, out4;              /* temporary variables */


#ifdef RISCV_MATH_MATRIX_CHECK
  /* Check for matrix mismatch condition */
  if ((pSrc->numRows != pDst->numRows) || (pSrc->numCols != pDst->numCols))
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else
#endif /*    #ifdef RISCV_MATH_MATRIX_CHECK    */
  {
    float32x4_t vec1;
    float32x4_t res;

    /* Total number of samples in the input matrix */
    numSamples = (uint32_t) pSrc->numRows * pSrc->numCols;

    blkCnt = numSamples >> 2;

    /* Compute 4 outputs at a time.
     ** a second loop below computes the remaining 1 to 3 samples. */
    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * scale */
      /* Scaling and results are stored in the destination buffer. */
      vec1 = vld1q_f32(pIn);
      res = vmulq_f32(vec1, vdupq_n_f32(scale));
      vst1q_f32(pOut, res);

      /* update pointers to process next sampels */
      pIn += 4U;
      pOut += 4U;

      /* Decrement the numSamples loop counter */
      blkCnt--;
    }

    /* If the numSamples is not a multiple of 4, compute any remaining output samples here.
     ** No loop unrolling is used. */
    blkCnt = numSamples % 0x4U;

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * scale */
      /* The results are stored in the destination buffer. */
      *pOut++ = (*pIn++) * scale;

      /* Decrement the loop counter */
      blkCnt--;
    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}
#else
riscv_status riscv_mat_scale_f32(
  const riscv_matrix_instance_f32 * pSrc,
        float32_t                 scale,
        riscv_matrix_instance_f32 * pDst)
{
  float32_t *pIn = pSrc->pData;                  /* Input data matrix pointer */
  float32_t *pOut = pDst->pData;                 /* Output data matrix pointer */
  uint32_t numSamples;                           /* Total number of elements in the matrix */
  uint32_t blkCnt;                               /* Loop counters */
  riscv_status status;                             /* Status of matrix scaling */

#ifdef RISCV_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((pSrc->numRows != pDst->numRows) ||
      (pSrc->numCols != pDst->numCols)   )
  {
    /* Set status as RISCV_MATH_SIZE_MISMATCH */
    status = RISCV_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef RISCV_MATH_MATRIX_CHECK */

  {
    /* Total number of samples in input matrix */
    numSamples = (uint32_t) pSrc->numRows * pSrc->numCols;

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * scale */

      /* Scale and store result in destination buffer. */
      *pOut++ = (*pIn++) * scale;
      *pOut++ = (*pIn++) * scale;
      *pOut++ = (*pIn++) * scale;
      *pOut++ = (*pIn++) * scale;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;

#else

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * scale */

      /* Scale and store result in destination buffer. */
      *pOut++ = (*pIn++) * scale;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}
#endif /* #if defined(RISCV_MATH_NEON) */

/**
  @} end of MatrixScale group
 */
