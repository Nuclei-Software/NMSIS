/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mat_scale_q15.c
 * Description:  Multiplies a Q15 matrix by a scalar
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
  @brief         Q15 matrix scaling.
  @param[in]     pSrc        points to input matrix
  @param[in]     scaleFract  fractional portion of the scale factor
  @param[in]     shift       number of bits to shift the result by
  @param[out]    pDst        points to output matrix structure
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS       : Operation successful
                   - \ref RISCV_MATH_SIZE_MISMATCH : Matrix size check failed

  @par           Scaling and Overflow Behavior
                   The input data <code>*pSrc</code> and <code>scaleFract</code> are in 1.15 format.
                   These are multiplied to yield a 2.30 intermediate result and this is shifted with saturation to 1.15 format.
 */
riscv_status riscv_mat_scale_q15(
  const riscv_matrix_instance_q15 * pSrc,
        q15_t                     scaleFract,
        int32_t                   shift,
        riscv_matrix_instance_q15 * pDst)
{
        q15_t *pIn = pSrc->pData;                      /* Input data matrix pointer */
        q15_t *pOut = pDst->pData;                     /* Output data matrix pointer */
        uint32_t numSamples;                           /* Total number of elements in the matrix */
        uint32_t blkCnt;                               /* Loop counter */
        riscv_status status;                           /* Status of matrix scaling */
        int32_t kShift = 15 - shift;                   /* Total shift to apply after scaling */

#if defined (RISCV_MATH_LOOPUNROLL)
#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32)
        q63_t out12, out34;
        q31_t scaleTemp = __RV_DPKBB16(scaleFract, scaleFract);
#else
        q63_t out12, out34;
        q31_t scaleTemp = __RV_PKBB16(scaleFract, scaleFract);
#endif /* defined (NUCLEI_DSP_N2) && (__RISCV_XLEN == 32) */
#endif /* defined (RISCV_MATH_DSP) */
        q63_t out164, out264;
        q31_t inA1, inA2;
        q31_t out1, out2, out3, out4;                  /* Temporary output variables */
        q15_t in1, in2, in3, in4;                      /* Temporary input variables */
#endif /* defined (RISCV_MATH_LOOPUNROLL) */

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
#if defined(RISCV_MATH_VECTOR)
    blkCnt = numSamples;
    size_t l;
    vint16m4_t vx;
    for (; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l) {
      vx = __riscv_vle16_v_i16m4(pIn, l);
      pIn += l;
      __riscv_vse16_v_i16m4(pOut, __riscv_vnclip_wx_i16m4(__riscv_vwmul_vx_i32m8(vx, scaleFract, l), kShift, __RISCV_VXRM_RNU, l), l);
      pOut += l;
    }
      /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;

#elif defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
      /* C(m,n) = A(m,n) * k */

#if defined (RISCV_MATH_DSP)
      /* read 2 times 2 samples at a time from source */
      inA1 = read_q15x2_ia(&pIn);
      out164 = __RV_SMUL16(inA1, scaleTemp);
      inA1 = read_q15x2_ia(&pIn);
      out264 = __RV_SMUL16(inA1, scaleTemp);

#if __RISCV_XLEN == 64
      out12 = __RV_KSLRA32(out164, -kShift);
      out34 = __RV_KSLRA32(out264, -kShift);
      out1 = (q31_t)(out12 >> 32);
      out2 = (q31_t)out12;
      out3 = (q31_t)(out34 >> 32);
      out4 = (q31_t)out34;
#else
#ifdef NUCLEI_DSP_N2
      /* apply shifting and saturate the output */
      out12 = __dkslra32(out164, -kShift);
      out34 = __dkslra32(out264, -kShift);
      out1 = (q31_t)(out12 >> 32);
      out2 = (q31_t)out12;
      out3 = (q31_t)(out34 >> 32);
      out4 = (q31_t)out34;
#else
      out1 = (q31_t)(out164 >> 32) >> kShift;
      out2 = (q31_t)out164 >> kShift;
      out3 = (q31_t)(out264 >> 32) >> kShift;
      out4 = (q31_t)out264 >> kShift;
#endif /* NUCLEI_DSP_N2 */
#endif /* __RISCV_XLEN == 64 */

      /* saturate the output */
      in1 = (q15_t) (__SSAT(out1, 16));
      in2 = (q15_t) (__SSAT(out2, 16));
      in3 = (q15_t) (__SSAT(out3, 16));
      in4 = (q15_t) (__SSAT(out4, 16));

      /* store result to destination */
      write_q15x2_ia (&pOut, __PKHBT(in2, in1, 16));
      write_q15x2_ia (&pOut, __PKHBT(in4, in3, 16));

#else
      *pOut++ = (q15_t) (__SSAT(((q31_t) (*pIn++) * scaleFract) >> kShift, 16));
      *pOut++ = (q15_t) (__SSAT(((q31_t) (*pIn++) * scaleFract) >> kShift, 16));
      *pOut++ = (q15_t) (__SSAT(((q31_t) (*pIn++) * scaleFract) >> kShift, 16));
      *pOut++ = (q15_t) (__SSAT(((q31_t) (*pIn++) * scaleFract) >> kShift, 16));
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
      *pOut++ = (q15_t) (__SSAT(((q31_t) (*pIn++) * scaleFract) >> kShift, 16));

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);
}

/**
  @} end of MatrixScale group
 */
