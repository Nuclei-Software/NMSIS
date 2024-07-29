/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mult_cmplx_q15.c
 * Description:  Q15 complex-by-complex multiplication
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
  @brief         Q15 complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector
  @param[in]     pSrcB       points to second input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none

  @par           Scaling and Overflow Behavior
                   The function implements 1.15 by 1.15 multiplications and finally output is converted into 3.13 format.
 */

void riscv_cmplx_mult_cmplx_q15(
  const q15_t * pSrcA,
  const q15_t * pSrcB,
        q15_t * pDst,
        uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
  size_t blkCnt = numSamples;                 /* Loop counter */
  size_t l;
  vint16m4x2_t v_tupleA, v_tupleB;
  vint16m4_t v_R1, v_R2, v_I1, v_I2;
  vint32m8_t v_RR, v_II, v_RI, v_IR;

  for(; (l = __riscv_vsetvl_e16m4(blkCnt)) > 0; blkCnt -= l)
  {
    v_tupleA = __riscv_vlseg2e16_v_i16m4x2 (pSrcA, l);
    v_R1 = __riscv_vget_v_i16m4x2_i16m4(v_tupleA, 0);
    v_I1 = __riscv_vget_v_i16m4x2_i16m4(v_tupleA, 1);
    v_tupleB = __riscv_vlseg2e16_v_i16m4x2 (pSrcB, l);
    v_R2 = __riscv_vget_v_i16m4x2_i16m4(v_tupleB, 0);
    v_I2 = __riscv_vget_v_i16m4x2_i16m4(v_tupleB, 1);
    pSrcA += l * 2;
    pSrcB += l * 2;

    v_RR = __riscv_vsra_vx_i32m8(__riscv_vwmul_vv_i32m8(v_R1, v_R2, l), 17, l);
    v_II = __riscv_vsra_vx_i32m8(__riscv_vwmul_vv_i32m8(v_I1, v_I2, l), 17, l);
    v_RI = __riscv_vsra_vx_i32m8(__riscv_vwmul_vv_i32m8(v_R1, v_I2, l), 17, l);
    v_IR = __riscv_vsra_vx_i32m8(__riscv_vwmul_vv_i32m8(v_I1, v_R2, l), 17, l);

    v_R1 = __riscv_vnclip_wx_i16m4(__riscv_vssub_vv_i32m8(v_RR, v_II, l), 0, __RISCV_VXRM_RNU, l);
    v_R2 = __riscv_vnclip_wx_i16m4(__riscv_vsadd_vv_i32m8(v_RI, v_IR, l), 0, __RISCV_VXRM_RNU, l);

    v_tupleA = __riscv_vset_v_i16m4_i16m4x2 (v_tupleA, 0, v_R1);
    v_tupleA = __riscv_vset_v_i16m4_i16m4x2 (v_tupleA, 1, v_R2);
    __riscv_vsseg2e16_v_i16m4x2 (pDst, v_tupleA, l);

    pDst += l * 2;
  }
#else
  unsigned long blkCnt;                          /* Loop counter */
  q15_t a, b, c, d;                              /* Temporary variables */
#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
  q63_t inA, inB;
  q63_t mid_imag, mid_real;
#else
  q31_t inA, inB;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
#endif /* defined (RISCV_MATH_DSP) */

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i] * B[2 * i    ] - A[2 * i + 1] * B[2 * i + 1]. */
    /* C[2 * i + 1] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */
#if defined (RISCV_MATH_DSP)
#if (__RISCV_XLEN == 64)
    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);

    mid_real = __RV_SMDRS(inA, inB);
    mid_real = __RV_SRAI32(mid_real, 17);
    *pDst++ = (q31_t)mid_real;

    mid_imag = __RV_KMXDA(inA, inB);
    mid_imag = __RV_SRAI32(mid_imag, 17);
    *pDst++ = (q31_t)mid_imag;

    *pDst++ = (q31_t)(mid_real >> 32);
    *pDst++ = (q31_t)(mid_imag >> 32);

    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);

    mid_real = __RV_SMDRS(inA, inB);
    mid_real = __RV_SRAI32(mid_real, 17);
    *pDst++ = (q31_t)mid_real;

    mid_imag = __RV_KMXDA(inA, inB);
    mid_imag = __RV_SRAI32(mid_imag, 17);
    *pDst++ = (q31_t)mid_imag;

    *pDst++ = (q31_t)(mid_real >> 32);
    *pDst++ = (q31_t)(mid_imag >> 32);
#else
#if defined (NUCLEI_DSP_N3)
    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);
    mid_real = __RV_DSMDRS(inA, inB);
    mid_real = __RV_DKSLRA32(mid_real, -17);
    *pDst++ = (q31_t)mid_real;

    mid_imag = __RV_DKMXDA(inA, inB);
    mid_imag = __RV_DKSLRA32(mid_imag, -17);
    *pDst++ = (q31_t)mid_imag;

    *pDst++ = (q31_t)(mid_real >> 32);
    *pDst++ = (q31_t)(mid_imag >> 32);

    inA = read_q15x4_ia ((q15_t **) &pSrcA);
    inB = read_q15x4_ia ((q15_t **) &pSrcB);
    mid_real = __RV_DSMDRS(inA, inB);
    mid_real = __RV_DKSLRA32(mid_real, -17);
    *pDst++ = (q31_t)mid_real;

    mid_imag = __RV_DKMXDA(inA, inB);
    mid_imag = __RV_DKSLRA32(mid_imag, -17);
    *pDst++ = (q31_t)mid_imag;

    *pDst++ = (q31_t)(mid_real >> 32);
    *pDst++ = (q31_t)(mid_imag >> 32);
#else
    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    *pDst++ = (q15_t)(__RV_SMDRS(inA, inB) >> 17);
    *pDst++ = (q15_t)(__RV_KMXDA(inA, inB) >> 17);

    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    *pDst++ = (q15_t)(__RV_SMDRS(inA, inB) >> 17);
    *pDst++ = (q15_t)(__RV_KMXDA(inA, inB) >> 17);

    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    *pDst++ = (q15_t)(__RV_SMDRS(inA, inB) >> 17);
    *pDst++ = (q15_t)(__RV_KMXDA(inA, inB) >> 17);

    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    *pDst++ = (q15_t)(__RV_SMDRS(inA, inB) >> 17);
    *pDst++ = (q15_t)(__RV_KMXDA(inA, inB) >> 17);

#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    /* store result in 3.13 format in destination buffer. */
    *pDst++ = (q15_t)((((q31_t)a * c) >> 17) - (((q31_t)b * d) >> 17));
    *pDst++ = (q15_t)((((q31_t)a * d) >> 17) + (((q31_t)b * c) >> 17));

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q15_t)((((q31_t)a * c) >> 17) - (((q31_t)b * d) >> 17));
    *pDst++ = (q15_t)((((q31_t)a * d) >> 17) + (((q31_t)b * c) >> 17));

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q15_t)((((q31_t) a * c)>> 17) - (((q31_t)b * d) >> 17));
    *pDst++ = (q15_t)((((q31_t) a * d)>> 17) + (((q31_t)b * c) >> 17));

    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    *pDst++ = (q15_t) ((((q31_t)a * c) >> 17) - (((q31_t)b * d) >> 17));
    *pDst++ = (q15_t) ((((q31_t)a * d) >> 17) + (((q31_t)b * c) >> 17));

#endif /* defined (RISCV_MATH_DSP) */
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
#if defined (RISCV_MATH_DSP)
    inA = read_q15x2_ia ((q15_t **) &pSrcA);
    inB = read_q15x2_ia ((q15_t **) &pSrcB);
    *pDst++ = (q15_t)(__RV_SMALDRS(0, inA, inB) >> 17);
    *pDst++ = (q15_t)(__SMLALDX(inA, inB, 0) >> 17);
#else
    a = *pSrcA++;
    b = *pSrcA++;
    c = *pSrcB++;
    d = *pSrcB++;

    /* store result in 3.13 format in destination buffer. */
    *pDst++ = (q15_t)((((q31_t)a * c) >> 17) - (((q31_t)b * d) >> 17));
    *pDst++ = (q15_t)((((q31_t)a * d) >> 17) + (((q31_t)b * c) >> 17));
#endif

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of CmplxByCmplxMult group
 */
