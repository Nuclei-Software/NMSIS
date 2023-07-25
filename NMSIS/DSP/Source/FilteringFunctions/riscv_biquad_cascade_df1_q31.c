/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_biquad_cascade_df1_q31.c
 * Description:  Processing function for the Q31 Biquad cascade filter
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

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup BiquadCascadeDF1
  @{
 */

/**
  @brief         Processing function for the Q31 Biquad cascade filter.
  @param[in]     S         points to an instance of the Q31 Biquad cascade structure
  @param[in]     pSrc      points to the block of input data
  @param[out]    pDst      points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by 2 bits and lie in the range [-0.25 +0.25).
                   After all 5 multiply-accumulates are performed, the 2.62 accumulator is shifted by <code>postShift</code> bits and the result truncated to
                   1.31 format by discarding the low 32 bits.
  @remark
                   Refer to \ref riscv_biquad_cascade_df1_fast_q31() for a faster but less precise implementation of this filter.
 */

#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
void riscv_biquad_cascade_df1_q31(
  const riscv_biquad_casd_df1_inst_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
  const q31_t *pIn = pSrc;                             /* Source pointer */
        q31_t *pOut = pDst;                            /* Destination pointer */
        q31_t *pState = S->pState;                     /* pState pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q63_t acc, acc_1, acc_2;                         /* Accumulator */
        q63_t in;                                      /* Temporary variable to hold input value */
        q63_t out;                                     /* Temporary variable to hold output value */
        q63_t b0;                                      /* Temporary variable to hold b0 value */
        q63_t b1, a1;                                  /* Filter coefficients */
        q63_t state_in, state_out;                     /* Filter state variables */
        q31_t Xn;                                      /* Temporary input */
        uint32_t uShift = ((uint32_t) S->postShift + 1U);
        uint32_t lShift = 32U - uShift;                /* Shift to be applied to the output */
        uint32_t sample, stage = S->numStages;         /* Loop counters */
        q63_t out1, out2;
        q31_t Yn;

  do
  {
    /* Read the b0 and 0 coefficients using SIMD  */
    b0 = *pCoeffs++;

    /* Read the b1 and b2 coefficients using SIMD */
    b1 = __RV_PKBB32(*(pCoeffs + 1), *pCoeffs);
    pCoeffs += 2;

    /* Read the a1 and a2 coefficients using SIMD */
    a1 = __RV_PKBB32(*(pCoeffs + 1), *pCoeffs);
    pCoeffs += 2;

    /* Read the input state values from the state buffer:  x[n-1], x[n-2] */
    state_in = read_q31x2_ia((q31_t **) &pState);

    /* Read the output state values from the state buffer:  y[n-1], y[n-2] */
    state_out = read_q31x2_da((q31_t **) &pState);

    /* Apply loop unrolling and compute 2 output values simultaneously. */
    /*      The variable acc hold output values that are being computed:
     *
     *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     *    acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     */
    sample = blockSize >> 1U;

    /* First part of the processing with loop unrolling.  Compute 2 outputs at a time.
     ** a second loop below computes the remaining 1 sample. */
    while (sample > 0U)
    {

      /* Read the input */
      in = read_q31x2_ia((q31_t **) &pIn);

      /* out =  b0 * x[n] + 0 * 0 */
      acc_1 = __RV_KMDA32(b0, in);

      /* acc +=  b1 * x[n-1] +  b2 * x[n-2] + out */
      acc_1 = __RV_SMAR64(acc_1, b1, state_in);

      /* acc +=  a1 * y[n-1] +  a2 * y[n-2] */
      acc_1 = __RV_SMAR64(acc_1, a1, state_out);

      /* The result is converted to 1.31  */

      acc_2 = __RV_KMXDA32(b0, in);

      acc_1 = acc_1 >> lShift;

      state_in = __RV_PKBB32(state_in, in);
      state_out = __RV_PKBB32(state_out, acc_1);

      /* acc +=  b1 * x[n-1] +  b2 * x[n-2] + out */
      acc_2 = __RV_SMAR64(acc_2, b1, state_in);
      /* acc +=  a1 * y[n-1] + a2 * y[n-2] */
      acc_2 = __RV_SMAR64(acc_2, a1, state_out);

      /* The result is converted to 1.31  */

      acc_2 = acc_2 >> lShift;

      state_in = __RV_PKBT32(state_in, in);

      write_q31x2_ia(&pOut, __RV_PKBB32(acc_2, state_out));
      state_out = __RV_PKBB32(state_out, acc_2);

      sample--;

    }

    /* If the blockSize is not a multiple of 2, compute any remaining output samples here.
     ** No loop unrolling is used. */

    if ((blockSize & 0x1U) != 0U)
    {
      /* Read the input */
      in = *pIn++;

      /* out =  b0 * x[n] + 0 * 0 */
      out = __RV_KMDA32(b0, in);

      /* acc =  b1 * x[n-1] + b2 * x[n-2] + out */
      acc = __RV_SMAR64(out, b1, state_in);
      /* acc +=  a1 * y[n-1] + a2 * y[n-2] */
      acc = __RV_SMAR64(acc, a1, state_out);

      acc = acc >> lShift;

      /* Store the output in the destination buffer. */
      *pOut++ = (q31_t) acc;

      state_in = __RV_PKBB32(state_in, in);
      state_out = __RV_PKBB32(state_out, acc);
    }

    /* The first stage goes from the input wire to the output wire.  */
    /* Subsequent numStages occur in-place in the output wire  */
    pIn = pDst;

    /* Reset the output pointer */
    pOut = pDst;

    /* Store the updated state variables back into the state array */
    write_q31x2_ia(&pState, state_in);
    write_q31x2_ia(&pState, state_out);

    /* decrement loop counter */
    stage--;

  } while (stage > 0U);

}

#else

void riscv_biquad_cascade_df1_q31(
  const riscv_biquad_casd_df1_inst_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
  const q31_t *pIn = pSrc;                             /* Source pointer */
        q31_t *pOut = pDst;                            /* Destination pointer */
        q31_t *pState = S->pState;                     /* pState pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q63_t acc;                                     /* Accumulator */
        q31_t b0, b1, b2, a1, a2;                      /* Filter coefficients */
        q31_t Xn1, Xn2, Yn1, Yn2;                      /* Filter pState variables */
        q31_t Xn;                                      /* Temporary input */
        uint32_t uShift = ((uint32_t) S->postShift + 1U);
        uint32_t lShift = 32U - uShift;                /* Shift to be applied to the output */
        uint32_t sample, stage = S->numStages;         /* Loop counters */

#if defined (RISCV_MATH_LOOPUNROLL)
        q31_t acc_l, acc_h;                            /* temporary output variables */
#endif

  do
  {
    /* Reading the coefficients */
    b0 = *pCoeffs++;
    b1 = *pCoeffs++;
    b2 = *pCoeffs++;
    a1 = *pCoeffs++;
    a2 = *pCoeffs++;

    /* Reading the pState values */
    Xn1 = pState[0];
    Xn2 = pState[1];
    Yn1 = pState[2];
    Yn2 = pState[3];

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Apply loop unrolling and compute 4 output values simultaneously. */
    /* Variable acc hold output values that are being computed:
     *
     * acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     */

    /* Loop unrolling: Compute 4 outputs at a time */
    sample = blockSize >> 2U;

    while (sample > 0U)
    {
      /* Read the first input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = ((q63_t) b0 * Xn) + ((q63_t) b1 * Xn1) + ((q63_t) b2 * Xn2) + ((q63_t) a1 * Yn1) + ((q63_t) a2 * Yn2);

      /* The result is converted to 1.31 , Yn2 variable is reused */
      acc_l = (acc      ) & 0xffffffff; /* Calc lower part of acc */
      acc_h = (acc >> 32) & 0xffffffff; /* Calc upper part of acc */

      /* Apply shift for lower part of acc and upper part of acc */
      Yn2 = (uint32_t) acc_l >> lShift | acc_h << uShift;

      /* Store output in destination buffer. */
      *pOut++ = Yn2;

      /* Read the second input */
      Xn2 = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = ((q63_t) b0 * Xn2) + ((q63_t) b1 * Xn) + ((q63_t) b2 * Xn1) + ((q63_t) a1 * Yn2) + ((q63_t) a2 * Yn1);

      /* The result is converted to 1.31, Yn1 variable is reused  */
      acc_l = (acc      ) & 0xffffffff; /* Calc lower part of acc */
      acc_h = (acc >> 32) & 0xffffffff; /* Calc upper part of acc */

      /* Apply shift for lower part of acc and upper part of acc */
      Yn1 = (uint32_t) acc_l >> lShift | acc_h << uShift;

      /* Store output in destination buffer. */
      *pOut++ = Yn1;

      /* Read the third input */
      Xn1 = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = ((q63_t) b0 * Xn1) + ((q63_t) b1 * Xn2) + ((q63_t) b2 * Xn) + ((q63_t) a1 * Yn1) + ((q63_t) a2 * Yn2);

      /* The result is converted to 1.31, Yn2 variable is reused  */
      acc_l = (acc      ) & 0xffffffff; /* Calc lower part of acc */
      acc_h = (acc >> 32) & 0xffffffff; /* Calc upper part of acc */

      /* Apply shift for lower part of acc and upper part of acc */
      Yn2 = (uint32_t) acc_l >> lShift | acc_h << uShift;

      /* Store output in destination buffer. */
      *pOut++ = Yn2;

      /* Read the forth input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = ((q63_t) b0 * Xn) + ((q63_t) b1 * Xn1) + ((q63_t) b2 * Xn2) + ((q63_t) a1 * Yn2) + ((q63_t) a2 * Yn1);

      /* The result is converted to 1.31, Yn1 variable is reused  */
      acc_l = (acc      ) & 0xffffffff; /* Calc lower part of acc */
      acc_h = (acc >> 32) & 0xffffffff; /* Calc upper part of acc */

      /* Apply shift for lower part of acc and upper part of acc */
      Yn1 = (uint32_t) acc_l >> lShift | acc_h << uShift;

      /* Store output in destination buffer. */
      *pOut++ = Yn1;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */
      Xn2 = Xn1;
      Xn1 = Xn;

      /* decrement loop counter */
      sample--;
    }

    /* Loop unrolling: Compute remaining outputs */
    sample = blockSize & 0x3U;

#else

    /* Initialize blkCnt with number of samples */
    sample = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (sample > 0U)
    {
      /* Read the input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = ((q63_t) b0 * Xn) + ((q63_t) b1 * Xn1) + ((q63_t) b2 * Xn2) + ((q63_t) a1 * Yn1) + ((q63_t) a2 * Yn2);

      /* The result is converted to 1.31  */
      acc = acc >> lShift;

      /* Store output in destination buffer. */
      *pOut++ = (q31_t) acc;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */
      Xn2 = Xn1;
      Xn1 = Xn;
      Yn2 = Yn1;
      Yn1 = (q31_t) acc;

      /* decrement loop counter */
      sample--;
    }

    /* Store the updated state variables back into the pState array */
    *pState++ = Xn1;
    *pState++ = Xn2;
    *pState++ = Yn1;
    *pState++ = Yn2;

    /* The first stage goes from the input buffer to the output buffer. */
    /* Subsequent numStages occur in-place in the output buffer */
    pIn = pDst;

    /* Reset output pointer */
    pOut = pDst;

    /* decrement loop counter */
    stage--;

  } while (stage > 0U);

}

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
/**
  @} end of BiquadCascadeDF1 group
 */
