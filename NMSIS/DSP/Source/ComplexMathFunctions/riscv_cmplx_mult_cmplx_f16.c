/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mult_cmplx_f16.c
 * Description:  Floating-point complex-by-complex multiplication
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

#include "dsp/complex_math_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

/**
  @ingroup groupCmplxMath
 */



/**
  @addtogroup CmplxByCmplxMult
  @{
 */

/**
  @brief         Floating-point complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector
  @param[in]     pSrcB       points to second input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
 */

RISCV_DSP_ATTRIBUTE void riscv_cmplx_mult_cmplx_f16(
  const float16_t * pSrcA,
  const float16_t * pSrcB,
        float16_t * pDst,
        uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
  size_t blkCnt = numSamples;                               /* Loop counter */
  size_t l;
  vfloat16m4x2_t v_tupleA, v_tupleB;
  vfloat16m4_t v_R1, v_R2, v_I1, v_I2;
  vfloat16m4_t v_RR, v_II, v_RI, v_IR;

  for (; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_tupleA = __riscv_vlseg2e16_v_f16m4x2 (pSrcA, l);
    v_R1 = __riscv_vget_v_f16m4x2_f16m4(v_tupleA, 0);
    v_I1 = __riscv_vget_v_f16m4x2_f16m4(v_tupleA, 1);
    v_tupleB = __riscv_vlseg2e16_v_f16m4x2 (pSrcB, l);
    v_R2 = __riscv_vget_v_f16m4x2_f16m4(v_tupleB, 0);
    v_I2 = __riscv_vget_v_f16m4x2_f16m4(v_tupleB, 1);
    pSrcA += l * 2;
    pSrcB += l * 2;

    v_RR = __riscv_vfmul_vv_f16m4(v_R1, v_R2, l);
    v_II = __riscv_vfmul_vv_f16m4(v_I1, v_I2, l);
    v_RI = __riscv_vfmul_vv_f16m4(v_R1, v_I2, l);
    v_IR = __riscv_vfmul_vv_f16m4(v_I1, v_R2, l);

    v_RR = __riscv_vfsub_vv_f16m4(v_RR, v_II, l);
    v_RI = __riscv_vfadd_vv_f16m4(v_RI, v_IR, l);

    v_tupleA = __riscv_vset_v_f16m4_f16m4x2 (v_tupleA, 0, v_RR);
    v_tupleA = __riscv_vset_v_f16m4_f16m4x2 (v_tupleA, 1, v_RI);
    __riscv_vsseg2e16_v_f16m4x2 (pDst, v_tupleA, l);

    pDst += l * 2;
  }
#else
    uint32_t blkCnt;                               /* Loop counter */
    _Float16 a, b, c, d;  /* Temporary variables to store real and imaginary values */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
    /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;
    /* store result in destination buffer. */
    *pDst++ = (a * c) - (b * d);
    *pDst++ = (a * d) + (b * c);

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;
    *pDst++ = (a * c) - (b * d);
    *pDst++ = (a * d) + (b * c);

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;
    *pDst++ = (a * c) - (b * d);
    *pDst++ = (a * d) + (b * c);

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;
    *pDst++ = (a * c) - (b * d);
    *pDst++ = (a * d) + (b * c);

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
    /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
    /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    /* store result in destination buffer. */
    *pDst++ = (a * c) - (b * d);
    *pDst++ = (a * d) + (b * c);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of CmplxByCmplxMult group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
