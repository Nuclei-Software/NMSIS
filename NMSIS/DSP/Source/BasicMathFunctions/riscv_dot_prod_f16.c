/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_dot_prod_f16.c
 * Description:  Floating-point dot product
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
  @addtogroup BasicDotProd
  @{
 */

/**
  @brief         Dot product of floating-point vectors.
  @param[in]     pSrcA      points to the first input vector.
  @param[in]     pSrcB      points to the second input vector.
  @param[in]     blockSize  number of samples in each vector.
  @param[out]    result     output result returned here.
 */

#if defined(RISCV_FLOAT16_SUPPORTED)
RISCV_DSP_ATTRIBUTE void riscv_dot_prod_f16(
  const float16_t * pSrcA,
  const float16_t * pSrcB,
        uint32_t blockSize,
        float16_t * result)
{
        uint32_t blkCnt;                               /* Loop counter */
        _Float16 sum = 0.0f;                          /* Temporary return variable */

#if defined(RISCV_MATH_VECTOR)
  blkCnt = blockSize;                               /* Loop counter */
  size_t l;
  vfloat16m8_t v_A, v_B;
  l = __riscv_vsetvl_e16m1(1);
  vfloat16m1_t v_sum = __riscv_vfmv_v_f_f16m1(0.0f, l);

  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_A = __riscv_vle16_v_f16m8(pSrcA, l);
    pSrcA += l;
    v_B = __riscv_vle16_v_f16m8(pSrcB, l);
    pSrcB += l;
    v_sum = __riscv_vfredusum_vs_f16m8_f16m1(__riscv_vfmul_vv_f16m8(v_A, v_B, l), v_sum, l);
  }
  sum = __riscv_vfmv_f_s_f16m1_f16(v_sum);
#else
#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  /* First part of the processing with loop unrolling. Compute 4 outputs at a time.
   ** a second loop below computes the remaining 1 to 3 samples. */
  while (blkCnt > 0U)
  {
    /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */

    /* Calculate dot product and store result in a temporary buffer. */
    sum += (_Float16)(*pSrcA++) * (_Float16)(*pSrcB++);

    sum += (_Float16)(*pSrcA++) * (_Float16)(*pSrcB++);

    sum += (_Float16)(*pSrcA++) * (_Float16)(*pSrcB++);

    sum += (_Float16)(*pSrcA++) * (_Float16)(*pSrcB++);

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
    /* C = A[0]* B[0] + A[1]* B[1] + A[2]* B[2] + .....+ A[blockSize-1]* B[blockSize-1] */

    /* Calculate dot product and store result in a temporary buffer. */
    sum += (_Float16)(*pSrcA++) * (_Float16)(*pSrcB++);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
  /* Store result in destination buffer */
  *result = sum;
}
#endif
/**
  @} end of BasicDotProd group
 */
