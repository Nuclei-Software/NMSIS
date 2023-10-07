/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fir_decimate_q15.c
 * Description:  Q15 FIR Decimator
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
  @addtogroup FIR_decimate
  @{
 */

/**
  @brief         Processing function for the Q15 FIR decimator.
  @param[in]     S          points to an instance of the Q15 FIR decimator structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of input samples to process per call
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 64-bit internal accumulator.
                   Both coefficients and state variables are represented in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.
                   After all additions have been performed, the accumulator is truncated to 34.15 format by discarding low 15 bits.
                   Lastly, the accumulator is saturated to yield a result in 1.15 format.

 @remark
                   Refer to \ref riscv_fir_decimate_fast_q15() for a faster but less precise implementation of this function.
 */

#if defined (RISCV_MATH_DSP) && !defined (RISCV_MATH_VECTOR)

void riscv_fir_decimate_q15(
  const riscv_fir_decimate_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
        q15_t *pState = S->pState;                     /* State pointer */
  const q15_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q15_t *pStateCur;                              /* Points to the current sample of the state */
        q15_t *px;                                     /* Temporary pointer for state buffer */
  const q15_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q31_t x0, x1, c0;                              /* Temporary variables to hold state and coefficient values */
        q63_t sum0;                                    /* Accumulators */
        q63_t acc0, acc1;
        q15_t *px0, *px1;
        uint32_t blkCntN3;
        uint32_t numTaps = S->numTaps;                 /* Number of taps */
        uint32_t i, blkCnt, tapCnt, outBlockSize = blockSize / S->M;  /* Loop counters */

#if defined (RISCV_MATH_LOOPUNROLL)
        q31_t c1;                                      /* Temporary variables to hold state and coefficient values */
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)
        q63_t x064, x164, c064;
        q31_t tmp1, tmp2;
#endif /* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */
#endif /* defined (RISCV_MATH_LOOPUNROLL) */

  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (numTaps - 1U);

  /* Total number of output samples to be computed */
  blkCnt = outBlockSize / 2;
  blkCntN3 = outBlockSize - (2 * blkCnt);

  while (blkCnt > 0U)
  {
    /* Copy 2 * decimation factor number of new input samples into the state buffer */
    i = S->M * 2;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    acc0 = 0;
    acc1 = 0;

    /* Initialize state pointer for all the samples */
    px0 = pState;
    px1 = pState + S->M;

    /* Initialize coeff pointer */
    pb = pCoeffs;

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
#if __RISCV_XLEN == 64
      /* Read the b[numTaps-1], b[numTaps-2], b[numTaps-3] and b[numTaps-4] coefficients */
      c064 = read_q15x4_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-1], x[n-numTaps-2], x[n-numTaps-2] and x[n-numTaps-3] sample */
      tmp1 = read_q15x2_ia ((q15_t **) &px0);
      tmp2 = read_q15x2_ia ((q15_t **) &px0);
      x064 = __RV_PKBB32(tmp2, tmp1);
      tmp1 = read_q15x2_ia ((q15_t **) &px1);
      tmp2 = read_q15x2_ia ((q15_t **) &px1);
      x164 = __RV_PKBB32(tmp2, tmp1);

      acc0 = __SMLALD(x064, c064, acc0);
      acc1 = __SMLALD(x164, c064, acc1);

#else
#if defined (NUCLEI_DSP_N3)
      /* Read the b[numTaps-1], b[numTaps-2], b[numTaps-3] and b[numTaps-4] coefficients */
      c064 = read_q15x4_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-1], x[n-numTaps-2], x[n-numTaps-2] and x[n-numTaps-3] sample */
      x064 = read_q15x4_ia ((q15_t **) &px0);
      x164 = read_q15x4_ia ((q15_t **) &px1);

      acc0 = __RV_DSMALDA(acc0, x064, c064);
      acc1 = __RV_DSMALDA(acc1, x164, c064);

#else
      /* Read the b[numTaps-1] and b[numTaps-2] coefficients */
      c0 = read_q15x2_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-1] and x[n-numTaps-2]sample */
      x0 = read_q15x2_ia (&px0);
      x1 = read_q15x2_ia (&px1);

      /* Perform the multiply-accumulate */
      acc0 = __SMLALD(x0, c0, acc0);
      acc1 = __SMLALD(x1, c0, acc1);

      /* Read the b[numTaps-3] and b[numTaps-4] coefficient */
      c0 = read_q15x2_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-2] and x[n-numTaps-3] sample */
      x0 = read_q15x2_ia (&px0);
      x1 = read_q15x2_ia (&px1);

      /* Perform the multiply-accumulate */
      acc0 = __SMLALD(x0, c0, acc0);
      acc1 = __SMLALD(x1, c0, acc1);
#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps & 0x3U;

#else

    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch state variables for acc0, acc1 */
      x0 = *px0++;
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 = __SMLALD(x0, c0, acc0);
      acc1 = __SMLALD(x1, c0, acc1);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M * 2;

    /* Store filter output, smlad returns the values in 2.14 format */
    /* so downsacle by 15 to get output in 1.15 */
    *pDst++ = (q15_t) (__SSAT((acc0 >> 15), 16));
    *pDst++ = (q15_t) (__SSAT((acc1 >> 15), 16));

    /* Decrement loop counter */
    blkCnt--;
  }

  while (blkCntN3 > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
    i = S->M;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulator to zero */
    sum0 = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
#if __RISCV_XLEN == 64
      /* Read the b[numTaps-1], b[numTaps-2], b[numTaps-3] and b[numTaps-4] coefficients */
      c064 = read_q15x4_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-1], x[n-numTaps-2], x[n-numTaps-2] and x[n-numTaps-3] sample */
      tmp1 = read_q15x2_ia ((q15_t **) &px0);
      tmp2 = read_q15x2_ia ((q15_t **) &px0);
      x064 = __RV_PKBB32(tmp2, tmp1);

      sum0 = __SMLALD(x064, c064, sum0);
#else
#if defined (NUCLEI_DSP_N3)
      /* Read the b[numTaps-1], b[numTaps-2], b[numTaps-3] and b[numTaps-4] coefficients */
      c064 = read_q15x4_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-1], x[n-numTaps-2], x[n-numTaps-2] and x[n-numTaps-3] sample */
      x064 = read_q15x4_ia ((q15_t **) &px);

      sum0 = __RV_DSMALDA(sum0, x064, c064);
#else
      /* Read the b[numTaps-1] and b[numTaps-2] coefficients */
      c0 = read_q15x2_ia ((q15_t **) &pb);

      /* Read x[n-numTaps-1] and x[n-numTaps-2] sample */
      x0 = read_q15x2_ia (&px);

      /* Read the b[numTaps-3] and b[numTaps-4] coefficients */
      c1 = read_q15x2_ia ((q15_t **) &pb);

      /* Perform the multiply-accumulate */
      sum0 = __SMLALD(x0, c0, sum0);

      /* Read x[n-numTaps-2] and x[n-numTaps-3] sample */
      x0 = read_q15x2_ia (&px);

      /* Perform the multiply-accumulate */
      sum0 = __SMLALD(x0, c1, sum0);

#endif /* defined (NUCLEI_DSP_N3) */
#endif /* __RISCV_XLEN == 64 */
      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps & 0x3U;

#else

    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch 1 state variable */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 = __SMLALD(x0, c0, sum0);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* Store filter output, smlad returns the values in 2.14 format */
    /* so downsacle by 15 to get output in 1.15 */
    *pDst++ = (q15_t) (__SSAT((sum0 >> 15), 16));

    /* Decrement loop counter */
    blkCntN3--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the satrt of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;
  i = (numTaps - 1U) >> 2U;

  /* copy data */
  while (i > 0U)
  {
    write_q15x2_ia (&pStateCur, read_q15x2_ia (&pState));
    write_q15x2_ia (&pStateCur, read_q15x2_ia (&pState));

    /* Decrement loop counter */
    i--;
  }

  i = (numTaps - 1U) & 0x03U;

  /* Copy data */
  while (i > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    i--;
  }

}

#else /* #if defined (RISCV_MATH_DSP) */

void riscv_fir_decimate_q15(
  const riscv_fir_decimate_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
        q15_t *pState = S->pState;                     /* State pointer */
  const q15_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q15_t *pStateCur;                              /* Points to the current sample of the state */
        q15_t *px;                                     /* Temporary pointer for state buffer */
  const q15_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q15_t x0, x1, c0;                              /* Temporary variables to hold state and coefficient values */
        q63_t sum0;                                    /* Accumulators */
        q63_t acc0, acc1;
        q15_t *px0, *px1;
        uint32_t blkCntN3;
        uint32_t numTaps = S->numTaps;                 /* Number of taps */
        uint32_t i, blkCnt, tapCnt, outBlockSize = blockSize / S->M;  /* Loop counters */


  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (numTaps - 1U);

  /* Total number of output samples to be computed */
  blkCnt = outBlockSize / 2;
  blkCntN3 = outBlockSize - (2 * blkCnt);

  while (blkCnt > 0U)
  {
#if defined (RISCV_MATH_VECTOR)
    uint32_t blkCnti = S->M * 2;                              /* Loop counter */
    size_t l;

    for (; (l = __riscv_vsetvl_e16m8(blkCnti)) > 0; blkCnti -= l) {
      __riscv_vse16_v_i16m8(pStateCur, __riscv_vle16_v_i16m8(pSrc, l), l);
      pSrc += l;
      pStateCur += l;
    }
#else
    /* Copy 2 * decimation factor number of new input samples into the state buffer */
    i = S->M * 2;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);
#endif /* defined (RISCV_MATH_VECTOR) */
    /* Set accumulator to zero */
    acc0 = 0;
    acc1 = 0;

    /* Initialize state pointer */
    px0 = pState;
    px1 = pState + S->M;

    /* Initialize coeff pointer */
    pb = pCoeffs;

#if defined (RISCV_MATH_LOOPUNROLL) && !defined (RISCV_MATH_VECTOR)

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
      /* Read the Read b[numTaps-1] coefficients */
      c0 = *pb++;

      /* Read x[n-numTaps-1] for sample 0 and for sample 1 */
      x0 = *px0++;
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;

      /* Read the b[numTaps-2] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-2] for sample 0 and sample 1 */
      x0 = *px0++;
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;

      /* Read the b[numTaps-3] coefficients */
      c0 = *pb++;

      /* Read x[n-numTaps-3] for sample 0 and sample 1 */
      x0 = *px0++;
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;

      /* Read the b[numTaps-4] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-4] for sample 0 and sample 1 */
      x0 = *px0++;
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps & 0x3U;

#else

    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    uint32_t blkCntb;
    // size_t l;
    vint16m4_t va1m4, vb1m4, vb2m4;
    vint64m1_t vtemp00m1, vtemp01m1;
    blkCntb = numTaps;                               /* Loop counter */

    l = __riscv_vsetvl_e64m1(1);
    vtemp00m1 = __riscv_vmv_v_x_i64m1(0, l);
    vtemp01m1 = __riscv_vmv_v_x_i64m1(0, l);
    for (; (l = __riscv_vsetvl_e16m4(blkCntb)) > 0; blkCntb -= l) {
      va1m4 = __riscv_vle16_v_i16m4(pb, l);
      pb += l;
      vb1m4 = __riscv_vle16_v_i16m4(px0, l);
      px0 += l;
      vb2m4 = __riscv_vle16_v_i16m4(px1, l);
      px1 += l;
      vtemp00m1 = __riscv_vwredsum_vs_i32m8_i64m1(__riscv_vwmul_vv_i32m8(va1m4, vb1m4, l), vtemp00m1, l);
      vtemp01m1 = __riscv_vwredsum_vs_i32m8_i64m1(__riscv_vwmul_vv_i32m8(va1m4, vb2m4, l), vtemp01m1, l);
    }
    acc0 += (q63_t)__riscv_vmv_x_s_i64m1_i64(vtemp00m1);
    acc1 += (q63_t)__riscv_vmv_x_s_i64m1_i64(vtemp01m1);
#else
    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch 1 state variable */
      x0 = *px0++;
      x1 = *px1++;

      /* Perform the multiply-accumulate */
      acc0 += x0 * c0;
      acc1 += x1 * c0;

      /* Decrement the loop counter */
      tapCnt--;
    }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M * 2;

    /* Store filter output, smlad returns the values in 2.14 format */
    /* so downsacle by 15 to get output in 1.15 */

    *pDst++ = (q15_t) (__SSAT((acc0 >> 15), 16));
    *pDst++ = (q15_t) (__SSAT((acc1 >> 15), 16));

    /* Decrement loop counter */
    blkCnt--;
  }

  while (blkCntN3 > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
#if defined (RISCV_MATH_VECTOR)
    uint32_t blkCnti = S->M * 2;                              /* Loop counter */
    size_t l;

    for (; (l = __riscv_vsetvl_e16m8(blkCnti)) > 0; blkCnti -= l) {
      __riscv_vse16_v_i16m8(pStateCur, __riscv_vle16_v_i16m8(pSrc, l), l);
      pSrc += l;
      pStateCur += l;
    }
#else
    i = S->M;
    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);
#endif /* defined (RISCV_MATH_VECTOR) */
    /* Set accumulator to zero */
    sum0 = 0;

    /* Initialize state pointer */
    px = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
      /* Read the b[numTaps-1] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-1] sample */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Read the b[numTaps-2] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-2] sample */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Read the b[numTaps-3] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-3] sample */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Read the b[numTaps-4] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-4] sample */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps & 0x3U;

#else

    /* Initialize tapCnt with number of taps */
    tapCnt = numTaps;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *pb++;

      /* Fetch 1 state variable */
      x0 = *px++;

      /* Perform the multiply-accumulate */
      sum0 += x0 * c0;

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* Store filter output, smlad returns the values in 2.14 format */
    /* so downsacle by 15 to get output in 1.15 */
    *pDst++ = (q15_t) (__SSAT((sum0 >> 15), 16));

    /* Decrement loop counter */
    blkCntN3--;
  }

  /* Processing is complete.
   ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.
   ** This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;

  i = (numTaps - 1U) >> 2U;

  /* copy data */
  while (i > 0U)
  {
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    i--;
  }

  i = (numTaps - 1U) & 0x03U;

  /* copy data */
  while (i > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    i--;
  }
}

#endif /* #if defined (RISCV_MATH_DSP) */
/**
  @} end of FIR_decimate group
 */
