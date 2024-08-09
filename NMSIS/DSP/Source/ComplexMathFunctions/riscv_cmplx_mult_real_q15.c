/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cmplx_mult_real_q15.c
 * Description:  Q15 complex by real multiplication
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
  @addtogroup CmplxByRealMult
  @{
 */

/**
  @brief         Q15 complex-by-real multiplication.
  @param[in]     pSrcCmplx   points to complex input vector
  @param[in]     pSrcReal    points to real input vector
  @param[out]    pCmplxDst   points to complex output vector
  @param[in]     numSamples  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   Results outside of the allowable Q15 range [0x8000 0x7FFF] are saturated.
 */
RISCV_DSP_ATTRIBUTE void riscv_cmplx_mult_real_q15(
  const q15_t * pSrcCmplx,
  const q15_t * pSrcReal,
        q15_t * pCmplxDst,
        uint32_t numSamples)
{
#if defined(RISCV_MATH_VECTOR)
  uint32_t blkCnt = numSamples;                 /* Loop counter */
  size_t l;
  ptrdiff_t bstride = 4;
  vint16m8_t v_Rc, v_Ic, v_Rr;
  vint16m8_t vR2_m8, vI2_m8;

  for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
  {
    v_Rc = __riscv_vlse16_v_i16m8(pSrcCmplx, bstride, l);
    v_Ic = __riscv_vlse16_v_i16m8(pSrcCmplx + 1, bstride, l);
    pSrcCmplx += l * 2;
    v_Rr = __riscv_vle16_v_i16m8(pSrcReal, l);
    pSrcReal += l;

    vR2_m8 = __riscv_vsmul_vv_i16m8(v_Rc, v_Rr, __RISCV_VXRM_RNU, l);
    vI2_m8 = __riscv_vsmul_vv_i16m8(v_Ic, v_Rr, __RISCV_VXRM_RNU, l);

    __riscv_vsse16_v_i16m8(pCmplxDst, bstride, vR2_m8, l);
    __riscv_vsse16_v_i16m8(pCmplxDst + 1, bstride, vI2_m8, l);
    pCmplxDst += l * 2;
  }
#else

  unsigned long blkCnt;                       /* Loop counter */
  q15_t in;                              /* Temporary variable */

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP)
#if defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64)
  q63_t inA;                   /* Temporary variables to hold input data */
  q63_t inB;                   /* Temporary variables to hold input data */
  q15_t temp1, temp2;
  q15_t out1, out2, out3, out4; /* Temporary variables to hold output data */
  q63_t out64; /* Temporary variables to hold intermediate data */
#else
  q31_t inA1, inA2;             /* Temporary variables to hold input data */
  q31_t inB1;                   /* Temporary variables to hold input data */
  q15_t out1, out2, out3, out4; /* Temporary variables to hold output data */
  q31_t mul1, mul2, mul3, mul4; /* Temporary variables to hold intermediate data */
#endif /* defined (NUCLEI_DSP_N2) || (__RISCV_XLEN == 64) */
#endif /* defined (RISCV_MATH_DSP) */

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = numSamples >> 2U;

  while (blkCnt > 0U)
  {
    /* C[2 * i    ] = A[2 * i    ] * B[i]. */
    /* C[2 * i + 1] = A[2 * i + 1] * B[i]. */

#if defined (RISCV_MATH_DSP)
#if __RISCV_XLEN == 64
    /* read 2 complex numbers both real and imaginary from complex input buffer */
    inA = read_q15x4_ia ((q15_t **) &pSrcCmplx);
    /* read 2 real values at a time from real input buffer */
    temp1 = *pSrcReal++;
    temp2 = *pSrcReal++;
    inB = __RV_PKBB32(__RV_PKBB16(temp2, temp2), __RV_PKBB16(temp1, temp1));

    out64 = __RV_KHM16(inA, inB); // 1,2, 3, 4
    write_q15x4_ia (&pCmplxDst, out64);

    inA = read_q15x4_ia ((q15_t **) &pSrcCmplx);
    /* read 2 real values at a time from real input buffer */
    temp1 = *pSrcReal++;
    temp2 = *pSrcReal++;
    inB = __RV_PKBB32(__RV_PKBB16(temp2, temp2), __RV_PKBB16(temp1, temp1));

    out64 = __RV_KHM16(inA, inB); // 1,2, 3, 4
    write_q15x4_ia (&pCmplxDst, out64);

#else
#if defined(NUCLEI_DSP_N2)
    /* read 2 complex numbers both real and imaginary from complex input buffer */
    inA = read_q15x4_ia ((q15_t **) &pSrcCmplx);
    /* read 2 real values at a time from real input buffer */
    temp1 = *pSrcReal++;
    temp2 = *pSrcReal++;
    inB = __RV_DPACK32(__RV_DPKBB16(temp2, temp2), __RV_DPKBB16(temp1, temp1));

    out64 = __RV_DKHM16(inA, inB); // 1,2, 3, 4
    write_q15x4_ia (&pCmplxDst, out64);

    inA = read_q15x4_ia ((q15_t **) &pSrcCmplx);
    /* read 2 real values at a time from real input buffer */
    temp1 = *pSrcReal++;
    temp2 = *pSrcReal++;
    inB = __RV_DPACK32(__RV_DPKBB16(temp2, temp2), __RV_DPKBB16(temp1, temp1));

    out64 = __RV_DKHM16(inA, inB); // 1,2, 3, 4
    write_q15x4_ia (&pCmplxDst, out64);
#else
    /* read 2 complex numbers both real and imaginary from complex input buffer */
    inA1 = read_q15x2_ia ((q15_t **) &pSrcCmplx);
    inA2 = read_q15x2_ia ((q15_t **) &pSrcCmplx);
    /* read 2 real values at a time from real input buffer */
    inB1 = read_q15x2_ia ((q15_t **) &pSrcReal);

    /* multiply complex number with real numbers */
    mul1 = __SMBB16(inA1, inB1);
    mul2 = __SMBT16(inB1, inA1);
    mul3 = __SMBT16(inA2, inB1);
    mul4 = __SMTT16(inA2, inB1);

    /* saturate the result */
    out1 = (q15_t)__SSAT(mul1 >> 15U, 16);
    out2 = (q15_t)__SSAT(mul2 >> 15U, 16);
    out3 = (q15_t)__SSAT(mul3 >> 15U, 16);
    out4 = (q15_t)__SSAT(mul4 >> 15U, 16);
    /* pack real and imaginary outputs and store them to destination */
    write_q15x2_ia (&pCmplxDst, __PKHBT(out1, out2, 16));
    write_q15x2_ia (&pCmplxDst, __PKHBT(out3, out4, 16));

    inA1 = read_q15x2_ia ((q15_t **) &pSrcCmplx);
    inA2 = read_q15x2_ia ((q15_t **) &pSrcCmplx);
    inB1 = read_q15x2_ia ((q15_t **) &pSrcReal);

    mul1 = __SMBB16(inA1, inB1);
    mul2 = __SMBT16(inB1, inA1);
    mul3 = __SMBT16(inA2, inB1);
    mul4 = __SMTT16(inA2, inB1);

    out1 = (q15_t)__SSAT(mul1 >> 15U, 16);
    out2 = (q15_t)__SSAT(mul2 >> 15U, 16);
    out3 = (q15_t)__SSAT(mul3 >> 15U, 16);
    out4 = (q15_t)__SSAT(mul4 >> 15U, 16);
    write_q15x2_ia (&pCmplxDst, __PKHBT(out1, out2, 16));
    write_q15x2_ia (&pCmplxDst, __PKHBT(out3, out4, 16));
#endif /* defined(NUCLEI_DSP_N2) */
#endif /* __RISCV_XLEN == 64 */
#else
    in = *pSrcReal++;
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);

    in = *pSrcReal++;
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);

    in = *pSrcReal++;
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);

    in = *pSrcReal++;
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);
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
    /* C[2 * i    ] = A[2 * i    ] * B[i]. */
    /* C[2 * i + 1] = A[2 * i + 1] * B[i]. */

    in = *pSrcReal++;
    /* store the result in the destination buffer. */
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);
    *pCmplxDst++ = (q15_t)__SSAT((((q31_t)*pSrcCmplx++ * in) >> 15), 16);

    /* Decrement loop counter */
    blkCnt--;
  }
#endif /* defined(RISCV_MATH_VECTOR) */
}

/**
  @} end of CmplxByRealMult group
 */
