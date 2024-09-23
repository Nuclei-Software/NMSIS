/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_q7_to_q31.c
 * Description:  Converts the elements of the Q7 vector to Q31 vector
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
  @addtogroup q7_to_x
  @{
 */

/**
  @brief         Converts the elements of the Q7 vector to Q31 vector.
  @param[in]     pSrc       points to the Q7 input vector
  @param[out]    pDst       points to the Q31 output vector
  @param[in]     blockSize  number of samples in each vector

  @par           Details
                   The equation used for the conversion process is:
  <pre>
      pDst[n] = (q31_t) pSrc[n] << 24;   0 <= n < blockSize.
  </pre>
 */
RISCV_DSP_ATTRIBUTE void riscv_q7_to_q31(
  const q7_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */
  const q7_t *pIn = pSrc;                              /* Source pointer */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                         /* Loop counter */
  size_t l;
  vint8m2_t v_in;
  vint32m8_t v_out;
  for (; (l = __riscv_vsetvl_e8m2(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle8_v_i8m2(pIn, l);
    pIn += l;
    v_out = __riscv_vsll_vx_i32m8(__riscv_vsext_vf4_i32m8(v_in, l), 24U, l);
    __riscv_vse32_v_i32m8 (pDst, v_out, l);
    pDst += l;
  }
#else

#if defined (RISCV_MATH_LOOPUNROLL)

  q31_t in;

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = (q31_t) A << 24 */

    /* Convert from q7 to q31 and store result in destination buffer */
    in = read_q7x4_ia ((q7_t **) &pIn);
    *pDst++ = (__ROR(in, 8)) & 0xFF000000;
    *pDst++ = (__ROR(in, 16)) & 0xFF000000;
    *pDst++ = (__ROR(in, 24)) & 0xFF000000;
    *pDst++ = (in & 0xFF000000);
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
    /* C = (q31_t) A << 24 */

    /* Convert from q7 to q31 and store result in destination buffer */
    *pDst++ = (q31_t) * pIn++ << 24;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of q7_to_x group
 */
