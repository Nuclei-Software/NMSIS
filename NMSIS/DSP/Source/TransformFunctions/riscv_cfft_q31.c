/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_q31.c
 * Description:  Combined Radix Decimation in Frequency CFFT fixed point processing function
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

#include "dsp/transform_functions.h"




extern void riscv_radix4_butterfly_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint32_t twidCoefModifier);

extern void riscv_radix4_butterfly_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint32_t twidCoefModifier);

extern void riscv_bitreversal_32(
        uint32_t * pSrc,
  const uint16_t bitRevLen,
  const uint16_t * pBitRevTable);

void riscv_cfft_radix4by2_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef);

void riscv_cfft_radix4by2_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef);


/**
  @addtogroup ComplexFFTQ31
  @{
 */

/**
  @brief         Processing function for the Q31 complex FFT.
  @param[in]     S               points to an instance of the fixed-point CFFT structure
  @param[in,out] p1              points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        none
 */
void riscv_cfft_q31(
  const riscv_cfft_instance_q31 * S,
        q31_t * p1,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag)
{
  uint32_t L = S->fftLen;

  if (ifftFlag == 1U)
  {
     switch (L)
     {
     case 16:
     case 64:
     case 256:
     case 1024:
     case 4096:
       riscv_radix4_butterfly_inverse_q31 ( p1, L, (q31_t*)S->pTwiddle, 1 );
       break;

     case 32:
     case 128:
     case 512:
     case 2048:
       riscv_cfft_radix4by2_inverse_q31 ( p1, L, S->pTwiddle );
       break;
     }
  }
  else
  {
     switch (L)
     {
     case 16:
     case 64:
     case 256:
     case 1024:
     case 4096:
       riscv_radix4_butterfly_q31 ( p1, L, (q31_t*)S->pTwiddle, 1 );
       break;

     case 32:
     case 128:
     case 512:
     case 2048:
       riscv_cfft_radix4by2_q31 ( p1, L, S->pTwiddle );
       break;
     }
  }

  if ( bitReverseFlag )
    riscv_bitreversal_32 ((uint32_t*) p1, S->bitRevLength, S->pBitRevTable);
}

/**
  @} end of ComplexFFTQ31 group
 */

void riscv_cfft_radix4by2_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef)
{
        uint32_t i, l;
        uint32_t n2;
        q31_t xt, yt, cosVal, sinVal;
        q31_t p0, p1;

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
  const q31_t *pC = pCoef;
        q31_t *pSi = pSrc;
        q31_t *pSl = pSrc + fftLen;
        q63_t T, S, R;
        q63_t coeff, out1, out2;
        q63_t temp0, temp1;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
  n2 = fftLen >> 1U;

  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_RADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_RADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_SUB32(T, S);

    write_q31x2_ia (&pSi, __RV_RADD32(T, S));

    out1 = __RV_KMDA32(coeff, R);
    out2 = __RV_SMXDS32(R, coeff);

    write_q31x2_ia (&pSl, __RV_PKTT32(out2, out1));
#else
#if defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_DRADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_DRADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_DSUB32(T, S);

    write_q31x2_ia (&pSi, __RV_DRADD32(T, S));

    out1 = __RV_DKMDA32(0, coeff, R);
    out2 = __RV_DSMXDS32(0, R, coeff);

    write_q31x2_ia (&pSl, __RV_DPKTT32(out2, out1));
#else
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt = (pSrc[2 * i] >> 2U) - (pSrc[2 * l] >> 2U);
     pSrc[2 * i] = (pSrc[2 * i] >> 2U) + (pSrc[2 * l] >> 2U);

     yt = (pSrc[2 * i + 1] >> 2U) - (pSrc[2 * l + 1] >> 2U);
     pSrc[2 * i + 1] = (pSrc[2 * l + 1] >> 2U) + (pSrc[2 * i + 1] >> 2U);

     mult_32x32_keep32_R(p0, xt, cosVal);
     mult_32x32_keep32_R(p1, yt, cosVal);
     multAcc_32x32_keep32_R(p0, yt, sinVal);
     multSub_32x32_keep32_R(p1, xt, sinVal);

     pSrc[2 * l] = p0 << 1;
     pSrc[2 * l + 1] = p1 << 1;
#endif /* defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }


  /* first col */
  riscv_radix4_butterfly_q31 (pSrc, n2, (q31_t*)pCoef, 2U);

  /* second col */
  riscv_radix4_butterfly_q31 (pSrc + fftLen, n2, (q31_t*)pCoef, 2U);

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
     temp0 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
#if defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3)
     temp0 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
     p0 = pSrc[4 * i + 0];
     p1 = pSrc[4 * i + 1];
     xt = pSrc[4 * i + 2];
     yt = pSrc[4 * i + 3];

     p0 <<= 1U;
     p1 <<= 1U;
     xt <<= 1U;
     yt <<= 1U;

     pSrc[4 * i + 0] = p0;
     pSrc[4 * i + 1] = p1;
     pSrc[4 * i + 2] = xt;
     pSrc[4 * i + 3] = yt;
#endif /* defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }

}

void riscv_cfft_radix4by2_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef)
{
  uint32_t i, l;
  uint32_t n2;
  q31_t xt, yt, cosVal, sinVal;
  q31_t p0, p1;

#if defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
  const q31_t *pC = pCoef;
        q31_t *pSi = pSrc;
        q31_t *pSl = pSrc + fftLen;
        q63_t T, S, R;
        q63_t coeff, out1, out2;
        q63_t temp0, temp1;
#endif /* defined (RISCV_MATH_DSP) && (defined(NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_RADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_RADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_SUB32(T, S);

    write_q31x2_ia (&pSi, __RV_RADD32(T, S));

    out1 = __RV_SMDRS32(coeff, R);
    out2 = __RV_KMXDA32(R, coeff);

    write_q31x2_ia (&pSl, __RV_PKTT32(out2, out1));
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
    coeff = read_q31x2_ia ((q31_t **) &pC);

    T = read_q31x2 (pSi);
    T = __RV_DRADD32(T, 0); /* this is just a SIMD arithmetic shift right by 1 */

    S = read_q31x2 (pSl);
    S = __RV_DRADD32(S, 0); /* this is just a SIMD arithmetic shift right by 1 */

    R = __RV_DSUB32(T, S);

    write_q31x2_ia (&pSi, __RV_DRADD32(T, S));

    out1 = __RV_DSMDRS32(0, coeff, R);
    out2 = __RV_DKMXDA32(0, R, coeff);

    write_q31x2_ia (&pSl, __RV_DPKTT32(out2, out1));
#else
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt = (pSrc[2 * i] >> 2U) - (pSrc[2 * l] >> 2U);
     pSrc[2 * i] = (pSrc[2 * i] >> 2U) + (pSrc[2 * l] >> 2U);

     yt = (pSrc[2 * i + 1] >> 2U) - (pSrc[2 * l + 1] >> 2U);
     pSrc[2 * i + 1] = (pSrc[2 * l + 1] >> 2U) + (pSrc[2 * i + 1] >> 2U);

     mult_32x32_keep32_R(p0, xt, cosVal);
     mult_32x32_keep32_R(p1, yt, cosVal);
     multSub_32x32_keep32_R(p0, yt, sinVal);
     multAcc_32x32_keep32_R(p1, xt, sinVal);

     pSrc[2 * l] = p0 << 1U;
     pSrc[2 * l + 1] = p1 << 1U;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }

  /* first col */
  riscv_radix4_butterfly_inverse_q31(pSrc, n2, (q31_t*)pCoef, 2U);

  /* second col */
  riscv_radix4_butterfly_inverse_q31( pSrc + fftLen, n2, (q31_t*)pCoef, 2U);

  n2 = fftLen >> 1U;
  for (i = 0; i < n2; i++)
  {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
     temp0 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_KSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
     temp0 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i), 1);
     temp1 = __RV_DKSLRA32(read_q31x2(pSrc + 4 * i + 2), 1);
     write_q31x2(pSrc + 4 * i, temp0);
     write_q31x2(pSrc + 4 * i + 2, temp1);
#else
     p0 = pSrc[4 * i + 0];
     p1 = pSrc[4 * i + 1];
     xt = pSrc[4 * i + 2];
     yt = pSrc[4 * i + 3];

     p0 <<= 1U;
     p1 <<= 1U;
     xt <<= 1U;
     yt <<= 1U;

     pSrc[4 * i + 0] = p0;
     pSrc[4 * i + 1] = p1;
     pSrc[4 * i + 2] = xt;
     pSrc[4 * i + 3] = yt;
#endif /* defined (RISCV_MATH_DSP) && defined(NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
  }
}
