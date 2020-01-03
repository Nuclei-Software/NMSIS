/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mag_squared_q31.c
 * Description:  Q31 complex magnitude squared
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: RISC-V Cores
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
  @ingroup groupCmplxMath
 */

/**
  @addtogroup cmplx_mag_squared
  @{
 */

/**
  @brief         Q31 complex magnitude squared.
  @param[in]     pSrc        points to input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function implements 1.31 by 1.31 multiplications and finally output is converted into 3.29 format.
                   Input down scaling is not required.
 */

void riscv_cmplx_mag_squared_q31(
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */
        q31_t real, imag;                              /* Temporary input variables */
        q31_t acc0, acc1;                              /* Accumulators */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] = (A[0] * A[0] + A[1] * A[1]) */

    real = *pSrc++;
    imag = *pSrc++;

#if defined(RISCV_MATH_DSP)
	acc0 = (q31_t) (__MULSR64(real, real) >> 33);
	acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#else
	acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif

    /* store the result in 3.29 format in the destination buffer. */
    *pDst++ = acc0 + acc1;

    real = *pSrc++;
    imag = *pSrc++;

#if defined(RISCV_MATH_DSP)
	acc0 = (q31_t) (__MULSR64(real, real) >> 33);
	acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#else
	acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif

    *pDst++ = acc0 + acc1;

    real = *pSrc++;
    imag = *pSrc++;

#if defined(RISCV_MATH_DSP)
	acc0 = (q31_t) (__MULSR64(real, real) >> 33);
	acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#else
	acc0 = (q31_t) (((q63_t) real * real) >> 33);
	acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif

    *pDst++ = acc0 + acc1;

    real = *pSrc++;
    imag = *pSrc++;

#if defined(RISCV_MATH_DSP)
	acc0 = (q31_t) (__MULSR64(real, real) >> 33);
	acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#else
	acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif

    *pDst++ = acc0 + acc1;

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
    /* C[0] = (A[0] * A[0] + A[1] * A[1]) */

    real = *pSrc++;
    imag = *pSrc++;

#if defined(RISCV_MATH_DSP)
	acc0 = (q31_t) (__MULSR64(real, real) >> 33);
	acc1 = (q31_t) (__MULSR64(imag, imag) >> 33);
#else
	acc0 = (q31_t) (((q63_t) real * real) >> 33);
    acc1 = (q31_t) (((q63_t) imag * imag) >> 33);
#endif

    /* store result in 3.29 format in destination buffer. */
    *pDst++ = acc0 + acc1;

    /* Decrement loop counter */
    blkCnt--;
  }

}

/**
  @} end of cmplx_mag_squared group
 */
