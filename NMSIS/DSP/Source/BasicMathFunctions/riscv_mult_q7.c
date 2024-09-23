/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_mult_q7.c
 * Description:  Q7 vector multiplication
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicMult
  @{
 */

/**
  @brief         Q7 vector multiplication
  @param[in]     pSrcA      points to the first input vector
  @param[in]     pSrcB      points to the second input vector
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q7 range [0x80 0x7F] are saturated.
 */
RISCV_DSP_ATTRIBUTE void riscv_mult_q7(
  const q7_t * pSrcA,
  const q7_t * pSrcB,
        q7_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vint8m8_t vx, vy;

  for (; (l = __riscv_vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
  {
    vx = __riscv_vle8_v_i8m8(pSrcA, l);
    pSrcA += l;
    vy = __riscv_vle8_v_i8m8(pSrcB, l);
    pSrcB += l;
    __riscv_vse8_v_i8m8(pDst, __riscv_vsmul_vv_i8m8(vx, vy, __RISCV_VXRM_RNU, l), l);
    pDst += l;
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)
#if defined (RISCV_MATH_DSP)
  /* Loop unrolling: Compute 8 outputs at a time */
  blkCnt = blockSize >> 3U;
#else
  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;
#endif /* RISCV_MATH_DSP */

  while (blkCnt > 0U)
  {
    /* C = A * B */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    write_q7x8_ia(&pDst, __RV_KHM8(read_q7x8_ia((q7_t **)&pSrcA), read_q7x8_ia((q7_t **)&pSrcB)));
#else
#ifdef NUCLEI_DSP_N1
    write_q7x8_ia(&pDst, __RV_DKHM8(read_q7x8_ia((q7_t **)&pSrcA), read_q7x8_ia((q7_t **)&pSrcB)));
#else
    write_q7x4_ia(&pDst, __RV_KHM8(read_q7x4_ia((q7_t **)&pSrcA), read_q7x4_ia((q7_t **)&pSrcB)));
    write_q7x4_ia(&pDst, __RV_KHM8(read_q7x4_ia((q7_t **)&pSrcA), read_q7x4_ia((q7_t **)&pSrcB)));
#endif /* NUCLEI_DSP_N1 */
#endif /* __RISCV_XLEN == 64 */
#else
    *pDst++ = (q7_t) __SSAT((((q15_t)(*pSrcA++) * (*pSrcB++)) >> 7), 8);
    *pDst++ = (q7_t) __SSAT((((q15_t)(*pSrcA++) * (*pSrcB++)) >> 7), 8);
    *pDst++ = (q7_t) __SSAT((((q15_t)(*pSrcA++) * (*pSrcB++)) >> 7), 8);
    *pDst++ = (q7_t) __SSAT((((q15_t)(*pSrcA++) * (*pSrcB++)) >> 7), 8);
#endif /* RISCV_MATH_DSP */

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
#if defined (RISCV_MATH_DSP)
  blkCnt = blockSize & 0x7U;
#else
  blkCnt = blockSize & 0x3U;
#endif /* RISCV_MATH_DSP */

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = A * B */

    /* Multiply input and store result in destination buffer. */
    *pDst++ = (q7_t) __SSAT((((q15_t) (*pSrcA++) * (*pSrcB++)) >> 7), 8);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicMult group
 */
