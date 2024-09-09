/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_conj_q15.c
 * Description:  Q15 complex conjugate
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

#include "dsp/complex_math_functions.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @addtogroup cmplx_conj
  @{
 */

/**
  @brief         Q15 complex conjugate.
  @param[in]     pSrc        points to the input vector
  @param[out]    pDst        points to the output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   The Q15 value -1 (0x8000) is saturated to the maximum allowable positive value 0x7FFF.
 */


void riscv_cmplx_conj_q15(
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
  uint32_t blkCnt = numSamples;                               /* Loop counter */
  size_t l;
  vint16m8_t vx;
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
    vx = __riscv_vle16_v_i16m8(pSrc, l);
    __riscv_vse16_v_i16m8(pDst, __riscv_vmul_vx_i16m8_m(mask, vx, -1, l), l);
    pSrc += l;
    pDst += l;
  }

#else
  unsigned long blkCnt;               /* Loop counter */
  q31_t in1;                          /* Temporary input variable */

#if defined (RISCV_MATH_LOOPUNROLL) && defined (RISCV_MATH_DSP)
#if defined(NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)
  q63_t in641, in642, in643, in644;
#else
  q31_t in2, in3, in4;            /* Temporary input variables */
#endif /* defined(NUCLEI_DSP_N2) || (__RISCV_XLEN == 64) */
#endif /* defined (RISCV_MATH_LOOPUNROLL) && defined (RISCV_MATH_DSP) */


#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[0] + jC[1] = A[0]+ j(-1)A[1] */

    /* Calculate Complex Conjugate and store result in destination buffer. */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    in641 = read_q15x4_ia((q15_t **) &pSrc);
    in641 = __RV_KSTSA16(0, in641);
    write_q15x4_ia (&pDst, in641);
    in641 = read_q15x4_ia((q15_t **) &pSrc);
    in641 = __RV_KSTSA16(0, in641);
    write_q15x4_ia(&pDst, in641);
#else
#if defined(NUCLEI_DSP_N2)
    in641 = read_q15x4_ia((q15_t **) &pSrc);
    in641 = __RV_DKSTSA16(0, in641);
    write_q15x4_ia (&pDst, in641);
    in641 = read_q15x4_ia((q15_t **) &pSrc);
    in641 = __RV_DKSTSA16(0, in641);
    write_q15x4_ia(&pDst, in641);
#else
    in1 = read_q15x2_ia((q15_t **)&pSrc);
    in2 = read_q15x2_ia((q15_t **)&pSrc);
    in3 = read_q15x2_ia((q15_t **)&pSrc);
    in4 = read_q15x2_ia((q15_t **)&pSrc);

    in1 = __RV_KSTSA16(0, in1);
    in2 = __RV_KSTSA16(0, in2);
    in3 = __RV_KSTSA16(0, in3);
    in4 = __RV_KSTSA16(0, in4);

    write_q15x2_ia(&pDst, in1);
    write_q15x2_ia(&pDst, in2);
    write_q15x2_ia(&pDst, in3);
    write_q15x2_ia(&pDst, in4);
#endif /* defined(NUCLEI_DSP_N2) */
#endif /* __RISCV_XLEN == 64 */
#else
    *pDst++ = *pSrc++;
    in1 = *pSrc++;
    *pDst++ = (in1 == (q15_t) 0x8000) ? (q15_t) 0x7fff : -in1;

    *pDst++ = *pSrc++;
    in1 = *pSrc++;
    *pDst++ = (in1 == (q15_t) 0x8000) ? (q15_t) 0x7fff : -in1;

    *pDst++ = *pSrc++;
    in1 = *pSrc++;
    *pDst++ = (in1 == (q15_t) 0x8000) ? (q15_t) 0x7fff : -in1;

    *pDst++ = *pSrc++;
    in1 = *pSrc++;
    *pDst++ = (in1 == (q15_t) 0x8000) ? (q15_t) 0x7fff : -in1;

#endif /* #if defined (RISCV_MATH_DSP) */

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
    *pDst++ = *pSrc++;
    in1 = *pSrc++;
#if defined (RISCV_MATH_DSP)
    *pDst++ = __SSAT(-in1, 16);
#else
    *pDst++ = (in1 == (q15_t) 0x8000) ? (q15_t) 0x7fff : -in1;
#endif

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of cmplx_conj group
 */
