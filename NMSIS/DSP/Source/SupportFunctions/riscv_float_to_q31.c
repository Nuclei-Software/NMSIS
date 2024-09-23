/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_float_to_q31.c
 * Description:  Converts the elements of the floating-point vector to Q31 vector
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

#include "dsp/support_functions.h"

/**
  @ingroup groupSupport
 */

/**
 * @defgroup float_to_x  Convert 32-bit floating point value
 */

/**
  @addtogroup float_to_x
  @{
 */

/**
  @brief         Converts the elements of the floating-point vector to Q31 vector.
  @param[in]     pSrc       points to the floating-point input vector
  @param[out]    pDst       points to the Q31 output vector
  @param[in]     blockSize  number of samples in each vector

  @par           Details
                   The equation used for the conversion process is:
  <pre>
      pDst[n] = (q31_t)(pSrc[n] * 2147483648);   0 <= n < blockSize.
  </pre>

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q31 range[0x80000000 0x7FFFFFFF] are saturated.

  @note
                   In order to apply rounding, the library should be rebuilt with the ROUNDING macro
                   defined in the preprocessor section of project options.
 
  @note         If the input float values are very big (2**32) then the function
                won't be able to saturate to the right values.
                If you expect very big float values in the input array then you
                should force those values to +1 or -1 before calling this function.
                For reasonable float values (< 2**32), the function will saturate correctly.
 */

RISCV_DSP_ATTRIBUTE void riscv_float_to_q31(
  const float32_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
  const float32_t *pIn = pSrc;                         /* Source pointer */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vfloat32m4_t v_in;
  vint32m4_t v_out;
  for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle32_v_f32m4(pIn, l);
    pIn += l;
#ifdef RISCV_MATH_ROUNDING
    v_out = __riscv_vfcvt_x_f_v_i32m4(__riscv_vfmul_vf_f32m4(v_in, 2147483648.0f, l), l);
#else
    v_out = __riscv_vfcvt_rtz_x_f_v_i32m4(__riscv_vfmul_vf_f32m4(v_in, 2147483648.0f, l), l);
#endif
    __riscv_vse32_v_i32m4 (pDst, v_out, l);
    pDst += l;
  }
#else
#ifdef RISCV_MATH_ROUNDING
        float32_t in;
#endif /* #ifdef RISCV_MATH_ROUNDING */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A * 2147483648 */

    /* convert from float to Q31 and store result in destination buffer */
#ifdef RISCV_MATH_ROUNDING

    in = (*pIn++ * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

    in = (*pIn++ * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

    in = (*pIn++ * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

    in = (*pIn++ * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

#else

    /* C = A * 2147483648 */
    /* Convert from float to Q31 and then store the results in the destination buffer */
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));

#endif /* #ifdef RISCV_MATH_ROUNDING */

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
    /* C = A * 2147483648 */

    /* convert from float to Q31 and store result in destination buffer */
#ifdef RISCV_MATH_ROUNDING

    in = (*pIn++ * 2147483648.0f);
    in += in > 0.0f ? 0.5f : -0.5f;
    *pDst++ = clip_q63_to_q31((q63_t) (in));

#else

    /* C = A * 2147483648 */
    /* Convert from float to Q31 and then store the results in the destination buffer */
    *pDst++ = clip_q63_to_q31((q63_t) (*pIn++ * 2147483648.0f));

#endif /* #ifdef RISCV_MATH_ROUNDING */

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of float_to_x group
 */
