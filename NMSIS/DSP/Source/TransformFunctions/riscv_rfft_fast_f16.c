/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rfft_fast_f16.c
 * Description:  RFFT & RIFFT Floating point process function
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
#include "riscv_common_tables_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


#if defined(RISCV_MATH_VECTOR_FLOAT16) 
/*

No stage merge functions defined here for RVV.

*/
#else
static void stage_rfft_f16(
  const riscv_rfft_fast_instance_f16 * S,
  const float16_t * p,
        float16_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float16_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float16_t * pCoeff = S->pTwiddleRFFT;       /* Points to RFFT Twiddle factors */
  const float16_t *pA = p;                          /* increasing pointer */
  const float16_t *pB = p;                          /* decreasing pointer */
        float16_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float16_t t1a, t1b;                         /* temporary variables */
        float16_t p0, p1, p2, p3;                   /* temporary variables */


   k = (S->Sint).fftLen - 1;

   /* Pack first and last sample of the frequency domain together */

   xBR = pB[0];
   xBI = pB[1];
   xAR = pA[0];
   xAI = pA[1];

   twR = *pCoeff++ ;
   twI = *pCoeff++ ;


   // U1 = XA(1) + XB(1); % It is real
   t1a = (_Float16)xBR + (_Float16)xAR  ;

   // U2 = XB(1) - XA(1); % It is imaginary
   t1b = (_Float16)xBI + (_Float16)xAI  ;

   // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
   // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
   *pOut++ = 0.5f16 * ( (_Float16)t1a + (_Float16)t1b );
   *pOut++ = 0.5f16 * ( (_Float16)t1a - (_Float16)t1b );

   // XA(1) = 1/2*( U1 - imag(U2) +  i*( U1 +imag(U2) ));
   pB  = p + 2*k;
   pA += 2;

   do
   {
      /*
         function X = my_split_rfft(X, ifftFlag)
         % X is a series of real numbers
         L  = length(X);
         XC = X(1:2:end) +i*X(2:2:end);
         XA = fft(XC);
         XB = conj(XA([1 end:-1:2]));
         TW = i*exp(-2*pi*i*[0:L/2-1]/L).';
         for l = 2:L/2
            XA(l) = 1/2 * (XA(l) + XB(l) + TW(l) * (XB(l) - XA(l)));
         end
         XA(1) = 1/2* (XA(1) + XB(1) + TW(1) * (XB(1) - XA(1))) + i*( 1/2*( XA(1) + XB(1) + i*( XA(1) - XB(1))));
         X = XA;
      */

      xBI = pB[1];
      xBR = pB[0];
      xAR = pA[0];
      xAI = pA[1];

      twR = *pCoeff++;
      twI = *pCoeff++;

      t1a = (_Float16)xBR - (_Float16)xAR ;
      t1b = (_Float16)xBI + (_Float16)xAI ;

      // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
      // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
      p0 = (_Float16)twR * (_Float16)t1a;
      p1 = (_Float16)twI * (_Float16)t1a;
      p2 = (_Float16)twR * (_Float16)t1b;
      p3 = (_Float16)twI * (_Float16)t1b;

      *pOut++ = 0.5f16 * ((_Float16)xAR + (_Float16)xBR + (_Float16)p0 + (_Float16)p3 ); //xAR
      *pOut++ = 0.5f16 * ((_Float16)xAI - (_Float16)xBI + (_Float16)p1 - (_Float16)p2 ); //xAI


      pA += 2;
      pB -= 2;
      k--;
   } while (k > 0);
}

/* Prepares data for inverse cfft */
static void merge_rfft_f16(
  const riscv_rfft_fast_instance_f16 * S,
  const float16_t * p,
        float16_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float16_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float16_t *pCoeff = S->pTwiddleRFFT;        /* Points to RFFT Twiddle factors */
  const float16_t *pA = p;                          /* increasing pointer */
  const float16_t *pB = p;                          /* decreasing pointer */
        float16_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float16_t t1a, t1b, r, s, t, u;             /* temporary variables */

   k = (S->Sint).fftLen - 1;

   xAR = pA[0];
   xAI = pA[1];

   pCoeff += 2 ;

   *pOut++ = 0.5f16 * ( (_Float16)xAR + (_Float16)xAI );
   *pOut++ = 0.5f16 * ( (_Float16)xAR - (_Float16)xAI );

   pB  =  p + 2*k ;
   pA +=  2	   ;

   while (k > 0)
   {
      /* G is half of the frequency complex spectrum */
      //for k = 2:N
      //    Xk(k) = 1/2 * (G(k) + conj(G(N-k+2)) + Tw(k)*( G(k) - conj(G(N-k+2))));
      xBI =   pB[1]    ;
      xBR =   pB[0]    ;
      xAR =  pA[0];
      xAI =  pA[1];

      twR = *pCoeff++;
      twI = *pCoeff++;

      t1a = (_Float16)xAR - (_Float16)xBR ;
      t1b = (_Float16)xAI + (_Float16)xBI ;

      r = (_Float16)twR * (_Float16)t1a;
      s = (_Float16)twI * (_Float16)t1b;
      t = (_Float16)twI * (_Float16)t1a;
      u = (_Float16)twR * (_Float16)t1b;

      // real(tw * (xA - xB)) = twR * (xAR - xBR) - twI * (xAI - xBI);
      // imag(tw * (xA - xB)) = twI * (xAR - xBR) + twR * (xAI - xBI);
      *pOut++ = 0.5f16 * ((_Float16)xAR + (_Float16)xBR - (_Float16)r - (_Float16)s ); //xAR
      *pOut++ = 0.5f16 * ((_Float16)xAI - (_Float16)xBI + (_Float16)t - (_Float16)u ); //xAI

      pA += 2;
      pB -= 2;
      k--;
   }

}

#endif /* #if defined(RISCV_MATH_VECTOR_FLOAT16) */

/**
  @ingroup RealFFT
*/

/**
  @defgroup RealFFTF16 Real FFT F16 Functions
*/

/**
  @addtogroup RealFFTF16
  @{
*/

/**
  @brief         Processing function for the floating-point real FFT.
  @param[in]     S         points to an riscv_rfft_fast_instance_f16 structure
  @param[in]     p         points to input buffer (Source buffer is modified by this function.)
  @param[in]     pOut      points to output buffer
  @param[in]     ifftFlag
                   - value = 0: RFFT
                   - value = 1: RIFFT

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
*/
#if defined(RISCV_MATH_VECTOR_FLOAT16) 

typedef struct {
    float16_t re;
    float16_t im;
} cplxf16;

RISCV_DSP_ATTRIBUTE void riscv_rfft_fast_f16(
  const riscv_rfft_fast_instance_f16 * S,
  const float16_t * p,
  float16_t * pOut,
  float16_t *tmpbuf,
  uint8_t ifftFlag)
{
    const long N = S->fftLenRFFT;
    if (ifftFlag) {
        size_t avl = (N >> 1) - 1;
        const cplxf16 *px = (const cplxf16 *)p + 1;
        const cplxf16 *px_inv = (const cplxf16 *)(p + N) - 1;
        const float16_t *ptwd_re = S->ptwd_re + 1;
        const float16_t *ptwd_im = S->ptwd_im + 1;
        float16_t *py = pOut + 2;
        pOut[0] = (p[0] + p[1]) * 0.5f;
        pOut[1] = (p[0] - p[1]) * 0.5f;
        do {
            size_t vl = __riscv_vsetvl_e16m2(avl);

            vfloat16m2x2_t vx =
                __riscv_vlseg2e16_v_f16m2x2((const float16_t *)px, vl);
            px += vl;
            vfloat16m2_t vx_re = __riscv_vget_v_f16m2x2_f16m2(vx, 0);
            vfloat16m2_t vx_im = __riscv_vget_v_f16m2x2_f16m2(vx, 1);

            vfloat16m2x2_t vx_inv = __riscv_vlsseg2e16_v_f16m2x2(
                (const float16_t *)px_inv, -sizeof(cplxf16), vl);
            px_inv -= vl;
            vfloat16m2_t vx_inv_re = __riscv_vget_v_f16m2x2_f16m2(vx_inv, 0);
            vfloat16m2_t vx_inv_im = __riscv_vget_v_f16m2x2_f16m2(vx_inv, 1);
            vx_inv_im = __riscv_vfneg_v_f16m2(vx_inv_im, vl);

            vfloat16m2_t vy_re = __riscv_vfadd_vv_f16m2(vx_re, vx_inv_re, vl);
            vfloat16m2_t vy_im = __riscv_vfadd_vv_f16m2(vx_im, vx_inv_im, vl);

            vfloat16m2_t vtmp_re = __riscv_vfsub_vv_f16m2(vx_re, vx_inv_re, vl);
            vfloat16m2_t vtmp_im = __riscv_vfsub_vv_f16m2(vx_im, vx_inv_im, vl);

            vfloat16m2_t vtwd_re = __riscv_vle16_v_f16m2(ptwd_im, vl);
            ptwd_im += vl;
            vfloat16m2_t vtwd_im = __riscv_vle16_v_f16m2(ptwd_re, vl);
            ptwd_re += vl;

            vy_re = __riscv_vfadd_vv_f16m2(
                vy_re, __riscv_vfmul_vv_f16m2(vtmp_re, vtwd_re, vl), vl);
            vy_re = __riscv_vfsub_vv_f16m2(
                vy_re, __riscv_vfmul_vv_f16m2(vtmp_im, vtwd_im, vl), vl);

            vy_im = __riscv_vfadd_vv_f16m2(
                vy_im, __riscv_vfmul_vv_f16m2(vtmp_re, vtwd_im, vl), vl);
            vy_im = __riscv_vfadd_vv_f16m2(
                vy_im, __riscv_vfmul_vv_f16m2(vtmp_im, vtwd_re, vl), vl);

            vy_re = __riscv_vfmul_vf_f16m2(vy_re, 0.5f, vl);
            vy_im = __riscv_vfmul_vf_f16m2(vy_im, 0.5f, vl);

            vfloat16m2x2_t v_tuple;
            v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vy_re);
            v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vy_im);
            __riscv_vsseg2e16_v_f16m2x2(py, v_tuple, vl);

            py += 2 * vl;
            avl -= vl;
        } while (avl != 0);

        riscv_cfft_f16(&S->Sint, pOut, pOut, tmpbuf, ifftFlag);

    } else {
        float16_t *y = tmpbuf + 2 * N;
        riscv_cfft_f16(&S->Sint, p, y, tmpbuf, ifftFlag);

        // Y[0] = F[0] + G[0]
        const float16_t F0 = y[0];
        const float16_t G0 = y[1];
        pOut[0] = F0 + G0;
        pOut[1] = F0 - G0;

        size_t avl = (N >> 1) - 1;
        const float16_t *py = y + 2;
        const float16_t *py_inv = y + N - 2;
        float16_t *pout = pOut + 2;
        const float16_t *ptwd_re = S->ptwd_re + 1;
        const float16_t *ptwd_im = S->ptwd_im + 1;
        do {
            size_t vl = __riscv_vsetvl_e16m2(avl);

            // load vx_re, vx_im
            vfloat16m2x2_t v_tuple = __riscv_vlseg2e16_v_f16m2x2(py, vl);
            py += vl * 2;
            vfloat16m2_t vy_re = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 0);
            vfloat16m2_t vy_im = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 1);

            v_tuple = __riscv_vlsseg2e16_v_f16m2x2(py_inv,
                                                   -sizeof(float16_t) * 2, vl);
            py_inv -= vl * 2;
            vfloat16m2_t vy_inv_re = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 0);
            vfloat16m2_t vy_inv_im = __riscv_vget_v_f16m2x2_f16m2(v_tuple, 1);

            // vFr
            vfloat16m2_t vFr_re = __riscv_vfadd_vv_f16m2(vy_re, vy_inv_re, vl);
            vfloat16m2_t vFr_im = __riscv_vfsub_vv_f16m2(vy_im, vy_inv_im, vl);

            // vGr
            vfloat16m2_t vGr_re = __riscv_vfadd_vv_f16m2(vy_inv_im, vy_im, vl);
            vfloat16m2_t vGr_im = __riscv_vfsub_vv_f16m2(vy_inv_re, vy_re, vl);

            vfloat16m2_t vtwd_re = __riscv_vle16_v_f16m2(ptwd_re, vl);
            ptwd_re += vl;
            vfloat16m2_t vtwd_im = __riscv_vle16_v_f16m2(ptwd_im, vl);
            ptwd_im += vl;

            vfloat16m2_t vout_re = __riscv_vfsub_vv_f16m2(
                __riscv_vfmul_vv_f16m2(vGr_re, vtwd_re, vl),
                __riscv_vfmul_vv_f16m2(vGr_im, vtwd_im, vl), vl);
            vfloat16m2_t vout_im = __riscv_vfadd_vv_f16m2(
                __riscv_vfmul_vv_f16m2(vGr_im, vtwd_re, vl),
                __riscv_vfmul_vv_f16m2(vGr_re, vtwd_im, vl), vl);

            vout_re = __riscv_vfadd_vv_f16m2(vFr_re, vout_re, vl);
            vout_im = __riscv_vfadd_vv_f16m2(vFr_im, vout_im, vl);

            vout_re = __riscv_vfmul_vf_f16m2(vout_re, 0.5f, vl);
            vout_im = __riscv_vfmul_vf_f16m2(vout_im, 0.5f, vl);

            v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 0, vout_re);
            v_tuple = __riscv_vset_v_f16m2_f16m2x2(v_tuple, 1, vout_im);
            __riscv_vsseg2e16_v_f16m2x2(pout, v_tuple, vl);
            pout += 2 * vl;

            avl -= vl;
        } while (avl != 0);
    }
}

#else
RISCV_DSP_ATTRIBUTE void riscv_rfft_fast_f16(
  const riscv_rfft_fast_instance_f16 * S,
  float16_t * p,
  float16_t * pOut,
  uint8_t ifftFlag)
{
   const riscv_cfft_instance_f16 * Sint = &(S->Sint);


   /* Calculation of Real FFT */
   if (ifftFlag)
   {
      /*  Real FFT compression */
      merge_rfft_f16(S, p, pOut);
      /* Complex radix-4 IFFT process */
      riscv_cfft_f16( Sint, pOut, ifftFlag, 1);
   }
   else
   {

      /* Calculation of RFFT of input */
      riscv_cfft_f16( Sint, p, ifftFlag, 1);

      /*  Real FFT extraction */
      stage_rfft_f16(S, p, pOut);
   }
}
#endif
/**
* @} end of RealFFTF16 group
*/

#endif /*  #if defined(RISCV_FLOAT16_SUPPORTED) */
