/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_var_f16.c
 * Description:  Variance of the elements of a floating-point vector
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

#include "dsp/statistics_functions_f16.h"


#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupStats
 */


/**
  @addtogroup variance
  @{
 */

/**
  @brief         Variance of the elements of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    variance value returned here
 */

RISCV_DSP_ATTRIBUTE void riscv_var_f16(
  const float16_t * pSrc,
        uint32_t blockSize,
        float16_t * pResult)
{
        _Float16 sum = 0.0f;                          /* Temporary result storage */
        _Float16 fSum = 0.0f;
        _Float16 fMean, fValue;
  const float16_t * pInput = pSrc;

  if (blockSize <= 1U)
  {
    *pResult = 0;
    return;
  }

#if defined(RISCV_MATH_VECTOR)
  size_t blkCnt = blockSize;                   /* Loop counter */
  size_t l;
  vfloat16m8_t v_in;
  vfloat16m8_t v_fValue;

  l = __riscv_vsetvlmax_e16m8();
  vfloat16m8_t vsum = __riscv_vfmv_v_f_f16m8(0.0f, l);
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle16_v_f16m8(pInput, l);
    pInput += l;
    vsum = __riscv_vfadd_vv_f16m8(v_in, vsum, l);
  }
  l = __riscv_vsetvl_e16m8(1);
  vfloat16m1_t temp00m1 = __riscv_vfmv_v_f_f16m1(0.0f, l);
  l = __riscv_vsetvlmax_e16m8();
  temp00m1 = __riscv_vfredusum_vs_f16m8_f16m1(vsum, temp00m1, l);
  sum += __riscv_vfmv_f_s_f16m1_f16(temp00m1);

  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) / blockSize  */
  fMean = (_Float16)sum / (_Float16) blockSize;
  pInput = pSrc;
  blkCnt = blockSize;
  l = __riscv_vsetvlmax_e16m8();
  vsum = __riscv_vfmv_v_f_f16m8(0.0f, l);
  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_in = __riscv_vle16_v_f16m8(pInput, l);
    pInput += l;
    v_fValue = __riscv_vfsub_vf_f16m8(v_in, fMean, l);
     vsum = __riscv_vfmacc_vv_f16m8(vsum, v_fValue, v_fValue, l);
  }
  l = __riscv_vsetvl_e16m8(1);
  temp00m1 = __riscv_vfmv_v_f_f16m1(0.0f, l);
   l = __riscv_vsetvlmax_e16m8();
  temp00m1 = __riscv_vfredusum_vs_f16m8_f16m1(vsum, temp00m1, l);
  fSum += __riscv_vfmv_f_s_f16m1_f16(temp00m1);
#else

  uint32_t blkCnt;                               /* Loop counter */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */

    sum += (_Float16)*pInput++;
    sum += (_Float16)*pInput++;
    sum += (_Float16)*pInput++;
    sum += (_Float16)*pInput++;


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
    /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) */

    sum += (_Float16)*pInput++;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* C = (A[0] + A[1] + A[2] + ... + A[blockSize-1]) / blockSize  */
  fMean = (_Float16)sum / (_Float16) blockSize;

  pInput = pSrc;

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    fValue = (_Float16)*pInput++ - (_Float16)fMean;
    fSum += (_Float16)fValue * (_Float16)fValue;

    fValue = (_Float16)*pInput++ - (_Float16)fMean;
    fSum += (_Float16)fValue * (_Float16)fValue;

    fValue = (_Float16)*pInput++ - (_Float16)fMean;
    fSum += (_Float16)fValue * (_Float16)fValue;

    fValue = (_Float16)*pInput++ - (_Float16)fMean;
    fSum += (_Float16)fValue * (_Float16)fValue;

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
    fValue = (_Float16)*pInput++ - (_Float16)fMean;
    fSum += (_Float16)fValue * (_Float16)fValue;

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* #if defined(RISCV_MATH_VECTOR) */
  /* Variance */
  *pResult = (_Float16)fSum / ((_Float16)blockSize - 1.0f16);
}

/**
  @} end of variance group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

