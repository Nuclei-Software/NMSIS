/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_conj_f16.c
 * Description:  Floating-point complex conjugate
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
  @addtogroup cmplx_conj
  @{
 */

/**
  @brief         Floating-point complex conjugate.
  @param[in]     pSrc        points to the input vector
  @param[out]    pDst        points to the output vector
  @param[in]     numSamples  number of samples in each vector
 */

RISCV_DSP_ATTRIBUTE void riscv_cmplx_conj_f16(
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t numSamples)
{
        uint32_t blkCnt;                               /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = numSamples;
  size_t l;
  vfloat16m8_t vx;
  l = __riscv_vsetvlmax_e16m8();
  // It works for vlen <= 1024
  const uint32_t mask_v[16] = {0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
                               0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
                               0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
                               0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA};
  const uint8_t *mask_v16 = (const uint8_t *)mask_v;
  vbool2_t mask = __riscv_vlm_v_b2(mask_v16, l);
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    vx = __riscv_vle16_v_f16m8(pSrc, l);
    pSrc += l;
    __riscv_vse16_v_f16m8(pDst, __riscv_vfmul_vf_f16m8_m(mask, vx, -1, l), l);
    pDst += l;
  }

#else

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] + jC[1] = A[0]+ j(-1)A[1] */

    /* Calculate Complex Conjugate and store result in destination buffer. */
    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

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
    /* C[0] + jC[1] = A[0]+ j(-1)A[1] */

    /* Calculate Complex Conjugate and store result in destination buffer. */
    *pDst++ =  *pSrc++;
    *pDst++ = -(_Float16)*pSrc++;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of cmplx_conj group
 */
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
