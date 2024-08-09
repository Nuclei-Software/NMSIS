/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mult_real_f16.c
 * Description:  Floating-point complex by real multiplication
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

#include "dsp/complex_math_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

/**
  @ingroup groupCmplxMath
 */


/**
  @addtogroup CmplxByRealMult
  @{
 */

/**
  @brief         Floating-point complex-by-real multiplication.
  @param[in]     pSrcCmplx   points to complex input vector
  @param[in]     pSrcReal    points to real input vector
  @param[out]    pCmplxDst   points to complex output vector
  @param[in]     numSamples  number of samples in each vector
 */

RISCV_DSP_ATTRIBUTE void riscv_cmplx_mult_real_f16(
  const float16_t * pSrcCmplx,
  const float16_t * pSrcReal,
        float16_t * pCmplxDst,
        uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
  uint32_t blkCnt = numSamples;                               /* Loop counter */
  size_t l;

  ptrdiff_t bstride = 4;
  vfloat16m8_t v_Rc, v_Ic, v_Rr;
  vfloat16m8_t vR2_m8, vI2_m8;
  vfloat16m8_t v_sum;

  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_Rc = __riscv_vlse16_v_f16m8(pSrcCmplx, bstride, l);
    v_Ic = __riscv_vlse16_v_f16m8(pSrcCmplx + 1, bstride, l);
    v_Rr = __riscv_vle16_v_f16m8(pSrcReal, l);
    pSrcReal += l;
    pSrcCmplx += l * 2;

    vR2_m8 = __riscv_vfmul_vv_f16m8(v_Rc, v_Rr, l);
    vI2_m8 = __riscv_vfmul_vv_f16m8(v_Ic, v_Rr, l);
    __riscv_vsse16_v_f16m8(pCmplxDst, bstride, vR2_m8, l);
    __riscv_vsse16_v_f16m8(pCmplxDst + 1, bstride, vI2_m8, l);
    pCmplxDst += l * 2;
  }
#else
        uint32_t blkCnt;                               /* Loop counter */
        float16_t in;                                  /* Temporary variable */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i    ] * B[i]. */
    /* C[2 * i + 1] = A[2 * i + 1] * B[i]. */

    in = *pSrcReal++;
    /* store result in destination buffer. */
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;

    in = *pSrcReal++;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;

    in = *pSrcReal++;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;

    in = *pSrcReal++;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;

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
    /* C[2 * i    ] = A[2 * i    ] * B[i]. */
    /* C[2 * i + 1] = A[2 * i + 1] * B[i]. */

    in = *pSrcReal++;
    /* store result in destination buffer. */
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;
    *pCmplxDst++ = (_Float16)*pSrcCmplx++ * (_Float16)in;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of CmplxByRealMult group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
