/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_scale_f16.c
 * Description:  Multiplies a floating-point vector by a scalar
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

#include "dsp/basic_math_functions_f16.h"

/**
  @ingroup groupMath
 */



/**
  @addtogroup BasicScale
  @{
 */

/**
  @brief         Multiplies a floating-point vector by a scalar.
  @param[in]     pSrc       points to the input vector
  @param[in]     scale      scale factor to be applied
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

#if defined(RISCV_FLOAT16_SUPPORTED)
void riscv_scale_f16(
  const float16_t *pSrc,
        float16_t scale,
        float16_t *pDst,
        uint32_t blockSize)
{
  uint32_t blkCnt;                               /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vfloat16m8_t vx;

  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l) {
    vx = __riscv_vle16_v_f16m8(pSrc, l);
    pSrc += l;
    __riscv_vse16_v_f16m8(pDst, __riscv_vfmul_vf_f16m8(vx, scale, l), l);
    pDst += l;
  }
#else
#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = A * scale */

    /* Scale input and store result in destination buffer. */
    *pDst++ = (_Float16)(*pSrc++) * (_Float16)scale;

    *pDst++ = (_Float16)(*pSrc++) * (_Float16)scale;

    *pDst++ = (_Float16)(*pSrc++) * (_Float16)scale;

    *pDst++ = (_Float16)(*pSrc++) * (_Float16)scale;

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
    /* C = A * scale */

    /* Scale input and store result in destination buffer. */
    *pDst++ = (_Float16)(*pSrc++) * (_Float16)scale;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /*defined(RISCV_MATH_VECTOR)*/
}
#endif

/**
  @} end of BasicScale group
 */
