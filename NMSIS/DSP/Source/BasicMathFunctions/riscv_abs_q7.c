/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_abs_q7.c
 * Description:  Q7 vector absolute value
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
  @brief         Q7 vector absolute value.
  @param[in]     pSrc       points to the input vector
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none

  @par           Conditions for optimum performance
                   Input and output buffers should be aligned by 32-bit
  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   The Q7 value -1 (0x80) will be saturated to the maximum allowable positive value 0x7F.
 */

RISCV_DSP_ATTRIBUTE void riscv_abs_q7(
  const q7_t * pSrc,
        q7_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  q7_t in;                                       /* Temporary input variable */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;
  size_t l;
  vint8m8_t v_x, v_zero;
  l = __riscv_vsetvlmax_e8m8();
  v_zero = __riscv_vmv_v_x_i8m8(0, l);
  for (; (l = __riscv_vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l)
  {
      v_x = __riscv_vle8_v_i8m8(pSrc, l);
      pSrc += l;
      vbool1_t mask = __riscv_vmslt_vx_i8m8_b1(v_x, 0, l);
      v_x = __riscv_vssub_vv_i8m8_m(mask, v_zero, v_x, l);
      __riscv_vse8_v_i8m8(pDst, v_x, l);
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
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7f) and store result in destination buffer. */
#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    write_q7x8_ia(&pDst, __RV_KABS8(read_q7x8_ia((q7_t **) &pSrc)));
#else
#if defined (NUCLEI_DSP_N1)
    write_q7x8_ia(&pDst, __RV_DKABS8(read_q7x8_ia((q7_t **) &pSrc)));
#else
    write_q7x4_ia(&pDst, __RV_KABS8(read_q7x4_ia((q7_t **) &pSrc)));
    write_q7x4_ia(&pDst, __RV_KABS8(read_q7x4_ia((q7_t **) &pSrc)));
#endif /* NUCLEI_DSP_N1 */
#endif /* __RISCV_XLEN == 64 */
#else
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q7_t) 0x80) ? (q7_t) 0x7f : -in);
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q7_t) 0x80) ? (q7_t) 0x7f : -in);
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q7_t) 0x80) ? (q7_t) 0x7f : -in);
    in = *pSrc++;
    *pDst++ = (in > 0) ? in : ((in == (q7_t) 0x80) ? (q7_t) 0x7f : -in);
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


#else /* RISCV_MATH_LOOPUNROLL */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* RISCV_MATH_LOOPUNROLL */

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7f) and store result in destination buffer. */
    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    *pDst++ = (q7_t)__RV_KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == (q7_t) 0x80) ? (q7_t) 0x7f : -in);
#endif /* RISCV_MATH_DSP */

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* #if defined (RISCV_MATH_VECTOR) */
}

/**
  @} end of BasicAbs group
 */
