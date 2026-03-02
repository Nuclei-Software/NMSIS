/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_rfft_fast_f32.c
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


#include "dsp/transform_functions.h"

#if defined(RISCV_MATH_VECTOR) 
/*

No stage merge functions defined here for RVV.

*/
#else
static void stage_rfft_f32(
  const riscv_rfft_fast_instance_f32 * S,
  const float32_t * p,
        float32_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float32_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float32_t * pCoeff = S->pTwiddleRFFT;       /* Points to RFFT Twiddle factors */
  const float32_t *pA = p;                          /* increasing pointer */
  const float32_t *pB = p;                          /* decreasing pointer */
        float32_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float32_t t1a, t1b;                         /* temporary variables */
        float32_t p0, p1, p2, p3;                   /* temporary variables */


   k = (S->Sint).fftLen - 1;

   /* Pack first and last sample of the frequency domain together */

   xBR = pB[0];
   xBI = pB[1];
   xAR = pA[0];
   xAI = pA[1];

   twR = *pCoeff++ ;
   twI = *pCoeff++ ;


   // U1 = XA(1) + XB(1); % It is real
   t1a = xBR + xAR  ;

   // U2 = XB(1) - XA(1); % It is imaginary
   t1b = xBI + xAI  ;

   // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
   // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
   *pOut++ = 0.5f * ( t1a + t1b );
   *pOut++ = 0.5f * ( t1a - t1b );

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

      t1a = xBR - xAR ;
      t1b = xBI + xAI ;

      // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
      // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
      p0 = twR * t1a;
      p1 = twI * t1a;
      p2 = twR * t1b;
      p3 = twI * t1b;

      *pOut++ = 0.5f * (xAR + xBR + p0 + p3 ); //xAR
      *pOut++ = 0.5f * (xAI - xBI + p1 - p2 ); //xAI


      pA += 2;
      pB -= 2;
      k--;
   } while (k > 0);
}

/* Prepares data for inverse cfft */
static void merge_rfft_f32(
  const riscv_rfft_fast_instance_f32 * S,
  const float32_t * p,
        float32_t * pOut)
{
        int32_t  k;                                /* Loop Counter */
        float32_t twR, twI;                         /* RFFT Twiddle coefficients */
  const float32_t *pCoeff = S->pTwiddleRFFT;        /* Points to RFFT Twiddle factors */
  const float32_t *pA = p;                          /* increasing pointer */
  const float32_t *pB = p;                          /* decreasing pointer */
        float32_t xAR, xAI, xBR, xBI;               /* temporary variables */
        float32_t t1a, t1b, r, s, t, u;             /* temporary variables */

   k = (S->Sint).fftLen - 1;

   xAR = pA[0];
   xAI = pA[1];

   pCoeff += 2 ;

   *pOut++ = 0.5f * ( xAR + xAI );
   *pOut++ = 0.5f * ( xAR - xAI );

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

      t1a = xAR - xBR ;
      t1b = xAI + xBI ;

      r = twR * t1a;
      s = twI * t1b;
      t = twI * t1a;
      u = twR * t1b;

      // real(tw * (xA - xB)) = twR * (xAR - xBR) - twI * (xAI - xBI);
      // imag(tw * (xA - xB)) = twI * (xAR - xBR) + twR * (xAI - xBI);
      *pOut++ = 0.5f * (xAR + xBR - r - s ); //xAR
      *pOut++ = 0.5f * (xAI - xBI + t - u ); //xAI

      pA += 2;
      pB -= 2;
      k--;
   }

}

#endif /* #if defined(RISCV_MATH_VECTOR) */

/**
  @ingroup groupTransforms
*/

/**
  @defgroup RealFFT Real FFT Functions

  @par
                   The NMSIS DSP library includes specialized algorithms for computing the
                   FFT of real data sequences.  The FFT is defined over complex data but
                   in many applications the input is real.  Real FFT algorithms take advantage
                   of the symmetry properties of the FFT and have a speed advantage over complex
                   algorithms of the same length.
  @par
                   The Fast RFFT algorithm relays on the mixed radix CFFT that save processor usage.
  @par
                   The real length N forward FFT of a sequence is computed using the steps shown below.
  @par
                   \image html RFFT.png "Real Fast Fourier Transform"
  @par
                   The real sequence is initially treated as if it were complex to perform a CFFT.
                   Later, a processing stage reshapes the data to obtain half of the frequency spectrum
                   in complex format.

  @par
                   The input for the inverse RFFT should keep the same format as the output of the
                   forward RFFT. A first processing stage pre-process the data to later perform an
                   inverse CFFT.
  @par
                   \image html RIFFT.png "Real Inverse Fast Fourier Transform"
  @par
                   The algorithms for floating-point, Q15, and Q31 data are slightly different
                   and we describe each algorithm in turn.
  @par           Floating-point
                   The main functions are \ref riscv_rfft_fast_f32() and \ref riscv_rfft_fast_init_f32().

                   For f16, the functions are \ref riscv_rfft_fast_f16() and \ref riscv_rfft_fast_init_f16().
                   For f64, the functions are \ref riscv_rfft_fast_f64() and \ref riscv_rfft_fast_init_f64().
  @par
                   The FFT of a real N-point sequence has even symmetry in the frequency domain.
                   The second half of the data equals the conjugate of the first half flipped in frequency.
                   This conjugate part is not computed by the float RFFT. As consequence, the output of
                   a N point real FFT should be a N//2 + 1 complex numbers so N + 2 floats.
  @par
                   It happens that the first complex of number of the RFFT output is actually
                   all real. Its real part represents the DC offset.
                   The value at Nyquist frequency is also real.

  @par
                   Those two complex numbers can be encoded with 2 floats rather than using two numbers
                   with an imaginary part set to zero.
  @par
                   The implementation is using a trick so that the output buffer can be N float :
                   the last real is packaged in the imaginary part of the first complex (since
                   this imaginary part is not used and is zero).

  @par
                   The real FFT functions pack the frequency domain data in this fashion.
                   The forward transform outputs the data in this form and the inverse
                   transform expects input data in this form. The function always performs
                   the needed bitreversal so that the input and output data is always in
                   normal order. The functions support lengths of [32, 64, 128, ..., 4096]
                   samples.
  @par           Q15 and Q31
                   The real algorithms are defined in a similar manner and utilize N/2 complex
                   transforms behind the scenes.

  @par
                   But warning, contrary to the float version, the fixed point implementation
                   RFFT is also computing the conjugate part (except for MVE version) so the 
                   output buffer must be bigger.
                   Also the fixed point RFFTs are not using any trick to pack the DC and Nyquist
                   frequency in the same complex number.
                   The RIFFT is not using the conjugate part but it is still using the Nyquist
                   frequency value. The details are given in the documentation for the functions.
  @par
                   The complex transforms used internally include scaling to prevent fixed-point
                   overflows.  The overall scaling equals 1/(fftLen/2).
                   Due to the use of complex transform internally, the source buffer is
                   modified by the rfft.
  @par
                   A separate instance structure must be defined for each transform used but
                   twiddle factor and bit reversal tables can be reused.
  @par
                   There is also an associated initialization function for each data type.
                   The initialization function performs the following operations:
                    - Sets the values of the internal structure fields.
                    - Initializes twiddle factor table and bit reversal table pointers.
                    - Initializes the internal complex FFT data structure.
  @par
                   Use of the initialization function is optional **except for MVE versions where it is mandatory**.
                   If you don't use the initialization functions, then the structures should be initialized with code
                   similar to the one below:
  @code
      riscv_rfft_instance_q31 S = {fftLenReal, fftLenBy2, ifftFlagR, bitReverseFlagR, twidCoefRModifier, pTwiddleAReal, pTwiddleBReal, pCfft};
      riscv_rfft_instance_q15 S = {fftLenReal, fftLenBy2, ifftFlagR, bitReverseFlagR, twidCoefRModifier, pTwiddleAReal, pTwiddleBReal, pCfft};
  @endcode
                   where <code>fftLenReal</code> is the length of the real transform;
                   <code>fftLenBy2</code> length of  the internal complex transform (fftLenReal/2).
                   <code>ifftFlagR</code> Selects forward (=0) or inverse (=1) transform.
                   <code>bitReverseFlagR</code> Selects bit reversed output (=0) or normal order
                   output (=1).
                   <code>twidCoefRModifier</code> stride modifier for the twiddle factor table.
                   The value is based on the FFT length;
                   <code>pTwiddleAReal</code>points to the A array of twiddle coefficients;
                   <code>pTwiddleBReal</code>points to the B array of twiddle coefficients;
                   <code>pCfft</code> points to the CFFT Instance structure. The CFFT structure
                   must also be initialized.  
@par
                   Note that with MVE versions you can't initialize instance structures directly and **must
                   use the initialization function**.

 @par RVV version
                     The rvv version has a different API.
                     The input and output buffers must be
                     different.
                     There is a temporary buffer that is not optional.
                     
                     The bit reverse flag is not more 
                     available in rvv version.


  @code
        void riscv_rfft_fast_f32(
                const riscv_rfft_fast_instance_f32 * S,
                float32_t * p, 
                float32_t * pOut,
                float32_t *tmpbuf,
                uint8_t ifftFlag);
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
 */

/**
  @defgroup DeprecatedRealFFT Deprecated Real FFT Functions
*/

/**
  @defgroup RealFFTF32 Real FFT F32 Functions
*/
/**
  @addtogroup RealFFTF32
  @{
*/

/**
  @brief         Processing function for the floating-point real FFT.
  @param[in]     S         points to an riscv_rfft_fast_instance_f32 structure
  @param[in]     p         points to input buffer (Source buffer is modified by this function.)
  @param[in]     pOut      points to output buffer
  @param[in]     ifftFlag
                   - value = 0: RFFT
                   - value = 1: RIFFT

  @par RVV version
                     The rvv version has a different API.
                     The input and output buffers must be
                     different.
                     There is a temporary buffer.
  @par
                     The bit reverse flag is not more 
                     available in rvv version.

  @par
   @code
        void riscv_rfft_fast_f32(
                const riscv_rfft_fast_instance_f32 * S,
                const float32_t * p, 
                float32_t * pOut,
                float32_t *tmpbuf,
                uint8_t ifftFlag);
  @endcode

  @par Size of buffers according to the target architecture and datatype:
       They are described on the page \ref transformbuffers "transform buffers".
*/

#if defined(RISCV_MATH_VECTOR) 


/*

p size      : nfft   reals
pOut size   : nfft   reals (nfft/2 complex)
tmpBuf size : 2*nfft reals (nfft   complex)

*/
typedef struct {
    float32_t re;
    float32_t im;
} cplxf32;

RISCV_DSP_ATTRIBUTE void riscv_rfft_fast_f32(
  const riscv_rfft_fast_instance_f32 * S,
  const float32_t * p,
  float32_t * pOut,
  float32_t *tmpbuf,
  uint8_t ifftFlag)
{
    const long N = S->fftLenRFFT;
    if (ifftFlag) {
        size_t avl = (N >> 1) - 1;
        const cplxf32 *px = (const cplxf32 *)p + 1;
        const cplxf32 *px_inv = (const cplxf32 *)(p + N) - 1;
        const float32_t *ptwd_re = S->ptwd_re + 1;
        const float32_t *ptwd_im = S->ptwd_im + 1;
        float32_t *py = pOut + 2;
        pOut[0] = (p[0] + p[1]) * 0.5f;
        pOut[1] = (p[0] - p[1]) * 0.5f;
        do {
            size_t vl = __riscv_vsetvl_e32m2(avl);

            vfloat32m2x2_t vx =
                __riscv_vlseg2e32_v_f32m2x2((const float32_t *)px, vl);
            px += vl;
            vfloat32m2_t vx_re = __riscv_vget_v_f32m2x2_f32m2(vx, 0);
            vfloat32m2_t vx_im = __riscv_vget_v_f32m2x2_f32m2(vx, 1);

            vfloat32m2x2_t vx_inv = __riscv_vlsseg2e32_v_f32m2x2(
                (const float32_t *)px_inv, -sizeof(cplxf32), vl);
            px_inv -= vl;
            vfloat32m2_t vx_inv_re = __riscv_vget_v_f32m2x2_f32m2(vx_inv, 0);
            vfloat32m2_t vx_inv_im = __riscv_vget_v_f32m2x2_f32m2(vx_inv, 1);
            vx_inv_im = __riscv_vfneg_v_f32m2(vx_inv_im, vl);

            vfloat32m2_t vy_re = __riscv_vfadd_vv_f32m2(vx_re, vx_inv_re, vl);
            vfloat32m2_t vy_im = __riscv_vfadd_vv_f32m2(vx_im, vx_inv_im, vl);

            vfloat32m2_t vtmp_re = __riscv_vfsub_vv_f32m2(vx_re, vx_inv_re, vl);
            vfloat32m2_t vtmp_im = __riscv_vfsub_vv_f32m2(vx_im, vx_inv_im, vl);

            vfloat32m2_t vtwd_re = __riscv_vle32_v_f32m2(ptwd_im, vl);
            ptwd_im += vl;
            vfloat32m2_t vtwd_im = __riscv_vle32_v_f32m2(ptwd_re, vl);
            ptwd_re += vl;

            vy_re = __riscv_vfadd_vv_f32m2(
                vy_re, __riscv_vfmul_vv_f32m2(vtmp_re, vtwd_re, vl), vl);
            vy_re = __riscv_vfsub_vv_f32m2(
                vy_re, __riscv_vfmul_vv_f32m2(vtmp_im, vtwd_im, vl), vl);

            vy_im = __riscv_vfadd_vv_f32m2(
                vy_im, __riscv_vfmul_vv_f32m2(vtmp_re, vtwd_im, vl), vl);
            vy_im = __riscv_vfadd_vv_f32m2(
                vy_im, __riscv_vfmul_vv_f32m2(vtmp_im, vtwd_re, vl), vl);

            vy_re = __riscv_vfmul_vf_f32m2(vy_re, 0.5f, vl);
            vy_im = __riscv_vfmul_vf_f32m2(vy_im, 0.5f, vl);

            vfloat32m2x2_t v_tuple;
            v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 0, vy_re);
            v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 1, vy_im);
            __riscv_vsseg2e32_v_f32m2x2(py, v_tuple, vl);

            py += 2 * vl;
            avl -= vl;
        } while (avl != 0);

        riscv_cfft_f32(&S->Sint, pOut, pOut, tmpbuf, ifftFlag);

    } else {
        float32_t *y = tmpbuf + 2 * N;
        riscv_cfft_f32(&S->Sint, p, y, tmpbuf, ifftFlag);

        // Y[0] = F[0] + G[0]
        const float32_t F0 = y[0];
        const float32_t G0 = y[1];
        pOut[0] = F0 + G0;
        pOut[1] = F0 - G0;

        size_t avl = (N >> 1) - 1;
        const float32_t *py = y + 2;
        const float32_t *py_inv = y + N - 2;
        float32_t *pout = pOut + 2;
        const float32_t *ptwd_re = S->ptwd_re + 1;
        const float32_t *ptwd_im = S->ptwd_im + 1;
        do {
            size_t vl = __riscv_vsetvl_e32m2(avl);

            // load vx_re, vx_im
            vfloat32m2x2_t v_tuple = __riscv_vlseg2e32_v_f32m2x2(py, vl);
            py += vl * 2;
            vfloat32m2_t vy_re = __riscv_vget_v_f32m2x2_f32m2(v_tuple, 0);
            vfloat32m2_t vy_im = __riscv_vget_v_f32m2x2_f32m2(v_tuple, 1);

            v_tuple = __riscv_vlsseg2e32_v_f32m2x2(py_inv,
                                                   -sizeof(float32_t) * 2, vl);
            py_inv -= vl * 2;
            vfloat32m2_t vy_inv_re = __riscv_vget_v_f32m2x2_f32m2(v_tuple, 0);
            vfloat32m2_t vy_inv_im = __riscv_vget_v_f32m2x2_f32m2(v_tuple, 1);

            // vFr
            vfloat32m2_t vFr_re = __riscv_vfadd_vv_f32m2(vy_re, vy_inv_re, vl);
            vfloat32m2_t vFr_im = __riscv_vfsub_vv_f32m2(vy_im, vy_inv_im, vl);

            // vGr
            vfloat32m2_t vGr_re = __riscv_vfadd_vv_f32m2(vy_inv_im, vy_im, vl);
            vfloat32m2_t vGr_im = __riscv_vfsub_vv_f32m2(vy_inv_re, vy_re, vl);

            vfloat32m2_t vtwd_re = __riscv_vle32_v_f32m2(ptwd_re, vl);
            ptwd_re += vl;
            vfloat32m2_t vtwd_im = __riscv_vle32_v_f32m2(ptwd_im, vl);
            ptwd_im += vl;

            vfloat32m2_t vout_re = __riscv_vfsub_vv_f32m2(
                __riscv_vfmul_vv_f32m2(vGr_re, vtwd_re, vl),
                __riscv_vfmul_vv_f32m2(vGr_im, vtwd_im, vl), vl);
            vfloat32m2_t vout_im = __riscv_vfadd_vv_f32m2(
                __riscv_vfmul_vv_f32m2(vGr_im, vtwd_re, vl),
                __riscv_vfmul_vv_f32m2(vGr_re, vtwd_im, vl), vl);

            vout_re = __riscv_vfadd_vv_f32m2(vFr_re, vout_re, vl);
            vout_im = __riscv_vfadd_vv_f32m2(vFr_im, vout_im, vl);

            vout_re = __riscv_vfmul_vf_f32m2(vout_re, 0.5f, vl);
            vout_im = __riscv_vfmul_vf_f32m2(vout_im, 0.5f, vl);

            v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 0, vout_re);
            v_tuple = __riscv_vset_v_f32m2_f32m2x2(v_tuple, 1, vout_im);
            __riscv_vsseg2e32_v_f32m2x2(pout, v_tuple, vl);
            pout += 2 * vl;

            avl -= vl;
        } while (avl != 0);
    }
}
#else
RISCV_DSP_ATTRIBUTE void riscv_rfft_fast_f32(
  const riscv_rfft_fast_instance_f32 * S,
  float32_t * p,
  float32_t * pOut,
  uint8_t ifftFlag)
{
   const riscv_cfft_instance_f32 * Sint = &(S->Sint);

   /* Calculation of Real FFT */
   if (ifftFlag)
   {
      /*  Real FFT compression */
      merge_rfft_f32(S, p, pOut);
      /* Complex radix-4 IFFT process */
      riscv_cfft_f32( Sint, pOut, ifftFlag, 1);
   }
   else
   {
      /* Calculation of RFFT of input */
      riscv_cfft_f32( Sint, p, ifftFlag, 1);

      /*  Real FFT extraction */
      stage_rfft_f32(S, p, pOut);
   }
}
#endif
/**
* @} end of RealFFTF32 group
*/
