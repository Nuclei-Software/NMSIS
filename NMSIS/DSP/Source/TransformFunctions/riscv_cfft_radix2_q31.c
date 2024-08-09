/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_radix2_q31.c
 * Description:  Radix-2 Decimation in Frequency CFFT & CIFFT Fixed point processing function
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

RISCV_DSP_ATTRIBUTE void riscv_radix2_butterfly_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint16_t twidCoefModifier);

RISCV_DSP_ATTRIBUTE void riscv_radix2_butterfly_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint16_t twidCoefModifier);

RISCV_DSP_ATTRIBUTE void riscv_bitreversal_q31(
        q31_t * pSrc,
        uint32_t fftLen,
        uint16_t bitRevFactor,
  const uint16_t * pBitRevTab);


/**
  @addtogroup ComplexFFTDeprecated
  @{
 */

/**
  @brief         Processing function for the fixed-point CFFT/CIFFT.
  @deprecated    Do not use this function. It has been superseded by \ref riscv_cfft_q31 and will be removed in the future.
  @param[in]     S    points to an instance of the fixed-point CFFT/CIFFT structure
  @param[in,out] pSrc points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
 */

RISCV_DSP_ATTRIBUTE void riscv_cfft_radix2_q31(
  const riscv_cfft_radix2_instance_q31 * S,
        q31_t * pSrc)
{

   if (S->ifftFlag == 1U)
   {
      riscv_radix2_butterfly_inverse_q31(pSrc, S->fftLen,
      S->pTwiddle, S->twidCoefModifier);
   }
   else
   {
      riscv_radix2_butterfly_q31(pSrc, S->fftLen,
      S->pTwiddle, S->twidCoefModifier);
   }

   riscv_bitreversal_q31(pSrc, S->fftLen, S->bitRevFactor, S->pBitRevTable);
}

/**
  @} end of ComplexFFTDeprecated group
 */

RISCV_DSP_ATTRIBUTE void riscv_radix2_butterfly_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint16_t twidCoefModifier)
{

   unsigned i, j, k, l, m;
   unsigned n1, n2, ia;
   q31_t xt, yt, cosVal, sinVal;
   q31_t p0, p1;

#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
   q63_t coeff, in64_1, in64_2, tmpSub, tmp64_1, tmp64_2;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */

   //N = fftLen;
   n2 = fftLen;

   n1 = n2;
   n2 = n2 >> 1;
   ia = 0;

   // loop for groups
   for (i = 0; i < n2; i++)
   {
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
      coeff = read_q31x2(pCoef + ia * 2);
      ia = ia + twidCoefModifier;
      l = i + n2;
      in64_1 = read_q31x2(pSrc + i * 2);
      in64_2 = read_q31x2(pSrc + l * 2);
#if __RISCV_XLEN == 64
      write_q31x2(pSrc + i * 2, __RV_RADD32(__RV_RADD32(in64_1, in64_2), 0));
      tmpSub= __RV_RSUB32(in64_1, in64_2);
      tmp64_1 = __RV_KMDA32(tmpSub, coeff);
      tmp64_2 = __RV_SMXDS32(tmpSub, coeff);
      write_q31x2(pSrc + l * 2, __RV_PKTT32(tmp64_2, tmp64_1));
#else
#if defined (NUCLEI_DSP_N3)
      write_q31x2(pSrc + i * 2, __RV_DRADD32(__RV_DRADD32(in64_1, in64_2), 0));
      tmpSub = __RV_DRSUB32(in64_1, in64_2);
      tmp64_1 = __RV_DKMDA32(tmpSub, coeff);
      tmp64_2 = __RV_DSMXDS32(tmpSub, coeff);
      write_q31x2(pSrc + l * 2, __RV_DPKTT32(tmp64_2, tmp64_1));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      l = i + n2;
      xt = (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
      pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

      yt = (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
      pSrc[2 * i + 1] =
        ((pSrc[2 * l + 1] >> 1U) + (pSrc[2 * i + 1] >> 1U)) >> 1U;

      mult_32x32_keep32_R(p0, xt, cosVal);
      mult_32x32_keep32_R(p1, yt, cosVal);
      multAcc_32x32_keep32_R(p0, yt, sinVal);
      multSub_32x32_keep32_R(p1, xt, sinVal);

      pSrc[2U * l] = p0;
      pSrc[2U * l + 1U] = p1;

#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
   }                             // groups loop end

   twidCoefModifier <<= 1U;

   // loop for stage
   for (k = fftLen / 2; k > 2; k = k >> 1)
   {
      n1 = n2;
      n2 = n2 >> 1;
      ia = 0;

      // loop for groups
      for (j = 0; j < n2; j++)
      {
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
         coeff = read_q31x2(pCoef + ia * 2);
#else
         cosVal = pCoef[ia * 2];
         sinVal = pCoef[(ia * 2) + 1];
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
         ia = ia + twidCoefModifier;

         // loop for butterfly
         i = j;
         m = fftLen / n1;
         do
         {
            l = i + n2;
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
            in64_1 = read_q31x2(pSrc + i * 2);
            in64_2 = read_q31x2(pSrc + l * 2);
#if __RISCV_XLEN == 64
            write_q31x2(pSrc + i * 2, __RV_RADD32(in64_1, in64_2));
            tmpSub = __RV_SUB32(in64_1, in64_2);
            tmp64_1 = __RV_KMDA32(tmpSub, coeff);
            tmp64_2 = __RV_SMXDS32(tmpSub, coeff);
            write_q31x2(pSrc + l * 2, __RV_PKTT32(tmp64_2, tmp64_1));
#else
#if defined (NUCLEI_DSP_N3)
            write_q31x2(pSrc + i * 2, __RV_DRADD32(in64_1, in64_2));
            tmpSub = __RV_DSUB32(in64_1, in64_2);
            tmp64_1 = __RV_DKMDA32(tmpSub, coeff);
            tmp64_2 = __RV_DSMXDS32(tmpSub, coeff);
            write_q31x2(pSrc + l * 2, __RV_DPKTT32(tmp64_2, tmp64_1));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
            xt = pSrc[2 * i] - pSrc[2 * l];
            pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1U;

            yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
            pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1U;

            mult_32x32_keep32_R(p0, xt, cosVal);
            mult_32x32_keep32_R(p1, yt, cosVal);
            multAcc_32x32_keep32_R(p0, yt, sinVal);
            multSub_32x32_keep32_R(p1, xt, sinVal);

            pSrc[2U * l] = p0;
            pSrc[2U * l + 1U] = p1;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
            i += n1;
            m--;
         } while ( m > 0);                   // butterfly loop end

      }                           // groups loop end

      twidCoefModifier <<= 1U;
   }                             // stages loop end

   n1 = n2;
   n2 = n2 >> 1;
   ia = 0;

   // cosVal = pCoef[ia * 2] = 0;
   // sinVal = pCoef[(ia * 2) + 1] = 1;
   // ia = ia + twidCoefModifier;
   // last stage, the value of cos and sin are fixed

   // loop for butterfly
   for (i = 0; i < fftLen; i += n1)
   {
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
      l = i + n2;
      tmp64_1 = read_q31x2(pSrc + i * 2);
      tmp64_2 = read_q31x2(pSrc + l * 2);
#if __RISCV_XLEN == 64
      write_q31x2(pSrc + 2 * i, __RV_ADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_SUB32(tmp64_1, tmp64_2));
#else
#if defined (NUCLEI_DSP_N3)
      write_q31x2(pSrc + 2 * i, __RV_DADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_DSUB32(tmp64_1, tmp64_2));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

      i += n1;
      l = i + n2;

      tmp64_1 = read_q31x2(pSrc + i * 2);
      tmp64_2 = read_q31x2(pSrc + l * 2);

#if __RISCV_XLEN == 64
      write_q31x2(pSrc + 2 * i, __RV_ADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_SUB32(tmp64_1, tmp64_2));
#else
#if defined (NUCLEI_DSP_N3)
      write_q31x2(pSrc + 2 * i, __RV_DADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_DSUB32(tmp64_1, tmp64_2));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
      l = i + n2;
      xt = pSrc[2 * i] - pSrc[2 * l];
      pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

      pSrc[2U * l] = xt;

      pSrc[2U * l + 1U] = yt;

      i += n1;
      l = i + n2;

      xt = pSrc[2 * i] - pSrc[2 * l];
      pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

      pSrc[2U * l] = xt;

      pSrc[2U * l + 1U] = yt;

#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
   }                             // butterfly loop end

}


RISCV_DSP_ATTRIBUTE void riscv_radix2_butterfly_inverse_q31(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pCoef,
        uint16_t twidCoefModifier)
{

   unsigned i, j, k, l;
   unsigned n1, n2, ia;
   q31_t xt, yt, cosVal, sinVal;
   q31_t p0, p1;

#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
   q63_t coeff, in64_1, in64_2, tmpSub, tmp64_1, tmp64_2;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */

   //N = fftLen;
   n2 = fftLen;

   n1 = n2;
   n2 = n2 >> 1;
   ia = 0;

   // loop for groups
   for (i = 0; i < n2; i++)
   {
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
      coeff = read_q31x2(pCoef + ia * 2);
      ia = ia + twidCoefModifier;
      l = i + n2;
      in64_1 = read_q31x2(pSrc + i * 2);
      in64_2 = read_q31x2(pSrc + l * 2);
#if __RISCV_XLEN == 64
      write_q31x2(pSrc + i * 2, __RV_RADD32(__RV_RADD32(in64_1, in64_2), 0));
      tmpSub= __RV_RSUB32(in64_1, in64_2);
      tmp64_1 = __RV_SMDRS32(tmpSub, coeff);
      tmp64_2 = __RV_KMXDA32(tmpSub, coeff);
      write_q31x2(pSrc + l * 2, __RV_PKTT32(tmp64_2, tmp64_1));
#else
#if defined (NUCLEI_DSP_N3)
      write_q31x2(pSrc + i * 2, __RV_DRADD32(__RV_DRADD32(in64_1, in64_2), 0));
      tmpSub = __RV_DRSUB32(in64_1, in64_2);
      tmp64_1 = __RV_DSMDRS32(tmpSub, coeff);
      tmp64_2 = __RV_DKMXDA32(tmpSub, coeff);
      write_q31x2(pSrc + l * 2, __RV_DPKTT32(tmp64_2, tmp64_1));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
      cosVal = pCoef[ia * 2];
      sinVal = pCoef[(ia * 2) + 1];
      ia = ia + twidCoefModifier;

      l = i + n2;
      xt = (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
      pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

      yt = (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
      pSrc[2 * i + 1] =
        ((pSrc[2 * l + 1] >> 1U) + (pSrc[2 * i + 1] >> 1U)) >> 1U;

      mult_32x32_keep32_R(p0, xt, cosVal);
      mult_32x32_keep32_R(p1, yt, cosVal);
      multSub_32x32_keep32_R(p0, yt, sinVal);
      multAcc_32x32_keep32_R(p1, xt, sinVal);

      pSrc[2U * l] = p0;
      pSrc[2U * l + 1U] = p1;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
   }                             // groups loop end

   twidCoefModifier = twidCoefModifier << 1U;

   // loop for stage
   for (k = fftLen / 2; k > 2; k = k >> 1)
   {
      n1 = n2;
      n2 = n2 >> 1;
      ia = 0;

      // loop for groups
      for (j = 0; j < n2; j++)
      {
         cosVal = pCoef[ia * 2];
         sinVal = pCoef[(ia * 2) + 1];
         ia = ia + twidCoefModifier;

         // loop for butterfly
         for (i = j; i < fftLen; i += n1)
         {
            l = i + n2;
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
            in64_1 = read_q31x2(pSrc + i * 2);
            in64_2 = read_q31x2(pSrc + l * 2);
#if __RISCV_XLEN == 64
            write_q31x2(pSrc + i * 2, __RV_RADD32(in64_1, in64_2));
            tmpSub = __RV_SUB32(in64_1, in64_2);
            tmp64_1 = __RV_SMDRS32(tmpSub, coeff);
            tmp64_2 = __RV_KMXDA32(tmpSub, coeff);
            write_q31x2(pSrc + l * 2, __RV_PKTT32(tmp64_2, tmp64_1));
#else
#if defined (NUCLEI_DSP_N3)
            write_q31x2(pSrc + i * 2, __RV_DRADD32(in64_1, in64_2));
            tmpSub = __RV_DSUB32(in64_1, in64_2);
            tmp64_1 = __RV_DSMDRS32(tmpSub, coeff);
            tmp64_2 = __RV_DKMXDA32(tmpSub, coeff);
            write_q31x2(pSrc + l * 2, __RV_DPKTT32(tmp64_2, tmp64_1));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
            xt = pSrc[2 * i] - pSrc[2 * l];
            pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]) >> 1U;

            yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
            pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]) >> 1U;

            mult_32x32_keep32_R(p0, xt, cosVal);
            mult_32x32_keep32_R(p1, yt, cosVal);
            multSub_32x32_keep32_R(p0, yt, sinVal);
            multAcc_32x32_keep32_R(p1, xt, sinVal);

            pSrc[2U * l] = p0;
            pSrc[2U * l + 1U] = p1;
#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
         }                         // butterfly loop end

      }                           // groups loop end

      twidCoefModifier = twidCoefModifier << 1U;
   }                             // stages loop end

   n1 = n2;
   n2 = n2 >> 1;
   ia = 0;

   // cosVal = pCoef[ia * 2] = 0;
   // sinVal = pCoef[(ia * 2) + 1] = 1;
   // ia = ia + twidCoefModifier;
   // last stage, the value of cos and sin are fixed

   // loop for butterfly
   for (i = 0; i < fftLen; i += n1)
   {
#if defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64)
      l = i + n2;
      tmp64_1 = read_q31x2(pSrc + i * 2);
      tmp64_2 = read_q31x2(pSrc + l * 2);
#if __RISCV_XLEN == 64
      write_q31x2(pSrc + 2 * i, __RV_ADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_SUB32(tmp64_1, tmp64_2));
#else
#if defined (NUCLEI_DSP_N3)
      write_q31x2(pSrc + 2 * i, __RV_DADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_DSUB32(tmp64_1, tmp64_2));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

      i += n1;
      l = i + n2;

      tmp64_1 = read_q31x2(pSrc + i * 2);
      tmp64_2 = read_q31x2(pSrc + l * 2);

#if __RISCV_XLEN == 64
      write_q31x2(pSrc + 2 * i, __RV_ADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_SUB32(tmp64_1, tmp64_2));
#else
#if defined (NUCLEI_DSP_N3)
      write_q31x2(pSrc + 2 * i, __RV_DADD32(tmp64_1, tmp64_2));
      write_q31x2(pSrc + 2 * l, __RV_DSUB32(tmp64_1, tmp64_2));
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
#else
      l = i + n2;
      xt = pSrc[2 * i] - pSrc[2 * l];
      pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

      pSrc[2U * l] = xt;

      pSrc[2U * l + 1U] = yt;

      i += n1;
      l = i + n2;

      xt = pSrc[2 * i] - pSrc[2 * l];
      pSrc[2 * i] = (pSrc[2 * i] + pSrc[2 * l]);

      yt = pSrc[2 * i + 1] - pSrc[2 * l + 1];
      pSrc[2 * i + 1] = (pSrc[2 * l + 1] + pSrc[2 * i + 1]);

      pSrc[2U * l] = xt;

      pSrc[2U * l + 1U] = yt;

#endif /* defined (RISCV_MATH_DSP) && (defined NUCLEI_DSP_N3 || __RISCV_XLEN == 64) */
   }                             // butterfly loop end

}
