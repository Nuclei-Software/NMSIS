/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_cfft_radix8_f16.c
 * Description:  Radix-8 Decimation in Frequency CFFT & CIFFT Floating point processing function
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


#include "dsp/transform_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

void riscv_radix8_butterfly_f16(
  float16_t * pSrc,
  uint16_t fftLen,
  const float16_t * pCoef,
  uint16_t twidCoefModifier);

/* ----------------------------------------------------------------------
 * Internal helper function used by the FFTs
 * -------------------------------------------------------------------- */

/**
  brief         Core function for the floating-point CFFT butterfly process.
  param[in,out] pSrc             points to the in-place buffer of floating-point data type.
  param[in]     fftLen           length of the FFT.
  param[in]     pCoef            points to the twiddle coefficient buffer.
  param[in]     twidCoefModifier twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table.
  return        none
*/

RISCV_DSP_ATTRIBUTE void riscv_radix8_butterfly_f16(
  float16_t * pSrc,
  uint16_t fftLen,
  const float16_t * pCoef,
  uint16_t twidCoefModifier)
{
   uint32_t ia1, ia2, ia3, ia4, ia5, ia6, ia7;
   uint32_t i1, i2, i3, i4, i5, i6, i7, i8;
   uint32_t id;
   uint32_t n1, n2, j;

   float16_t r1, r2, r3, r4, r5, r6, r7, r8;
   float16_t t1, t2;
   float16_t s1, s2, s3, s4, s5, s6, s7, s8;
   float16_t p1, p2, p3, p4;
   float16_t co2, co3, co4, co5, co6, co7, co8;
   float16_t si2, si3, si4, si5, si6, si7, si8;
   const float16_t C81 = 0.70710678118f16;

   n2 = fftLen;

   do
   {
      n1 = n2;
      n2 = n2 >> 3;
      i1 = 0;

      do
      {
         i2 = i1 + n2;
         i3 = i2 + n2;
         i4 = i3 + n2;
         i5 = i4 + n2;
         i6 = i5 + n2;
         i7 = i6 + n2;
         i8 = i7 + n2;
         r1 = (float16_t)pSrc[2 * i1] + (float16_t)pSrc[2 * i5];
         r5 = (float16_t)pSrc[2 * i1] - (float16_t)pSrc[2 * i5];
         r2 = (float16_t)pSrc[2 * i2] + (float16_t)pSrc[2 * i6];
         r6 = (float16_t)pSrc[2 * i2] - (float16_t)pSrc[2 * i6];
         r3 = (float16_t)pSrc[2 * i3] + (float16_t)pSrc[2 * i7];
         r7 = (float16_t)pSrc[2 * i3] - (float16_t)pSrc[2 * i7];
         r4 = (float16_t)pSrc[2 * i4] + (float16_t)pSrc[2 * i8];
         r8 = (float16_t)pSrc[2 * i4] - (float16_t)pSrc[2 * i8];
         t1 = (float16_t)r1 - (float16_t)r3;
         r1 = (float16_t)r1 + (float16_t)r3;
         r3 = (float16_t)r2 - (float16_t)r4;
         r2 = (float16_t)r2 + (float16_t)r4;
         pSrc[2 * i1] = (float16_t)r1 + (float16_t)r2;
         pSrc[2 * i5] = (float16_t)r1 - (float16_t)r2;
         r1 = (float16_t)pSrc[2 * i1 + 1] + (float16_t)pSrc[2 * i5 + 1];
         s5 = (float16_t)pSrc[2 * i1 + 1] - (float16_t)pSrc[2 * i5 + 1];
         r2 = (float16_t)pSrc[2 * i2 + 1] + (float16_t)pSrc[2 * i6 + 1];
         s6 = (float16_t)pSrc[2 * i2 + 1] - (float16_t)pSrc[2 * i6 + 1];
         s3 = (float16_t)pSrc[2 * i3 + 1] + (float16_t)pSrc[2 * i7 + 1];
         s7 = (float16_t)pSrc[2 * i3 + 1] - (float16_t)pSrc[2 * i7 + 1];
         r4 = (float16_t)pSrc[2 * i4 + 1] + (float16_t)pSrc[2 * i8 + 1];
         s8 = (float16_t)pSrc[2 * i4 + 1] - (float16_t)pSrc[2 * i8 + 1];
         t2 = (float16_t)r1 - (float16_t)s3;
         r1 = (float16_t)r1 + (float16_t)s3;
         s3 = (float16_t)r2 - (float16_t)r4;
         r2 = (float16_t)r2 + (float16_t)r4;
         pSrc[2 * i1 + 1] = (float16_t)r1 + (float16_t)r2;
         pSrc[2 * i5 + 1] = (float16_t)r1 - (float16_t)r2;
         pSrc[2 * i3]     = (float16_t)t1 + (float16_t)s3;
         pSrc[2 * i7]     = (float16_t)t1 - (float16_t)s3;
         pSrc[2 * i3 + 1] = (float16_t)t2 - (float16_t)r3;
         pSrc[2 * i7 + 1] = (float16_t)t2 + (float16_t)r3;
         r1 = ((float16_t)r6 - (float16_t)r8) * (float16_t)C81;
         r6 = ((float16_t)r6 + (float16_t)r8) * (float16_t)C81;
         r2 = ((float16_t)s6 - (float16_t)s8) * (float16_t)C81;
         s6 = ((float16_t)s6 + (float16_t)s8) * (float16_t)C81;
         t1 = (float16_t)r5 - (float16_t)r1;
         r5 = (float16_t)r5 + (float16_t)r1;
         r8 = (float16_t)r7 - (float16_t)r6;
         r7 = (float16_t)r7 + (float16_t)r6;
         t2 = (float16_t)s5 - (float16_t)r2;
         s5 = (float16_t)s5 + (float16_t)r2;
         s8 = (float16_t)s7 - (float16_t)s6;
         s7 = (float16_t)s7 + (float16_t)s6;
         pSrc[2 * i2]     = (float16_t)r5 + (float16_t)s7;
         pSrc[2 * i8]     = (float16_t)r5 - (float16_t)s7;
         pSrc[2 * i6]     = (float16_t)t1 + (float16_t)s8;
         pSrc[2 * i4]     = (float16_t)t1 - (float16_t)s8;
         pSrc[2 * i2 + 1] = (float16_t)s5 - (float16_t)r7;
         pSrc[2 * i8 + 1] = (float16_t)s5 + (float16_t)r7;
         pSrc[2 * i6 + 1] = (float16_t)t2 - (float16_t)r8;
         pSrc[2 * i4 + 1] = (float16_t)t2 + (float16_t)r8;

         i1 += n1;
      } while (i1 < fftLen);

      if (n2 < 8)
         break;

      ia1 = 0;
      j = 1;

      do
      {
         /*  index calculation for the coefficients */
         id  = ia1 + twidCoefModifier;
         ia1 = id;
         ia2 = ia1 + id;
         ia3 = ia2 + id;
         ia4 = ia3 + id;
         ia5 = ia4 + id;
         ia6 = ia5 + id;
         ia7 = ia6 + id;

         co2 = pCoef[2 * ia1];
         co3 = pCoef[2 * ia2];
         co4 = pCoef[2 * ia3];
         co5 = pCoef[2 * ia4];
         co6 = pCoef[2 * ia5];
         co7 = pCoef[2 * ia6];
         co8 = pCoef[2 * ia7];
         si2 = pCoef[2 * ia1 + 1];
         si3 = pCoef[2 * ia2 + 1];
         si4 = pCoef[2 * ia3 + 1];
         si5 = pCoef[2 * ia4 + 1];
         si6 = pCoef[2 * ia5 + 1];
         si7 = pCoef[2 * ia6 + 1];
         si8 = pCoef[2 * ia7 + 1];

         i1 = j;

         do
         {
            /*  index calculation for the input */
            i2 = i1 + n2;
            i3 = i2 + n2;
            i4 = i3 + n2;
            i5 = i4 + n2;
            i6 = i5 + n2;
            i7 = i6 + n2;
            i8 = i7 + n2;
            r1 = (float16_t)pSrc[2 * i1] + (float16_t)pSrc[2 * i5];
            r5 = (float16_t)pSrc[2 * i1] - (float16_t)pSrc[2 * i5];
            r2 = (float16_t)pSrc[2 * i2] + (float16_t)pSrc[2 * i6];
            r6 = (float16_t)pSrc[2 * i2] - (float16_t)pSrc[2 * i6];
            r3 = (float16_t)pSrc[2 * i3] + (float16_t)pSrc[2 * i7];
            r7 = (float16_t)pSrc[2 * i3] - (float16_t)pSrc[2 * i7];
            r4 = (float16_t)pSrc[2 * i4] + (float16_t)pSrc[2 * i8];
            r8 = (float16_t)pSrc[2 * i4] - (float16_t)pSrc[2 * i8];
            t1 = (float16_t)r1 - (float16_t)r3;
            r1 = (float16_t)r1 + (float16_t)r3;
            r3 = (float16_t)r2 - (float16_t)r4;
            r2 = (float16_t)r2 + (float16_t)r4;
            pSrc[2 * i1] = (float16_t)r1 + (float16_t)r2;
            r2 = (float16_t)r1 - (float16_t)r2;
            s1 = (float16_t)pSrc[2 * i1 + 1] + (float16_t)pSrc[2 * i5 + 1];
            s5 = (float16_t)pSrc[2 * i1 + 1] - (float16_t)pSrc[2 * i5 + 1];
            s2 = (float16_t)pSrc[2 * i2 + 1] + (float16_t)pSrc[2 * i6 + 1];
            s6 = (float16_t)pSrc[2 * i2 + 1] - (float16_t)pSrc[2 * i6 + 1];
            s3 = (float16_t)pSrc[2 * i3 + 1] + (float16_t)pSrc[2 * i7 + 1];
            s7 = (float16_t)pSrc[2 * i3 + 1] - (float16_t)pSrc[2 * i7 + 1];
            s4 = (float16_t)pSrc[2 * i4 + 1] + (float16_t)pSrc[2 * i8 + 1];
            s8 = (float16_t)pSrc[2 * i4 + 1] - (float16_t)pSrc[2 * i8 + 1];
            t2 = (float16_t)s1 - (float16_t)s3;
            s1 = (float16_t)s1 + (float16_t)s3;
            s3 = (float16_t)s2 - (float16_t)s4;
            s2 = (float16_t)s2 + (float16_t)s4;
            r1 = (float16_t)t1 + (float16_t)s3;
            t1 = (float16_t)t1 - (float16_t)s3;
            pSrc[2 * i1 + 1] = (float16_t)s1 + (float16_t)s2;
            s2 = (float16_t)s1 - (float16_t)s2;
            s1 = (float16_t)t2 - (float16_t)r3;
            t2 = (float16_t)t2 + (float16_t)r3;
            p1 = (float16_t)co5 * (float16_t)r2;
            p2 = (float16_t)si5 * (float16_t)s2;
            p3 = (float16_t)co5 * (float16_t)s2;
            p4 = (float16_t)si5 * (float16_t)r2;
            pSrc[2 * i5]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i5 + 1] = (float16_t)p3 - (float16_t)p4;
            p1 = (float16_t)co3 * (float16_t)r1;
            p2 = (float16_t)si3 * (float16_t)s1;
            p3 = (float16_t)co3 * (float16_t)s1;
            p4 = (float16_t)si3 * (float16_t)r1;
            pSrc[2 * i3]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i3 + 1] = (float16_t)p3 - (float16_t)p4;
            p1 = (float16_t)co7 * (float16_t)t1;
            p2 = (float16_t)si7 * (float16_t)t2;
            p3 = (float16_t)co7 * (float16_t)t2;
            p4 = (float16_t)si7 * (float16_t)t1;
            pSrc[2 * i7]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i7 + 1] = (float16_t)p3 - (float16_t)p4;
            r1 = ((float16_t)r6 - (float16_t)r8) * (float16_t)C81;
            r6 = ((float16_t)r6 + (float16_t)r8) * (float16_t)C81;
            s1 = ((float16_t)s6 - (float16_t)s8) * (float16_t)C81;
            s6 = ((float16_t)s6 + (float16_t)s8) * (float16_t)C81;
            t1 = (float16_t)r5 - (float16_t)r1;
            r5 = (float16_t)r5 + (float16_t)r1;
            r8 = (float16_t)r7 - (float16_t)r6;
            r7 = (float16_t)r7 + (float16_t)r6;
            t2 = (float16_t)s5 - (float16_t)s1;
            s5 = (float16_t)s5 + (float16_t)s1;
            s8 = (float16_t)s7 - (float16_t)s6;
            s7 = (float16_t)s7 + (float16_t)s6;
            r1 = (float16_t)r5 + (float16_t)s7;
            r5 = (float16_t)r5 - (float16_t)s7;
            r6 = (float16_t)t1 + (float16_t)s8;
            t1 = (float16_t)t1 - (float16_t)s8;
            s1 = (float16_t)s5 - (float16_t)r7;
            s5 = (float16_t)s5 + (float16_t)r7;
            s6 = (float16_t)t2 - (float16_t)r8;
            t2 = (float16_t)t2 + (float16_t)r8;
            p1 = (float16_t)co2 * (float16_t)r1;
            p2 = (float16_t)si2 * (float16_t)s1;
            p3 = (float16_t)co2 * (float16_t)s1;
            p4 = (float16_t)si2 * (float16_t)r1;
            pSrc[2 * i2]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i2 + 1] = (float16_t)p3 - (float16_t)p4;
            p1 = (float16_t)co8 * (float16_t)r5;
            p2 = (float16_t)si8 * (float16_t)s5;
            p3 = (float16_t)co8 * (float16_t)s5;
            p4 = (float16_t)si8 * (float16_t)r5;
            pSrc[2 * i8]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i8 + 1] = (float16_t)p3 - (float16_t)p4;
            p1 = (float16_t)co6 * (float16_t)r6;
            p2 = (float16_t)si6 * (float16_t)s6;
            p3 = (float16_t)co6 * (float16_t)s6;
            p4 = (float16_t)si6 * (float16_t)r6;
            pSrc[2 * i6]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i6 + 1] = (float16_t)p3 - (float16_t)p4;
            p1 = (float16_t)co4 * (float16_t)t1;
            p2 = (float16_t)si4 * (float16_t)t2;
            p3 = (float16_t)co4 * (float16_t)t2;
            p4 = (float16_t)si4 * (float16_t)t1;
            pSrc[2 * i4]     = (float16_t)p1 + (float16_t)p2;
            pSrc[2 * i4 + 1] = (float16_t)p3 - (float16_t)p4;

            i1 += n1;
         } while (i1 < fftLen);

         j++;
      } while (j < n2);

      twidCoefModifier <<= 3;
   } while (n2 > 7);
}

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
