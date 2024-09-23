/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fir_q31.c
 * Description:  Q31 FIR filter processing function
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
  @addtogroup FIR
  @{
 */

/**
  @brief         Processing function for Q31 FIR filter.
  @param[in]     S          points to an instance of the Q31 FIR filter structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by log2(numTaps) bits.
                   After all multiply-accumulates are performed, the 2.62 accumulator is right shifted by 31 bits and saturated to 1.31 format to yield the final result.

 @remark
                   Refer to \ref riscv_fir_fast_q31() for a faster but less precise implementation of this filter.
 */
RISCV_DSP_ATTRIBUTE void riscv_fir_q31(
  const riscv_fir_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCurnt;                            /* Points to the current sample of the state */
        q31_t *px;                                     /* Temporary pointer for state buffer */
  const q31_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q63_t acc0;                                    /* Accumulator */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t i, tapCnt, blkCnt;                    /* Loop counters */

#if defined (RISCV_MATH_LOOPUNROLL)
        q63_t acc1, acc2;                              /* Accumulators */
        q31_t x0, x1, x2;                              /* Temporary variables to hold state values */
        q31_t c0;                                      /* Temporary variable to hold coefficient value */
#endif

  /* S->pState points to state array which contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = &(S->pState[(numTaps - 1U)]);

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    uint32_t j;
    size_t l;
    vint32m4_t vx;
    vint64m8_t vres0m8;
    q31_t *pOut = pDst;
    /* Copy samples into state buffer */
    riscv_copy_q31(pSrc, pStateCurnt, blockSize);
    for (i = blockSize; i > 0; i -= l)
    {
      l = __riscv_vsetvl_e32m4(i);
      vx = __riscv_vle32_v_i32m4(pState, l);
      pState += l;
      vres0m8 = __riscv_vmv_v_x_i64m8(0, l);
      for (j = 0; j < numTaps; j++) {
        vres0m8 = __riscv_vwmacc_vx_i64m8(vres0m8, *(pCoeffs + j), vx, l);
        vx = __riscv_vslide1down_vx_i32m4(vx, *(pState + j), l);
      }
      __riscv_vse32_v_i32m4(pOut, __riscv_vnsra_wx_i32m4(vres0m8, 31, l), l);
      pOut += l;
    }
    /* Processing is complete.
       Now copy the last numTaps - 1 samples to the start of the state buffer.
       This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    /* Copy data */
    riscv_copy_q31(pState, pStateCurnt, numTaps - 1);
#else

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 output values simultaneously.
   * The variables acc0 ... acc3 hold output values that are being computed:
   *
   *    acc0 =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0]
   *    acc1 =  b[numTaps-1] * x[n-numTaps]   + b[numTaps-2] * x[n-numTaps-1] + b[numTaps-3] * x[n-numTaps-2] +...+ b[0] * x[1]
   *    acc2 =  b[numTaps-1] * x[n-numTaps+1] + b[numTaps-2] * x[n-numTaps]   + b[numTaps-3] * x[n-numTaps-1] +...+ b[0] * x[2]
   *    acc3 =  b[numTaps-1] * x[n-numTaps+2] + b[numTaps-2] * x[n-numTaps+1] + b[numTaps-3] * x[n-numTaps]   +...+ b[0] * x[3]
   */

  blkCnt = blockSize / 3;

  while (blkCnt > 0U)
  {
    /* Copy 3 new input samples into the state buffer. */
    *pStateCurnt++ = *pSrc++;
    *pStateCurnt++ = *pSrc++;
    *pStateCurnt++ = *pSrc++;

    /* Set all accumulators to zero */
    acc0 = 0;
    acc1 = 0;
    acc2 = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize coefficient pointer */
    pb = pCoeffs;

    /* Read the first 2 samples from the state buffer: x[n-numTaps], x[n-numTaps-1] */
    x0 = *px++;
    x1 = *px++;

    /* Loop unrolling: process 3 taps at a time. */
    tapCnt = numTaps / 3;

    while (tapCnt > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      q63_t c064 = read_q31x2_ia ((q31_t **) &pb);
      x2 = *(px++);

      acc0 = __RV_KMADA32(acc0, __RV_PKBB32(x1, x0), c064);
      acc1 = __RV_KMADA32(acc1, __RV_PKBB32(x2, x1), c064);
      x0 = *(px++);
      acc2 = __RV_KMADA32(acc2, __RV_PKBB32(x0, x2), c064);
      c0 = *(pb++);
      x1 = *(px++);
#else
      /* Read the b[numTaps] coefficient */
      c0 = *pb;

      /* Read x[n-numTaps-2] sample */
      x2 = *(px++);

      /* Perform the multiply-accumulates */
      acc0 += ((q63_t) x0 * c0);
      acc1 += ((q63_t) x1 * c0);
      acc2 += ((q63_t) x2 * c0);

      /* Read the coefficient and state */
      c0 = *(pb + 1U);
      x0 = *(px++);

      /* Perform the multiply-accumulates */
      acc0 += ((q63_t) x1 * c0);
      acc1 += ((q63_t) x2 * c0);
      acc2 += ((q63_t) x0 * c0);

      /* Read the coefficient and state */
      c0 = *(pb + 2U);
      x1 = *(px++);

      /* update coefficient pointer */
      pb += 3U;
#endif /*  defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

      /* Perform the multiply-accumulates */
      acc0 += ((q63_t) x2 * c0);
      acc1 += ((q63_t) x0 * c0);
      acc2 += ((q63_t) x1 * c0);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    tapCnt = numTaps % 0x3U;

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *(pb++);

      /* Fetch 1 state variable */
      x2 = *(px++);

      /* Perform the multiply-accumulates */
      acc0 += ((q63_t) x0 * c0);
      acc1 += ((q63_t) x1 * c0);
      acc2 += ((q63_t) x2 * c0);

      /* Reuse the present sample states for next sample */
      x0 = x1;
      x1 = x2;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by 3 to process the next group of 3 samples */
    pState = pState + 3;

    /* The result is in 2.30 format. Convert to 1.31 and store in destination buffer. */
    *pDst++ = (q31_t) (acc0 >> 31U);
    *pDst++ = (q31_t) (acc1 >> 31U);
    *pDst++ = (q31_t) (acc2 >> 31U);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining output samples */
  blkCnt = blockSize % 0x3U;

#else

  /* Initialize blkCnt with number of taps */
  blkCnt = blockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* Copy one sample at a time into state buffer */
    *pStateCurnt++ = *pSrc++;

    /* Set the accumulator to zero */
    acc0 = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize Coefficient pointer */
    pb = pCoeffs;

    i = numTaps;

    /* Perform the multiply-accumulates */
    do
    {
      /* acc =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0] */
      acc0 += (q63_t) *px++ * *pb++;

      i--;
    } while (i > 0U);

    /* Result is in 2.62 format. Convert to 1.31 and store in destination buffer. */
    *pDst++ = (q31_t) (acc0 >> 31U);

    /* Advance state pointer by 1 for the next sample */
    pState = pState + 1U;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the start of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCurnt = S->pState;

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 taps at a time */
  tapCnt = (numTaps - 1U) >> 2U;

  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

  /* Calculate remaining number of copies */
  tapCnt = (numTaps - 1U) & 0x3U;

#else

  /* Initialize tapCnt with number of taps */
  tapCnt = (numTaps - 1U);

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  /* Copy remaining data */
  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
}

/**
  @} end of FIR group
 */
