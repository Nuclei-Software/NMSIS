/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_abs_q31.c
 * Description:  Q31 vector absolute value
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
  @brief         Q31 vector absolute value.
  @param[in]     pSrc       points to the input vector
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   The Q31 value -1 (0x80000000) will be saturated to the maximum allowable positive value 0x7FFFFFFF.
 */

RISCV_DSP_ATTRIBUTE void riscv_abs_q31(
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  q31_t in;                                      /* Temporary variable */

#if defined (RISCV_MATH_VECTOR)
  blkCnt = blockSize;
  size_t l;
  vint32m8_t v_x, v_zero;
  l = __riscv_vsetvlmax_e32m8();
  v_zero = __riscv_vmv_v_x_i32m8(0, l);
  for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_x = __riscv_vle32_v_i32m8(pSrc, l);
    pSrc += l;
    vbool4_t mask = __riscv_vmslt_vx_i32m8_b4(v_x, 0, l);
    v_x = __riscv_vssub_vv_i32m8_m(mask, v_zero, v_x, l);
    __riscv_vse32_v_i32m8(pDst, v_x, l);
    pDst += l;
  }

#else
#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = |A| */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    write_q31x2_ia(&pDst, __RV_KABS32(read_q31x2_ia((q31_t **)&pSrc)));
    write_q31x2_ia(&pDst, __RV_KABS32(read_q31x2_ia((q31_t **)&pSrc)));
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    write_q31x2_ia(&pDst, __dkabs32(read_q31x2_ia((q31_t **)&pSrc)));
    write_q31x2_ia(&pDst, __dkabs32(read_q31x2_ia((q31_t **)&pSrc)));
#else
    /* Calculate absolute of input (if -1 then saturated to 0x7fffffff) and store result in destination buffer. */
    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    *pDst++ = __KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    *pDst++ = __KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    *pDst++ = __KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    *pDst++ = __KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif /* RISCV_MATH_DSP */

    /* Decrement loop counter */
#endif /* RISCV_MATH_DSP && NUCLEI_DSP_N2 */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize & 0x3U;

#else /* RISCV_MATH_LOOPUNROLL */

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* RISCV_MATH_LOOPUNROLL */

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7fffffff) and store result in destination buffer. */
    in = *pSrc++;
#if defined (RISCV_MATH_DSP)
    //*pDst++ = (in > 0) ? in : (q31_t)__QSUB(0, in);
    *pDst++ = __KABSW(in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif /* RISCV_MATH_DSP */

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* RISCV_MATH_VECTOR */
}
/**
  @} end of BasicAbs group
 */
