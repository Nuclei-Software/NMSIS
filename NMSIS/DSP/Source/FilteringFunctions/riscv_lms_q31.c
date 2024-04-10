/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_lms_q31.c
 * Description:  Processing function for the Q31 LMS filter
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
  @addtogroup LMS
  @{
 */

/**
  @brief         Processing function for Q31 LMS filter.
  @param[in]     S         points to an instance of the Q31 LMS filter structure.
  @param[in]     pSrc      points to the block of input data.
  @param[in]     pRef      points to the block of reference data.
  @param[out]    pOut      points to the block of output data.
  @param[out]    pErr      points to the block of error data.
  @param[in]     blockSize number of samples to process.
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate
                   multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clips.
                   In order to avoid overflows completely the input signal must be scaled down by
                   log2(numTaps) bits.
                   The reference signal should not be scaled down.
                   After all multiply-accumulates are performed, the 2.62 accumulator is shifted
                   and saturated to 1.31 format to yield the final result.
                   The output signal and error signal are in 1.31 format.
 @par
                   In this filter, filter coefficients are updated for each sample and
                   the updation of filter cofficients are saturted.
 */

void riscv_lms_q31(
  const riscv_lms_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pRef,
        q31_t * pOut,
        q31_t * pErr,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
        q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCurnt;                            /* Points to the current sample of the state */
        q31_t *px, *pb;                                /* Temporary pointers for state and coefficient buffers */
        q31_t mu = S->mu;                              /* Adaptive factor */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t tapCnt, blkCnt;                       /* Loop counters */
        q63_t acc;                                     /* Accumulator */
        q31_t e = 0;                                   /* Error of data sample */
        q31_t alpha;                                   /* Intermediate constant for taps update */
        q31_t coef;                                    /* Temporary variable for coef */
        q31_t acc_l, acc_h;                            /* Temporary input */
        uint32_t uShift = ((uint32_t) S->postShift + 1U);
        uint32_t lShift = 32U - uShift;                /*  Shift to be applied to the output */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) || defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
        q31_t tmp0, tmp1;
        q63_t px64, pb64;
        q63_t alpha64, coef64;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N2) || defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

  /* S->pState points to buffer which contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = &(S->pState[(numTaps - 1U)]);

  /* initialise loop count */
  blkCnt = blockSize;

  while (blkCnt > 0U)
  {
    /* Copy the new input sample into the state buffer */
    *pStateCurnt++ = *pSrc++;

    /* Initialize pState pointer */
    px = pState;

    /* Initialize coefficient pointer */
    pb = pCoeffs;

    /* Set the accumulator to zero */
    acc = 0;

#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    uint32_t vblkCnt = numTaps;
    size_t l;
    vint32m4_t vx, vy;
    vint64m1_t temp00m1;
    l = __riscv_vsetvl_e64m1(1);
    temp00m1 = __riscv_vmv_v_x_i64m1(0, l);
    for (; (l = __riscv_vsetvl_e32m4(vblkCnt)) > 0; vblkCnt -= l) {
      vx = __riscv_vle32_v_i32m4(px, l);
      px += l;
      vy = __riscv_vle32_v_i32m4(pb, l);
      pb += l;
      temp00m1 = __riscv_vredsum_vs_i64m8_i64m1(__riscv_vwmul_vv_i64m8(vx, vy, l), temp00m1, l);
    }
    acc += __riscv_vmv_x_s_i64m1_i64(temp00m1);
#else
#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time. */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      tmp0 = *px++;
      tmp1 = *px++;
      acc = __RV_KMADA32(acc, __RV_PKBB32(tmp1, tmp0), read_q31x2_ia(&pb));
      tmp0 = *px++;
      tmp1 = *px++;
      acc = __RV_KMADA32(acc, __RV_PKBB32(tmp1, tmp0), read_q31x2_ia(&pb));
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
      acc = __RV_DKMADA32(acc, read_q31x2_ia (&px), read_q31x2_ia (&pb));
      acc = __RV_DKMADA32(acc, read_q31x2_ia (&px), read_q31x2_ia (&pb));
#else
      /* Perform the multiply-accumulate */
      /* acc +=  b[N] * x[n-N] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* acc +=  b[N-1] * x[n-N-1] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* acc +=  b[N-2] * x[n-N-2] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* acc +=  b[N-3] * x[n-N-3] */
      acc += ((q63_t) (*px++)) * (*pb++);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps & 0x3U;

#else

    /* Initialize tapCnt with number of samples */
    tapCnt = numTaps;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* Decrement the loop counter */
      tapCnt--;
    }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
    /* Converting the result to 1.31 format */
    /* Calc lower part of acc */
    acc_l = acc & 0xffffffff;

    /* Calc upper part of acc */
    acc_h = (acc >> 32) & 0xffffffff;

    acc = (uint32_t) acc_l >> lShift | acc_h << uShift;

    /* Store the result from accumulator into the destination buffer. */
    *pOut++ = (q31_t) acc;

    /* Compute and store error */
    e = *pRef++ - (q31_t) acc;
    *pErr++ = e;

    /* Compute alpha i.e. intermediate constant for taps update */
    alpha = (q31_t) (((q63_t) e * mu) >> 31);
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    alpha64 = __RV_PKBB32(alpha, alpha);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
    alpha64 = __RV_DPACK32(alpha, alpha);
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */


    /* Initialize pState pointer */
    /* Advance state pointer by 1 for the next sample */
    px = pState++;

    /* Initialize coefficient pointer */
    pb = pCoeffs;
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    vblkCnt = numTaps;
    for (; (l = __riscv_vsetvl_e32m4(vblkCnt)) > 0; vblkCnt -= l) {
      vx = __riscv_vle32_v_i32m4(px, l);
      px += l;
      __riscv_vse32_v_i32m4(pb, __riscv_vnclip_wx_i32m4(__riscv_vwadd_vv_i64m8(__riscv_vsll_vx_i32m4(__riscv_vnclip_wx_i32m4(__riscv_vwmul_vx_i64m8(vx, alpha, l), 32, __RISCV_VXRM_RNU, l), 1, l), __riscv_vle32_v_i32m4(pb, l), l), 0, __RISCV_VXRM_RNU, l), l);
      pb += l;
    }
#else
#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time. */
    tapCnt = numTaps >> 2U;

    /* Update filter coefficients */
    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */

      /* coef is in 2.30 format */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      px64 = read_q31x2_ia(&px);
      coef64 = __RV_SMMUL(alpha64, px64);
      coef64 = __RV_KSLRA32(coef64, 1);

      pb64 = read_q31x2(pb);
      write_q31x2_ia(&pb, __RV_KADD32(pb64, coef64));

      px64 = read_q31x2_ia(&px);
      coef64 = __RV_SMMUL(alpha64, px64);
      coef64 = __RV_KSLRA32(coef64, 1);

      pb64 = read_q31x2(pb);
      write_q31x2_ia(&pb, __RV_KADD32(pb64, coef64));
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2)
      px64 = read_q31x2_ia(&px);
      coef64 = __RV_DSMMUL(alpha64, px64);
      coef64 = __RV_DKSLRA32(coef64, 1);

      pb64 = read_q31x2(pb);
      write_q31x2_ia(&pb, __RV_DKADD32(pb64, coef64));

      px64 = read_q31x2_ia(&px);
      coef64 = __RV_DSMMUL(alpha64, px64);
      coef64 = __RV_DKSLRA32(coef64, 1);

      pb64 = read_q31x2(pb);
      write_q31x2_ia(&pb, __RV_DKADD32(pb64, coef64));
#else
      coef = (q31_t) (((q63_t) alpha * (*px++)) >> (32));
      /* get coef in 1.31 format by left shifting */
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      /* update coefficient buffer to next coefficient */
      pb++;

      coef = (q31_t) (((q63_t) alpha * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      coef = (q31_t) (((q63_t) alpha * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      coef = (q31_t) (((q63_t) alpha * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N2) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps & 0x3U;

#else

    /* Initialize tapCnt with number of samples */
    tapCnt = numTaps;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */
      coef = (q31_t) (((q63_t) alpha * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      /* Decrement loop counter */
      tapCnt--;
    }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the start of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the pState buffer */
  pStateCurnt = S->pState;

  /* copy data */
#if defined (RISCV_MATH_VECTOR)
  uint32_t vblkCnt = (numTaps - 1U);
  size_t l;
  for (; (l = __riscv_vsetvl_e32m8(vblkCnt)) > 0; vblkCnt -= l) {
    __riscv_vse32_v_i32m8(pStateCurnt, __riscv_vle32_v_i32m8(pState, l), l);
    pState += l;
    pStateCurnt += l;
  }
#else
#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 taps at a time. */
  tapCnt = (numTaps - 1U) >> 2U;

  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

  /* Loop unrolling: Compute remaining taps */
  tapCnt = (numTaps - 1U) & 0x3U;

#else

  /* Initialize tapCnt with number of samples */
  tapCnt = (numTaps - 1U);

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) */
}

/**
  @} end of LMS group
 */
