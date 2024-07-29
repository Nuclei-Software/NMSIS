/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mult_cmplx_q31.c
 * Description:  Q31 complex-by-complex multiplication
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

#include "dsp/complex_math_functions.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @addtogroup CmplxByCmplxMult
  @{
 */

/**
  @brief         Q31 complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector
  @param[in]     pSrcB       points to second input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function implements 1.31 by 1.31 multiplications and finally output is converted into 3.29 format.
                   Input down scaling is not required.
 */

void riscv_cmplx_mult_cmplx_q31(
  const q31_t * pSrcA,
  const q31_t * pSrcB,
        q31_t * pDst,
        uint32_t numSamples)
{
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  size_t blkCnt = numSamples;                               /* Loop counter */
  size_t l;
  vint32m4x2_t v_tupleA, v_tupleB;
  vint32m4_t v_R1, v_R2, v_I1, v_I2;
  vint64m8_t v_RR, v_II, v_RI, v_IR;
  for (; (l = __riscv_vsetvl_e32m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_tupleA = __riscv_vlseg2e32_v_i32m4x2 (pSrcA, l);
    v_R1 = __riscv_vget_v_i32m4x2_i32m4(v_tupleA, 0);
    v_I1 = __riscv_vget_v_i32m4x2_i32m4(v_tupleA, 1);
    v_tupleB = __riscv_vlseg2e32_v_i32m4x2 (pSrcB, l);
    v_R2 = __riscv_vget_v_i32m4x2_i32m4(v_tupleB, 0);
    v_I2 = __riscv_vget_v_i32m4x2_i32m4(v_tupleB, 1);

    pSrcA += l * 2;
    pSrcB += l * 2;

    v_RR = __riscv_vsra_vx_i64m8(__riscv_vwmul_vv_i64m8(v_R1, v_R2, l), 33, l);
    v_II = __riscv_vsra_vx_i64m8(__riscv_vwmul_vv_i64m8(v_I1, v_I2, l), 33, l);
    v_RI = __riscv_vsra_vx_i64m8(__riscv_vwmul_vv_i64m8(v_R1, v_I2, l), 33, l);
    v_IR = __riscv_vsra_vx_i64m8(__riscv_vwmul_vv_i64m8(v_I1, v_R2, l), 33, l);

    v_R1 = __riscv_vnclip_wx_i32m4(__riscv_vssub_vv_i64m8(v_RR, v_II, l), 0, __RISCV_VXRM_RNU, l);
    v_R2 = __riscv_vnclip_wx_i32m4(__riscv_vsadd_vv_i64m8(v_RI, v_IR, l), 0, __RISCV_VXRM_RNU, l);

    v_tupleA = __riscv_vset_v_i32m4_i32m4x2 (v_tupleA, 0, v_R1);
    v_tupleA = __riscv_vset_v_i32m4_i32m4x2 (v_tupleA, 1, v_R2);
    __riscv_vsseg2e32_v_i32m4x2 (pDst, v_tupleA, l);
    pDst += l * 2;
  }
#else

  unsigned long blkCnt;                   /* Loop counter */
  q31_t a, b, c, d;                       /* Temporary variables */

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N2) || (__RISCV_XLEN == 64))
  q63_t inA, inB, calc_real, calc_imag, out64;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)) */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
    /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */

#if defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    inA = read_q31x2_ia ((q31_t **) &pSrcA);
    inB = read_q31x2_ia ((q31_t **) &pSrcB);
    calc_real = __RV_SMDRS32(inA, inB);
    *pDst++ = (q31_t)(calc_real >> 33);
    calc_imag = __RV_KMXDA32(inA, inB);
    *pDst++ = (q31_t)(calc_imag >> 33);
#else
#if defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    inA = read_q31x2_ia ((q31_t **)&pSrcA);
    inB = read_q31x2_ia ((q31_t **)&pSrcB);
    calc_real = __RV_DMSR33(inA, inB);
    *pDst++ = (q31_t)calc_real - (q31_t)(calc_real >> 32);
    calc_imag = __RV_DMXSR33(inA, inB);
    *pDst++ = (q31_t)calc_imag + (q31_t)(calc_imag >> 32);
#else
    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    /* store result in 3.29 format in destination buffer. */
    *pDst++ = (q31_t)((((q63_t)a * c) >> 33) - (((q63_t)b * d) >> 33));
    *pDst++ = (q31_t)((((q63_t)a * d) >> 33) + (((q63_t)b * c) >> 33));
#endif /* defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#if defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    inA = read_q31x2_ia ((q31_t **) &pSrcA);
    inB = read_q31x2_ia ((q31_t **) &pSrcB);
    calc_real = __RV_SMDRS32(inA, inB);
    *pDst++ = (q31_t)(calc_real >> 33);
    calc_imag = __RV_KMXDA32(inA, inB);
    *pDst++ = (q31_t)(calc_imag >> 33);
#else
#if defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    inA = read_q31x2_ia ((q31_t **)&pSrcA);
    inB = read_q31x2_ia ((q31_t **)&pSrcB);
    calc_real = __RV_DMSR33(inA, inB);
    *pDst++ = (q31_t)calc_real - (q31_t)(calc_real >> 32);
    calc_imag = __RV_DMXSR33(inA, inB);
    *pDst++ = (q31_t)calc_imag + (q31_t)(calc_imag >> 32);
#else
    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q31_t)((((q63_t) a * c) >> 33) - (((q63_t) b * d) >> 33));
    *pDst++ = (q31_t)((((q63_t) a * d) >> 33) + (((q63_t) b * c) >> 33));
#endif /* defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#if defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    inA = read_q31x2_ia ((q31_t **) &pSrcA);
    inB = read_q31x2_ia ((q31_t **) &pSrcB);
    calc_real = __RV_SMDRS32(inA, inB);
    *pDst++ = (q31_t)(calc_real >> 33);
    calc_imag = __RV_KMXDA32(inA, inB);
    *pDst++ = (q31_t)(calc_imag >> 33);
#else
#if defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    inA = read_q31x2_ia ((q31_t **)&pSrcA);
    inB = read_q31x2_ia ((q31_t **)&pSrcB);
    calc_real = __RV_DMSR33(inA, inB);
    *pDst++ = (q31_t)calc_real - (q31_t)(calc_real >> 32);
    calc_imag = __RV_DMXSR33(inA, inB);
    *pDst++ = (q31_t)calc_imag + (q31_t)(calc_imag >> 32);
#else
    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q31_t)((((q63_t) a * c) >> 33) - (((q63_t) b * d) >> 33));
    *pDst++ = (q31_t)((((q63_t) a * d) >> 33) + (((q63_t) b * c) >> 33));
#endif /* defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#if defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    inA = read_q31x2_ia ((q31_t **) &pSrcA);
    inB = read_q31x2_ia ((q31_t **) &pSrcB);
    calc_real = __RV_SMDRS32(inA, inB);
    *pDst++ = (q31_t)(calc_real >> 33);
    calc_imag = __RV_KMXDA32(inA, inB);
    *pDst++ = (q31_t)(calc_imag >> 33);
#else
#if defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    inA = read_q31x2_ia ((q31_t **)&pSrcA);
    inB = read_q31x2_ia ((q31_t **)&pSrcB);
    calc_real = __RV_DMSR33(inA, inB);
    *pDst++ = (q31_t)calc_real - (q31_t)(calc_real >> 32);
    calc_imag = __RV_DMXSR33(inA, inB);
    *pDst++ = (q31_t)calc_imag + (q31_t)(calc_imag >> 32);
#else
    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q31_t)((((q63_t) a * c) >> 33) - (((q63_t) b * d) >> 33));
    *pDst++ = (q31_t)((((q63_t) a * d) >> 33) + (((q63_t) b * c) >> 33));
#endif /* defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

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

    /* store result in 3.29 format in destination buffer. */
#if defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    inA = read_q31x2_ia ((q31_t **) &pSrcA);
    inB = read_q31x2_ia ((q31_t **) &pSrcB);
    calc_real = __RV_SMDRS32(inA, inB);
    *pDst++ = (q31_t)(calc_real >> 33);
    calc_imag = __RV_KMXDA32(inA, inB);
    *pDst++ = (q31_t)(calc_imag >> 33);
#else
#if defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    inA = read_q31x2_ia ((q31_t **) &pSrcA);
    inB = read_q31x2_ia ((q31_t **) &pSrcB);
    calc_real = __RV_DMSR33(inA, inB);
    *pDst++ = (q31_t)calc_real - (q31_t)(calc_real >> 32);
    calc_imag = __RV_DMXSR33(inA, inB);
    *pDst++ = (q31_t)calc_imag + (q31_t)(calc_imag >> 32);
#else
    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q31_t)((((q63_t) a * c) >> 33) - (((q63_t) b * d) >> 33));
    *pDst++ = (q31_t)((((q63_t) a * d) >> 33) + (((q63_t) b * c) >> 33));
#endif /* defined(RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined(RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
}

/**
  @} end of CmplxByCmplxMult group
 */
