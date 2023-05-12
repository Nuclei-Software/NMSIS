/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_abs_q15.c
 * Description:  Q15 vector absolute value
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
  @addtogroup BasicAbs
  @{
 */

/**
  @brief         Q15 vector absolute value.
  @param[in]     pSrc       points to the input vector
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   The Q15 value -1 (0x8000) will be saturated to the maximum allowable positive value 0x7FFF.
 */

void riscv_abs_q15(
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  q15_t in;                                      /* Temporary input variable */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;
  size_t l;
  vint16m8_t v_x, v_zero;
  l = vsetvlmax_e16m8();
  v_zero = vmv_v_x_i16m8(0, l);
  for (; (l = vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_x = vle16_v_i16m8(pSrc, l);
    pSrc += l;
    vbool2_t mask = vmslt_vx_i16m8_b2(v_x, 0, l);
    v_x = vssub_vv_i16m8_m(mask, v_x, v_zero, v_x, l);
    vse16_v_i16m8(pDst, v_x, l);
    pDst += l;
  }

#else
#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7fff) and store result in destination buffer. */
#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    write_q15x4_ia(&pDst, __RV_KABS16(read_q15x4_ia((q15_t **)&pSrc)));
#else
#ifdef NUCLEI_DSP_N1
    write_q15x4_ia(&pDst, __RV_DKABS16(read_q15x4_ia((q15_t **)&pSrc)));
#else
    write_q15x2_ia(&pDst, __RV_KABS16(read_q15x2_ia((q15_t **)&pSrc)));
    write_q15x2_ia(&pDst, __RV_KABS16(read_q15x2_ia((q15_t **)&pSrc)));
#endif  /* NUCLEI_DSP_N1 */
#endif /* __RISCV_XLEN == 64 */
#else
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q15_t)0x8000) ? 0x7fff : -in);
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q15_t)0x8000) ? 0x7fff : -in);
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q15_t)0x8000) ? 0x7fff : -in);
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q15_t)0x8000) ? 0x7fff : -in);
#endif /* RISCV_MATH_DSP */
    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;
#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7fff) and store result in destination buffer. */
    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    //*pDst++ = (in > 0) ? in : (q15_t)__QSUB16(0, in);
    *pDst++ = (q15_t)__RV_KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == (q15_t)0x8000) ? 0x7fff : -in);
#endif

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* RISCV_MATH_VECTOR */
}

/**
  @} end of BasicAbs group
 */
