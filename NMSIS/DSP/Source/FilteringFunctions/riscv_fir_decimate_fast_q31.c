/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_fir_decimate_fast_q31.c
 * Description:  Fast Q31 FIR Decimator
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
  @brief         Processing function for the Q31 FIR decimator (fast variant).
  @param[in]     S          points to an instance of the Q31 FIR decimator structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process

  @par           Scaling and Overflow Behavior
                   This function is optimized for speed at the expense of fixed-point precision and overflow protection.
                   The result of each 1.31 x 1.31 multiplication is truncated to 2.30 format.
                   These intermediate results are added to a 2.30 accumulator.
                   Finally, the accumulator is saturated and converted to a 1.31 result.
                   The fast version has the same overflow behavior as the standard version and provides less precision since it discards the low 32 bits of each multiplication result.
                   In order to avoid overflows completely the input signal must be scaled down by log2(numTaps) bits (where log2 is read as log to the base 2).

  @remark
                   Refer to \ref riscv_fir_decimate_q31() for a slower implementation of this function which uses a 64-bit accumulator to provide higher precision.
                   Both the slow and the fast versions use the same instance structure.
                   Use function \ref riscv_fir_decimate_init_q31() to initialize the filter structure.
 */

RISCV_DSP_ATTRIBUTE void riscv_fir_decimate_fast_q31(
  const riscv_fir_decimate_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
  const q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCur;                              /* Points to the current sample of the state */
        q31_t *px0;                                    /* Temporary pointer for state buffer */
  const q31_t *pb;                                     /* Temporary pointer for coefficient buffer */
        q31_t x0, c0;                                  /* Temporary variables to hold state and coefficient values */
        q63_t acc0;                                    /* Accumulator */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;  /* Loop counters */

#if defined (RISCV_MATH_LOOPUNROLL)
        q31_t *px1, *px2, *px3;
        q31_t x1, x2, x3;
        q63_t acc1, acc2, acc3;
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
        q63_t x064, c064, x164, x264, x364;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
#endif

  /* S->pState buffer contains previous frame (numTaps - 1) samples */
  /* pStateCur points to the location where the new input data should be written */
  pStateCur = S->pState + (numTaps - 1U);

#if defined (RISCV_MATH_LOOPUNROLL) && !defined (RISCV_MATH_VECTOR)

    /* Loop unrolling: Compute 4 samples at a time */
  blkCnt = outBlockSize >> 2U;

  /* Samples loop unrolled by 4 */
  while (blkCnt > 0U)
  {
    /* Copy 4 * decimation factor number of new input samples into the state buffer */
    i = S->M * 4;

    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);

    /* Set accumulators to zero */
    acc0 = 0;
    acc1 = 0;
    acc2 = 0;
    acc3 = 0;

    /* Initialize state pointer for all the samples */
    px0 = pState;
    px1 = pState + S->M;
    px2 = pState + 2 * S->M;
    px3 = pState + 3 * S->M;

    /* Initialize coeff pointer */
    pb = pCoeffs;

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      x164 = read_q31x2_ia ((q31_t **) &px1);
      x264 = read_q31x2_ia ((q31_t **) &px2);
      x364 = read_q31x2_ia ((q31_t **) &px3);

      acc0 = __RV_KMADA32(acc0, x064, c064);
      acc1 = __RV_KMADA32(acc1, x164, c064);
      acc2 = __RV_KMADA32(acc2, x264, c064);
      acc3 = __RV_KMADA32(acc3, x364, c064);

      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      x164 = read_q31x2_ia ((q31_t **) &px1);
      x264 = read_q31x2_ia ((q31_t **) &px2);
      x364 = read_q31x2_ia ((q31_t **) &px3);

      acc0 = __RV_KMADA32(acc0, x064, c064);
      acc1 = __RV_KMADA32(acc1, x164, c064);
      acc2 = __RV_KMADA32(acc2, x264, c064);
      acc3 = __RV_KMADA32(acc3, x364, c064);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      x164 = read_q31x2_ia ((q31_t **) &px1);
      x264 = read_q31x2_ia ((q31_t **) &px2);
      x364 = read_q31x2_ia ((q31_t **) &px3);

      acc0 = __RV_DKMADA32(acc0, x064, c064);
      acc1 = __RV_DKMADA32(acc1, x164, c064);
      acc2 = __RV_DKMADA32(acc2, x264, c064);
      acc3 = __RV_DKMADA32(acc3, x364, c064);

      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      x164 = read_q31x2_ia ((q31_t **) &px1);
      x264 = read_q31x2_ia ((q31_t **) &px2);
      x364 = read_q31x2_ia ((q31_t **) &px3);

      acc0 = __RV_DKMADA32(acc0, x064, c064);
      acc1 = __RV_DKMADA32(acc1, x164, c064);
      acc2 = __RV_DKMADA32(acc2, x264, c064);
      acc3 = __RV_DKMADA32(acc3, x364, c064);
#else
      /* Read the b[numTaps-1] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-1] sample for acc0 */
      x0 = *(px0++);
      /* Read x[n-numTaps-1] sample for acc1 */
      x1 = *(px1++);
      /* Read x[n-numTaps-1] sample for acc2 */
      x2 = *(px2++);
      /* Read x[n-numTaps-1] sample for acc3 */
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
      acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
      acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
      acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);

      /* Read the b[numTaps-2] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-2] sample for acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
      acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
      acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
      acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);

      /* Read the b[numTaps-3] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-3] sample acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
      acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
      acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
      acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);

      /* Read the b[numTaps-4] coefficient */
      c0 = *(pb++);

      /* Read x[n-numTaps-4] sample acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
      acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
      acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
      acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);

#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
    acc0 = (q31_t)(acc0 >> 32);
    acc1 = (q31_t)(acc1 >> 32);
    acc2 = (q31_t)(acc2 >> 32);
    acc3 = (q31_t)(acc3 >> 32);
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
    tapCnt = numTaps & 0x3U;

    while (tapCnt > 0U)
    {
      /* Read coefficients */
      c0 = *(pb++);

      /* Fetch state variables for acc0, acc1, acc2, acc3 */
      x0 = *(px0++);
      x1 = *(px1++);
      x2 = *(px2++);
      x3 = *(px3++);

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
      acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
      acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
      acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M * 4;

    /* The result is in the accumulator, store in the destination buffer. */
    *pDst++ = (q31_t) (acc0 << 1);
    *pDst++ = (q31_t) (acc1 << 1);
    *pDst++ = (q31_t) (acc2 << 1);
    *pDst++ = (q31_t) (acc3 << 1);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining samples */
  blkCnt = outBlockSize & 0x3U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = outBlockSize;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (blkCnt > 0U)
  {
    /* Copy decimation factor number of new input samples into the state buffer */
    i = S->M;
#if defined (RISCV_MATH_VECTOR)
    uint32_t blkCnti = S->M;                              /* Loop counter */
    size_t l;

    for (; (l = __riscv_vsetvl_e32m8(blkCnti)) > 0; blkCnti -= l) {
      __riscv_vse32_v_i32m8(pStateCur, __riscv_vle32_v_i32m8(pSrc, l), l);
      pSrc += l;
      pStateCur += l;
    }
#else
    do
    {
      *pStateCur++ = *pSrc++;

    } while (--i);
#endif /* defined (RISCV_MATH_VECTOR) */
    /* Set accumulator to zero */
    acc0 = 0;

    /* Initialize state pointer */
    px0 = pState;

    /* Initialize coeff pointer */
    pb = pCoeffs;

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      acc0 = __RV_KMADA32(acc0, x064, c064);

      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      acc0 = __RV_KMADA32(acc0, x064, c064);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      acc0 = __RV_DKMADA32(acc0, x064, c064);

      c064 = read_q31x2_ia ((q31_t **) &pb);
      x064 = read_q31x2_ia ((q31_t **) &px0);
      acc0 = __RV_DKMADA32(acc0, x064, c064);
#else
      /* Read the b[numTaps-1] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-1] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);

      /* Read the b[numTaps-2] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-2] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);

      /* Read the b[numTaps-3] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-3] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);

      /* Read the b[numTaps-4] coefficient */
      c0 = *pb++;

      /* Read x[n-numTaps-4] sample */
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);

#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
    acc0 = (q31_t)(acc0 >> 32);
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
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
      x0 = *px0++;

      /* Perform the multiply-accumulate */
      acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Advance the state pointer by the decimation factor
     * to process the next group of decimation factor number samples */
    pState = pState + S->M;

    /* The result is in the accumulator, store in the destination buffer. */
    *pDst++ = (q31_t) (acc0 << 1);

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the satrt of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the state buffer */
  pStateCur = S->pState;

#if defined (RISCV_MATH_LOOPUNROLL) && !defined (RISCV_MATH_VECTOR)

  /* Loop unrolling: Compute 4 taps at a time */
  tapCnt = (numTaps - 1U) >> 2U;

  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

  /* Loop unrolling: Compute remaining taps */
  tapCnt = (numTaps - 1U) & 0x03U;

#else

  /* Initialize tapCnt with number of taps */
  tapCnt = (numTaps - 1U);

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */
#if defined (RISCV_MATH_VECTOR)
  uint32_t blkCnti = tapCnt;                              /* Loop counter */
  size_t l;

  for (; (l = __riscv_vsetvl_e32m8(blkCnti)) > 0; blkCnti -= l) {
    __riscv_vse32_v_i32m8(pStateCur, __riscv_vle32_v_i32m8(pState, l), l);
    pState += l;
    pStateCur += l;
  }
#else
  /* Copy data */
  while (tapCnt > 0U)
  {
    *pStateCur++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }
#endif /* defined (RISCV_MATH_VECTOR) */
}

/**
  @} end of FIR_decimate group
 */
