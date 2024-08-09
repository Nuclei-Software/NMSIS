/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_scale_q31.c
 * Description:  Multiplies a Q31 matrix by a scalar
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
  @addtogroup MatrixScale
  @{
 */

/**
  @brief         Q31 matrix scaling.
  @param[in]     pSrc        points to input matrix
  @param[in]     scaleFract  fractional portion of the scale factor
  @param[in]     shift       number of bits to shift the result by
  @param[out]    pDst        points to output matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The input data <code>*pSrc</code> and <code>scaleFract</code> are in 1.31 format.
                   These are multiplied to yield a 2.62 intermediate result which is shifted with saturation to 1.31 format.
 */
RISCV_DSP_ATTRIBUTE riscv_status riscv_mat_scale_q31(
  const riscv_matrix_instance_q31 * pSrc,
        q31_t                     scaleFract,
        int32_t                   shift,
        riscv_matrix_instance_q31 * pDst)
{
  q31_t *pIn = pSrc->pData;                      /* Input data matrix pointer */
  q31_t *pOut = pDst->pData;                     /* Output data matrix pointer */
  uint32_t numSamples;                           /* Total number of elements in the matrix */
  uint32_t blkCnt;                               /* Loop counter */
  riscv_status status;                             /* Status of matrix scaling */
  int32_t kShift = shift + 1;                    /* Shift to apply after scaling */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64))
  q63_t in;
#else
  q31_t in;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)) */
  q31_t out;                                 /* Temporary variabels */

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

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    blkCnt = numSamples;
    size_t l;
    vint32m4_t v_in, v_out;
    for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l) {
      v_in = __riscv_vle32_v_i32m4(pIn, l);
      pIn += l;
      v_in = __riscv_vnsra_wx_i32m4(__riscv_vwmul_vx_i64m8(v_in, scaleFract, l), 32U, l);
      v_out = __riscv_vsll_vx_i32m4(v_in, (uint8_t)kShift, l);
      __riscv_vse32_v_i32m4(pOut, v_out, l);
      pOut += l;
    }
      /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
#else
#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        q63_t Double_scaleFract = __RV_PKBB32(scaleFract, scaleFract);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
        q63_t Double_scaleFract = __RV_DPKBB32(scaleFract, scaleFract);
#else
        q63_t Double_scaleFract = ((q63_t)scaleFract << 32) | (((q63_t)scaleFract) & 0xffffffff);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * k */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
      in = read_q31x2_ia ((q31_t **) &pIn);              /* read two inputs from source */
      in = __RV_SMMUL(in, Double_scaleFract);          /* multiply input with scaler value */
      write_q31x2_ia (&pOut, __RV_KSLRA32(in, kShift)); /* apply shifting, saturate and Store result destination*/

      in = read_q31x2_ia ((q31_t **) &pIn);
      in = __RV_SMMUL(in, Double_scaleFract);
      write_q31x2_ia (&pOut, __RV_KSLRA32(in, kShift));
#else
#ifdef NUCLEI_DSP_N2
      /* Scale, saturate and store result in destination buffer. */
      in = read_q31x2_ia ((q31_t **) &pIn);              /* read two inputs from source */
      in = __dsmmul(in, Double_scaleFract);            /* multiply input with scaler value */
      write_q31x2_ia (&pOut, __dkslra32(in, kShift));   /* apply shifting, saturate and Store result destination*/

      in = read_q31x2_ia ((q31_t **) &pIn);
      in = __dsmmul(in, Double_scaleFract);
      write_q31x2_ia (&pOut, __dkslra32(in, kShift));
#else
      in = *pIn++;                                     /* read one inputs from source */
      in = __RV_SMMUL(in, scaleFract);                 /* multiply input with scaler value */
      out = __SSAT((q31_t)in << kShift, 32);           /* apply shifting and saturate the results */
      *pOut++ = out;                                   /* Store result destination */

      in = *pIn++;
      in = __RV_SMMUL(in, scaleFract);
      out = __SSAT((q31_t)in << kShift, 32);
      *pOut++ = out;

      in = *pIn++;
      in = __RV_SMMUL(in, scaleFract);
      out = __SSAT((q31_t)in << kShift, 32);
      *pOut++ = out;

      in = *pIn++;
      in = __RV_SMMUL(in, scaleFract);
      out = __SSAT((q31_t)in << kShift, 32);
      *pOut++ = out;
#endif /* NUCLEI_DSP_N2 */
#endif /* __RISCV_XLEN == 64 */
#else
      /* Scale, saturate and store result in destination buffer. */
      in = *pIn++;                                 /* read one inputs from source */
      in = ((q63_t) in * scaleFract) >> 32;        /* multiply input with scaler value */
      out = in << kShift;                          /* apply shifting */
      if (in != (out >> kShift))                   /* saturate the results. */
        out = 0x7FFFFFFF ^ (in >> 31);
      *pOut++ = out;                               /* Store result destination */

      in = *pIn++;
      in = ((q63_t) in * scaleFract) >> 32;
      out = in << kShift;
      if (in != (out >> kShift))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pOut++ = out;

      in = *pIn++;
      in = ((q63_t) in * scaleFract) >> 32;
      out = in << kShift;
      if (in != (out >> kShift))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pOut++ = out;

      in = *pIn++;
      in = ((q63_t) in * scaleFract) >> 32;
      out = in << kShift;
      if (in != (out >> kShift))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pOut++ = out;
#endif /* defined (RISCV_MATH_DSP) */

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
      /* C(m,n) = A(m,n) * k */

      /* Scale, saturate and store result in destination buffer. */
      in = *pIn++;
      in = ((q63_t) in * scaleFract) >> 32;
      out = in << kShift;
      if (in != (out >> kShift))
        out = 0x7FFFFFFF ^ (in >> 31);
      *pOut++ = out;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
#endif /* defined(RISCV_MATH_VECTOR) */
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixScale group
 */
