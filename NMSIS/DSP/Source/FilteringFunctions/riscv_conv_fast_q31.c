/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_conv_fast_q31.c
 * Description:  Fast Q31 Convolution
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
  @addtogroup Conv
  @{
 */

/**
  @brief         Convolution of Q31 sequences (fast version).
  @param[in]     pSrcA      points to the first input sequence.
  @param[in]     srcALen    length of the first input sequence.
  @param[in]     pSrcB      points to the second input sequence.
  @param[in]     srcBLen    length of the second input sequence.
  @param[out]    pDst       points to the location where the output result is written.  Length srcALen+srcBLen-1.

  @par           Scaling and Overflow Behavior
                   This function is optimized for speed at the expense of fixed-point precision and overflow protection.
                   The result of each 1.31 x 1.31 multiplication is truncated to 2.30 format.
                   These intermediate results are accumulated in a 32-bit register in 2.30 format.
                   Finally, the accumulator is saturated and converted to a 1.31 result.
  @par
                   The fast version has the same overflow behavior as the standard version but provides less precision since it discards the low 32 bits of each multiplication result.
                   In order to avoid overflows completely the input signals must be scaled down.
                   Scale down the inputs by log2(min(srcALen, srcBLen)) (log2 is read as log to the base 2) times to avoid overflows,
                   as maximum of min(srcALen, srcBLen) number of additions are carried internally.
  @remark
                   Refer to \ref riscv_conv_q31() for a slower implementation of this function which uses 64-bit accumulation to provide higher precision.
 */

RISCV_DSP_ATTRIBUTE void riscv_conv_fast_q31(
  const q31_t * pSrcA,
        uint32_t srcALen,
  const q31_t * pSrcB,
        uint32_t srcBLen,
        q31_t * pDst)
{
  const q31_t *pIn1;                                   /* InputA pointer */
  const q31_t *pIn2;                                   /* InputB pointer */
        q31_t *pOut = pDst;                            /* Output pointer */
  const q31_t *px;                                     /* Intermediate inputA pointer */
  const q31_t *py;                                     /* Intermediate inputB pointer */
  const q31_t *pSrc1, *pSrc2;                          /* Intermediate pointers */
        q31_t sum, acc0, acc1, acc2, acc3;             /* Accumulators */
        q31_t x0, x1, x2, x3, c0;                      /* Temporary variables to hold state and coefficient values */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t j, k, count, blkCnt;                  /* Loop counters */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        q31_t tmp0, tmp1;
        q63_t px64, py64, sum64;
        q63_t acc064, acc164, acc264, acc364;
        q63_t x064, x164, x264, x364, c064;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

  /* The algorithm implementation is based on the lengths of the inputs. */
  /* srcB is always made to slide across srcA. */
  /* So srcBLen is always considered as shorter or equal to srcALen */
  if (srcALen >= srcBLen)
  {
    /* Initialization of inputA pointer */
    pIn1 = pSrcA;

    /* Initialization of inputB pointer */
    pIn2 = pSrcB;
  }
  else
  {
    /* Initialization of inputA pointer */
    pIn1 = pSrcB;

    /* Initialization of inputB pointer */
    pIn2 = pSrcA;

    /* srcBLen is always considered as shorter or equal to srcALen */
    j = srcBLen;
    srcBLen = srcALen;
    srcALen = j;
  }

  /* conv(x,y) at n = x[n] * y[0] + x[n-1] * y[1] + x[n-2] * y[2] + ...+ x[n-N+1] * y[N -1] */
  /* The function is internally
   * divided into three stages according to the number of multiplications that has to be
   * taken place between inputA samples and inputB samples. In the first stage of the
   * algorithm, the multiplications increase by one for every iteration.
   * In the second stage of the algorithm, srcBLen number of multiplications are done.
   * In the third stage of the algorithm, the multiplications decrease by one
   * for every iteration. */

  /* The algorithm is implemented in three stages.
     The loop counters of each stage is initiated here. */
  blockSize1 = srcBLen - 1U;
  blockSize2 = srcALen - (srcBLen - 1U);
  blockSize3 = blockSize1;

  /* --------------------------
   * Initializations of stage1
   * -------------------------*/

  /* sum = x[0] * y[0]
   * sum = x[0] * y[1] + x[1] * y[0]
   * ....
   * sum = x[0] * y[srcBlen - 1] + x[1] * y[srcBlen - 2] +...+ x[srcBLen - 1] * y[0]
   */

  /* In this stage the MAC operations are increased by 1 for every iteration.
     The count variable holds the number of MAC operations performed */
  count = 1U;

  /* Working pointer of inputA */
  px = pIn1;

  /* Working pointer of inputB */
  py = pIn2;


  /* ------------------------
   * Stage1 process
   * ----------------------*/

  /* The first stage starts here */
  while (blockSize1 > 0U)
  {
    /* Accumulator is made zero for every iteration */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      sum64 = 0;
#else
      sum = 0;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

    /* Apply loop unrolling and compute 4 MACs simultaneously. */
    k = count >> 2U;

    /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
     ** a second loop below computes MACs for the remaining 1 to 3 samples. */
    while (k > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      tmp0 = *py--;
      tmp1 = *py--;
      py64 = __RV_PKBB32(tmp1, tmp0);
      px64 = read_q31x2_ia ((q31_t **) &px);
      sum64 = __RV_KMADA32(sum64, px64, py64);

      tmp0 = *py--;
      tmp1 = *py--;
      py64 = __RV_PKBB32(tmp1, tmp0);
      px64 = read_q31x2_ia ((q31_t **) &px);
      sum64 = __RV_KMADA32(sum64, px64, py64);
#else
      /* x[0] * y[srcBLen - 1] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* x[1] * y[srcBLen - 2] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* x[2] * y[srcBLen - 3] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* x[3] * y[srcBLen - 4] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      k--;
    }

    /* If the count is not a multiple of 4, compute any remaining MACs here.
     ** No loop unrolling is used. */
    k = count & 0x3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    sum = (q31_t) (sum64 >> 32);
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* Decrement loop counter */
      k--;
    }

    /* Store the result in the accumulator in the destination buffer. */
    *pOut++ = sum << 1;

    /* Update the inputA and inputB pointers for next MAC calculation */
    py = pIn2 + count;
    px = pIn1;

    /* Increment MAC count */
    count++;

    /* Decrement loop counter */
    blockSize1--;
  }

  /* --------------------------
   * Initializations of stage2
   * ------------------------*/

  /* sum = x[0] * y[srcBLen-1] + x[1] * y[srcBLen-2] +...+ x[srcBLen-1] * y[0]
   * sum = x[1] * y[srcBLen-1] + x[2] * y[srcBLen-2] +...+ x[srcBLen] * y[0]
   * ....
   * sum = x[srcALen-srcBLen-2] * y[srcBLen-1] + x[srcALen] * y[srcBLen-2] +...+ x[srcALen-1] * y[0]
   */

  /* Working pointer of inputA */
  px = pIn1;

  /* Working pointer of inputB */
  pSrc2 = pIn2 + (srcBLen - 1U);
  py = pSrc2;

  /* count is index by which the pointer pIn1 to be incremented */
  count = 0U;

  /* -------------------
   * Stage2 process
   * ------------------*/

  /* Stage2 depends on srcBLen as in this stage srcBLen number of MACS are performed.
   * So, to loop unroll over blockSize2,
   * srcBLen should be greater than or equal to 4 */
  if (srcBLen >= 4U)
  {
    /* Loop unroll over blockSize2, by 4 */
    blkCnt = blockSize2 >> 2U;

    while (blkCnt > 0U)
    {
      /* Set all accumulators to zero */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        acc064 = 0;
        acc164 = 0;
        acc264 = 0;
        acc364 = 0;
#else
        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

      /* read x[0], x[1], x[2] samples */
      x0 = *px++;
      x1 = *px++;
      x2 = *px++;

      /* Apply loop unrolling and compute 4 MACs simultaneously. */
      k = srcBLen >> 2U;

      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
      do
      {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        tmp0 = *py--;
        tmp1 = *py--;
        c064 = __RV_PKBB32(tmp1, tmp0);

        x064 = __RV_PKBB32(x1, x0);
        acc064 = __RV_KMADA32(acc064, x064, c064);

        x164 = __RV_PKBB32(x2, x1);
        acc164 = __RV_KMADA32(acc164, x164, c064);

        /* Read x[3] sample */
        x3 = *px++;

        x264 = __RV_PKBB32(x3, x2);
        acc264 = __RV_KMADA32(acc264, x264, c064);

        /* Read x[4] sample */
        x0 = *px++;

        x364 = __RV_PKBB32(x0, x3);
        acc364 = __RV_KMADA32(acc364, x364, c064);

        tmp0 = *py--;
        tmp1 = *py--;
        c064 = __RV_PKBB32(tmp1, tmp0);

        x064 = __RV_PKBB32(x3, x2);
        acc064 = __RV_KMADA32(acc064, x064, c064);

        x164 = __RV_PKBB32(x0, x3);
        acc164 = __RV_KMADA32(acc164, x164, c064);

        /* Read x[5] sample */
        x1 = *px++;

        x264 = __RV_PKBB32(x1, x0);
        acc264 = __RV_KMADA32(acc264, x264, c064);

        /* Read x[6] sample */
        x2 = *px++;

        x364 = __RV_PKBB32(x2, x1);
        acc364 = __RV_KMADA32(acc364, x364, c064);
#else
        /* Read y[srcBLen - 1] sample */
        c0 = *py--;
        /* Read x[3] sample */
        x3 = *px++;

        /* Perform the multiply-accumulate */
        /* acc0 +=  x[0] * y[srcBLen - 1] */
        acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
        /* acc1 +=  x[1] * y[srcBLen - 1] */
        acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
        /* acc2 +=  x[2] * y[srcBLen - 1] */
        acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
        /* acc3 +=  x[3] * y[srcBLen - 1] */
        acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);


        /* Read y[srcBLen - 2] sample */
        c0 = *py--;
        /* Read x[4] sample */
        x0 = *px++;

        /* Perform the multiply-accumulate */
        /* acc0 +=  x[1] * y[srcBLen - 2] */
        acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x1 * c0)) >> 32);
        /* acc1 +=  x[2] * y[srcBLen - 2] */
        acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x2 * c0)) >> 32);
        /* acc2 +=  x[3] * y[srcBLen - 2] */
        acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x3 * c0)) >> 32);
        /* acc3 +=  x[4] * y[srcBLen - 2] */
        acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x0 * c0)) >> 32);


        /* Read y[srcBLen - 3] sample */
        c0 = *py--;
        /* Read x[5] sample */
        x1 = *px++;

        /* Perform the multiply-accumulates */
        /* acc0 +=  x[2] * y[srcBLen - 3] */
        acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x2 * c0)) >> 32);
        /* acc1 +=  x[3] * y[srcBLen - 3] */
        acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x3 * c0)) >> 32);
        /* acc2 +=  x[4] * y[srcBLen - 3] */
        acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x0 * c0)) >> 32);
        /* acc3 +=  x[5] * y[srcBLen - 3] */
        acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x1 * c0)) >> 32);


        /* Read y[srcBLen - 4] sample */
        c0 = *py--;
        /* Read x[6] sample */
        x2 = *px++;

        /* Perform the multiply-accumulates */
        /* acc0 +=  x[3] * y[srcBLen - 4] */
        acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x3 * c0)) >> 32);
        /* acc1 +=  x[4] * y[srcBLen - 4] */
        acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x0 * c0)) >> 32);
        /* acc2 +=  x[5] * y[srcBLen - 4] */
        acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x1 * c0)) >> 32);
        /* acc3 +=  x[6] * y[srcBLen - 4] */
        acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x2 * c0)) >> 32);


#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      } while (--k);

      /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = srcBLen & 0x3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      acc0 = (q31_t) (acc064 >> 32);
      acc1 = (q31_t) (acc164 >> 32);
      acc2 = (q31_t) (acc264 >> 32);
      acc3 = (q31_t) (acc364 >> 32);
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

      while (k > 0U)
      {
        /* Read y[srcBLen - 5] sample */
        c0 = *py--;
        /* Read x[7] sample */
        x3 = *px++;

        /* Perform the multiply-accumulates */
        /* acc0 +=  x[4] * y[srcBLen - 5] */
        acc0 = (q31_t) ((((q63_t) acc0 << 32) + ((q63_t) x0 * c0)) >> 32);
        /* acc1 +=  x[5] * y[srcBLen - 5] */
        acc1 = (q31_t) ((((q63_t) acc1 << 32) + ((q63_t) x1 * c0)) >> 32);
        /* acc2 +=  x[6] * y[srcBLen - 5] */
        acc2 = (q31_t) ((((q63_t) acc2 << 32) + ((q63_t) x2 * c0)) >> 32);
        /* acc3 +=  x[7] * y[srcBLen - 5] */
        acc3 = (q31_t) ((((q63_t) acc3 << 32) + ((q63_t) x3 * c0)) >> 32);

        /* Reuse the present samples for the next MAC */
        x0 = x1;
        x1 = x2;
        x2 = x3;

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q31_t) (acc0 << 1);
      *pOut++ = (q31_t) (acc1 << 1);
      *pOut++ = (q31_t) (acc2 << 1);
      *pOut++ = (q31_t) (acc3 << 1);

      /* Increment the pointer pIn1 index, count by 4 */
      count += 4U;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pSrc2;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* If the blockSize2 is not a multiple of 4, compute any remaining output samples here.
     ** No loop unrolling is used. */
    blkCnt = blockSize2 & 0x3U;

    while (blkCnt > 0U)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      /* Apply loop unrolling and compute 4 MACs simultaneously. */
      k = srcBLen >> 2U;

      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
      while (k > 0U)
      {
        /* Perform the multiply-accumulates */
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);

        /* Decrement loop counter */
        k--;
      }

      /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = srcBLen & 0x3U;

      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = sum << 1;

      /* Increment MAC count */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pSrc2;

      /* Decrement loop counter */
      blkCnt--;
    }
  }
  else
  {
    /* If the srcBLen is not a multiple of 4,
     * the blockSize2 loop cannot be unrolled by 4 */
    blkCnt = blockSize2;

    while (blkCnt > 0U)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      /* srcBLen number of MACS should be performed */
      k = srcBLen;

      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
        sum = (q31_t) ((((q63_t) sum << 32) +
                        ((q63_t) *px++ * (*py--))) >> 32);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = sum << 1;

      /* Increment MAC count */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pSrc2;

      /* Decrement loop counter */
      blkCnt--;
    }
  }


  /* --------------------------
   * Initializations of stage3
   * -------------------------*/

  /* sum += x[srcALen-srcBLen+1] * y[srcBLen-1] + x[srcALen-srcBLen+2] * y[srcBLen-2] +...+ x[srcALen-1] * y[1]
   * sum += x[srcALen-srcBLen+2] * y[srcBLen-1] + x[srcALen-srcBLen+3] * y[srcBLen-2] +...+ x[srcALen-1] * y[2]
   * ....
   * sum +=  x[srcALen-2] * y[srcBLen-1] + x[srcALen-1] * y[srcBLen-2]
   * sum +=  x[srcALen-1] * y[srcBLen-1]
   */

  /* In this stage the MAC operations are decreased by 1 for every iteration.
     The blockSize3 variable holds the number of MAC operations performed */

  /* Working pointer of inputA */
  pSrc1 = (pIn1 + srcALen) - (srcBLen - 1U);
  px = pSrc1;

  /* Working pointer of inputB */
  pSrc2 = pIn2 + (srcBLen - 1U);
  py = pSrc2;

  /* -------------------
   * Stage3 process
   * ------------------*/

  while (blockSize3 > 0U)
  {
    /* Accumulator is made zero for every iteration */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    sum64 = 0;
#else
    sum = 0;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

    /* Apply loop unrolling and compute 4 MACs simultaneously. */
    k = blockSize3 >> 2U;

    /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
     ** a second loop below computes MACs for the remaining 1 to 3 samples. */
    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      tmp0 = *py--;
      tmp1 = *py--;
      py64 = __RV_PKBB32(tmp1, tmp0);
      tmp0 = *px++;
      tmp1 = *px++;
      px64 = __RV_PKBB32(tmp1, tmp0);
      sum64 = __RV_KMADA32(sum64, px64, py64);

      tmp0 = *py--;
      tmp1 = *py--;
      py64 = __RV_PKBB32(tmp1, tmp0);
      tmp0 = *px++;
      tmp1 = *px++;
      px64 = __RV_PKBB32(tmp1, tmp0);
      sum64 = __RV_KMADA32(sum64, px64, py64);
#else
      /* sum += x[srcALen - srcBLen + 1] * y[srcBLen - 1] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* sum += x[srcALen - srcBLen + 2] * y[srcBLen - 2] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* sum += x[srcALen - srcBLen + 3] * y[srcBLen - 3] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* sum += x[srcALen - srcBLen + 4] * y[srcBLen - 4] */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      k--;
    }

    /* If the blockSize3 is not a multiple of 4, compute any remaining MACs here.
     ** No loop unrolling is used. */
    k = blockSize3 & 0x3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    sum = (q31_t) (sum64 >> 32);
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
      sum = (q31_t) ((((q63_t) sum << 32) +
                      ((q63_t) *px++ * (*py--))) >> 32);

      /* Decrement loop counter */
      k--;
    }

    /* Store the result in the accumulator in the destination buffer. */
    *pOut++ = sum << 1;

    /* Update the inputA and inputB pointers for next MAC calculation */
    px = ++pSrc1;
    py = pSrc2;

    /* Decrement loop counter */
    blockSize3--;
  }
}

/**
  @} end of Conv group
 */
