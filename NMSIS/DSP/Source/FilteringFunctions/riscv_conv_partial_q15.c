/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_conv_partial_q15.c
 * Description:  Partial convolution of Q15 sequences
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
  @addtogroup PartialConv
  @{
 */

/**
  @brief         Partial convolution of Q15 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written
  @param[in]     firstIndex is the first output sample to start with
  @param[in]     numPoints  is the number of output points to be computed
  @return        execution status
                   - \ref RISCV_MATH_SUCCESS        : Operation successful
                   - \ref RISCV_MATH_ARGUMENT_ERROR : requested subset is not in the range [0 srcALen+srcBLen-2]

  @remark
                   Refer to \ref riscv_conv_partial_fast_q15() for a faster but less precise version of this function.
  @remark
                   Refer to \ref riscv_conv_partial_opt_q15() for a faster implementation of this function using scratch buffers.
 */

RISCV_DSP_ATTRIBUTE riscv_status riscv_conv_partial_q15(
  const q15_t * pSrcA,
        uint32_t srcALen,
  const q15_t * pSrcB,
        uint32_t srcBLen,
        q15_t * pDst,
        uint32_t firstIndex,
        uint32_t numPoints)
{

#if defined (RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)

  const q15_t *pIn1;                                   /* InputA pointer */
  const q15_t *pIn2;                                   /* InputB pointer */
        q15_t *pOut = pDst;                            /* Output pointer */
        q63_t sum, acc0, acc1, acc2, acc3;             /* Accumulator */
  const q15_t *px;                                     /* Intermediate inputA pointer */
  const q15_t *py;                                     /* Intermediate inputB pointer */
  const q15_t *pSrc1, *pSrc2;                          /* Intermediate pointers */
        q31_t x0, x1, x2, x3, c0;                      /* Temporary input variables to hold state and coefficient values */
        int32_t blockSize1, blockSize2, blockSize3;    /* Loop counters */
        uint32_t j, k, count, blkCnt, check;
        riscv_status status;                             /* Status of Partial convolution */
        q15_t tmp0, tmp1, tmp2, tmp3;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        q63_t px64, py64;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

  /* Check for range of output samples to be calculated */
  if ((firstIndex + numPoints) > ((srcALen + (srcBLen - 1U))))
  {
    /* Set status as RISCV_MATH_ARGUMENT_ERROR */
    status = RISCV_MATH_ARGUMENT_ERROR;
  }
  else
  {
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

    /* Conditions to check which loopCounter holds
     * the first and last indices of the output samples to be calculated. */
    check = firstIndex + numPoints;
    blockSize3 = ((int32_t)check > (int32_t)srcALen) ? (int32_t)check - (int32_t)srcALen : 0;
    blockSize3 = ((int32_t)firstIndex > (int32_t)srcALen - 1) ? blockSize3 - (int32_t)firstIndex + (int32_t)srcALen : blockSize3;
    blockSize1 = ((int32_t) srcBLen - 1) - (int32_t) firstIndex;
    blockSize1 = (blockSize1 > 0) ? ((check > (srcBLen - 1U)) ? blockSize1 :  (int32_t)numPoints) : 0;
    blockSize2 = (int32_t) check - ((blockSize3 + blockSize1) + (int32_t) firstIndex);
    blockSize2 = (blockSize2 > 0) ? blockSize2 : 0;

    /* conv(x,y) at n = x[n] * y[0] + x[n-1] * y[1] + x[n-2] * y[2] + ...+ x[n-N+1] * y[N -1] */
    /* The function is internally
     * divided into three stages according to the number of multiplications that has to be
     * taken place between inputA samples and inputB samples. In the first stage of the
     * algorithm, the multiplications increase by one for every iteration.
     * In the second stage of the algorithm, srcBLen number of multiplications are done.
     * In the third stage of the algorithm, the multiplications decrease by one
     * for every iteration. */

    /* Set the output pointer to point to the firstIndex
     * of the output sample to be calculated. */
    pOut = pDst + firstIndex;

    /* --------------------------
     * Initializations of stage1
     * -------------------------*/

    /* sum = x[0] * y[0]
     * sum = x[0] * y[1] + x[1] * y[0]
     * ....
     * sum = x[0] * y[srcBlen - 1] + x[1] * y[srcBlen - 2] +...+ x[srcBLen - 1] * y[0]
     */

    /* In this stage the MAC operations are increased by 1 for every iteration.
       The count variable holds the number of MAC operations performed.
       Since the partial convolution starts from firstIndex
       Number of Macs to be performed is firstIndex + 1 */
    count = 1U + firstIndex;

    /* Working pointer of inputA */
    px = pIn1;

    /* Working pointer of inputB */
    pSrc2 = pIn2 + firstIndex;
    py = pSrc2;

    /* ------------------------
     * Stage1 process
     * ----------------------*/

    /* For loop unrolling by 4, this stage is divided into two. */
    /* First part of this stage computes the MAC operations less than 4 */
    /* Second part of this stage computes the MAC operations greater than or equal to 4 */
#if defined (RISCV_MATH_VECTOR)
  while (blockSize1 > 0U)
  {
    /* Accumulator is made zero for every iteration */
    sum = 0;
    uint32_t vblkCnt = count;                               /* Loop counter */
    size_t l;
    vint16m4_t vx, vy;
    vint32m1_t temp00m1;
    ptrdiff_t bstride = -2;
    l = __riscv_vsetvl_e32m1(vblkCnt);
    temp00m1 = __riscv_vmv_v_x_i32m1(0, l);
    for (; (l = __riscv_vsetvl_e16m4(vblkCnt)) > 0; vblkCnt -= l) {
      vx = __riscv_vle16_v_i16m4(px, l);
      px += l;
      vy = __riscv_vlse16_v_i16m4(py, bstride, l);
      py -= l;
      temp00m1 = __riscv_vredsum_vs_i32m8_i32m1(__riscv_vwmul_vv_i32m8(vx, vy, l), temp00m1, l);
    }
    sum += __riscv_vmv_x_s_i32m1_i32(temp00m1);

    /* Store the result in the accumulator in the destination buffer. */
    *pOut++ = (q15_t) (__SSAT((sum >> 15), 16));

    /* Update the inputA and inputB pointers for next MAC calculation */
    py = pIn2 + count;
    px = pIn1;

    /* Increment MAC count */
    count++;

    /* Decrement loop counter */
    blockSize1--;
  }
#else
    /* The first part of the stage starts here */
    while ((count < 4U) && (blockSize1 > 0))
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      /* Loop over number of MAC operations between
       * inputA samples and inputB samples */
      k = count;

      while (k > 0U)
      {
        /* Perform the multiply-accumulates */
        sum = __SMLALD(*px++, *py--, sum);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q15_t) (__SSAT((sum >> 15), 16));

      /* Update the inputA and inputB pointers for next MAC calculation */
      py = ++pSrc2;
      px = pIn1;

      /* Increment MAC count */
      count++;

      /* Decrement loop counter */
      blockSize1--;
    }

    /* The second part of the stage starts here */
    /* The internal loop, over count, is unrolled by 4 */
    /* To, read the last two inputB samples using SIMD:
     * y[srcBLen] and y[srcBLen-1] coefficients, py is decremented by 1 */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 32)
    py = py - 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 32) */

    while (blockSize1 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      /* Apply loop unrolling and compute 4 MACs simultaneously. */
      k = count >> 2U;

      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
         a second loop below computes MACs for the remaining 1 to 3 samples. */
      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        px64 = read_q15x4_ia ((q15_t **) &px);
        tmp0 = *py--;
        tmp1 = *py--;
        tmp2 = *py--;
        tmp3 = *py--;
        py64 = __RV_PKBB32(__RV_PKBB16(tmp3, tmp2), __RV_PKBB16(tmp1, tmp0));
        sum = __SMLALD(px64, py64, sum);
#else
        /* x[0], x[1] are multiplied with y[srcBLen - 1], y[srcBLen - 2] respectively */
        sum = __SMLALDX(read_q15x2_ia ((q15_t **) &px), read_q15x2_da ((q15_t **) &py), sum);
        /* x[2], x[3] are multiplied with y[srcBLen - 3], y[srcBLen - 4] respectively */
        sum = __SMLALDX(read_q15x2_ia ((q15_t **) &px), read_q15x2_da ((q15_t **) &py), sum);

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        /* Decrement loop counter */
        k--;
      }

      /* For the next MAC operations, the pointer py is used without SIMD
       * So, py is incremented by 1 */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 32)
      py = py + 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 32) */

      /* If the count is not a multiple of 4, compute any remaining MACs here.
         No loop unrolling is used. */
      k = count & 0x3U;

      while (k > 0U)
      {
        /* Perform the multiply-accumulates */
        sum = __SMLALD(*px++, *py--, sum);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q15_t) (__SSAT((sum >> 15), 16));

      /* Update the inputA and inputB pointers for next MAC calculation */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      py = ++pSrc2;
#else
      py = ++pSrc2 - 1U;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      px = pIn1;

      /* Increment MAC count */
      count++;

      /* Decrement loop counter */
      blockSize1--;
    }
#endif /* defined (RISCV_MATH_VECTOR) */
    /* --------------------------
     * Initializations of stage2
     * ------------------------*/

    /* sum = x[0] * y[srcBLen-1] + x[1] * y[srcBLen-2] +...+ x[srcBLen-1] * y[0]
     * sum = x[1] * y[srcBLen-1] + x[2] * y[srcBLen-2] +...+ x[srcBLen] * y[0]
     * ....
     * sum = x[srcALen-srcBLen-2] * y[srcBLen-1] + x[srcALen] * y[srcBLen-2] +...+ x[srcALen-1] * y[0]
     */

    /* Working pointer of inputA */
    if ((int32_t)firstIndex - (int32_t)srcBLen + 1 > 0)
    {
      pSrc1 = pIn1 + firstIndex - srcBLen + 1;
    }
    else
    {
      pSrc1 = pIn1;
    }
    px = pSrc1;

    /* Working pointer of inputB */
    pSrc2 = pIn2 + (srcBLen - 1U);
    py = pSrc2;

    /* count is the index by which the pointer pIn1 to be incremented */
    count = 0U;

    /* -------------------
     * Stage2 process
     * ------------------*/
#if defined (RISCV_MATH_VECTOR)
    blkCnt = blockSize2;

    while (blkCnt > 0U)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      uint32_t vblkCnt = srcBLen;                               /* Loop counter */
      size_t l;
      vint16m4_t vx, vy;
      vint32m1_t temp00m1;
      ptrdiff_t bstride = -2;
      l = __riscv_vsetvl_e32m1(vblkCnt);
      temp00m1 = __riscv_vmv_v_x_i32m1(0, l);
      for (; (l = __riscv_vsetvl_e16m4(vblkCnt)) > 0; vblkCnt -= l) {
        vx = __riscv_vle16_v_i16m4(px, l);
        px += l;
        vy = __riscv_vlse16_v_i16m4(py, bstride, l);
        py -= l;
        temp00m1 = __riscv_vredsum_vs_i32m8_i32m1(__riscv_vwmul_vv_i32m8(vx, vy, l), temp00m1, l);
      }
      sum += __riscv_vmv_x_s_i32m1_i32(temp00m1);
      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q15_t) (__SSAT(sum >> 15, 16));

      /* Increment the MAC count */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pSrc2;

      /* Decrement the loop counter */
      blkCnt--;
    }
#else
    /* Stage2 depends on srcBLen as in this stage srcBLen number of MACS are performed.
     * So, to loop unroll over blockSize2,
     * srcBLen should be greater than or equal to 4 */
    if (srcBLen >= 4U)
    {
      /* Loop unrolling: Compute 4 outputs at a time */
      blkCnt = ((uint32_t) blockSize2 >> 2U);

      while (blkCnt > 0U)
      {
        py = py - 1U;

        /* Set all accumulators to zero */
        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;


        /* read x[0], x[1] samples */
        x0 = read_q15x2 ((q15_t *) px);
        /* read x[1], x[2] samples */
        x1 = __RV_PKBT16(*(px + 2), x0);
        px += 2U;


        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        k = srcBLen >> 2U;

        /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
         ** a second loop below computes MACs for the remaining 1 to 3 samples. */
        do
        {
          /* Read the last two inputB samples using SIMD:
           * y[srcBLen - 1] and y[srcBLen - 2] */
          c0 = read_q15x2_da ((q15_t **) &py);

          /* acc0 +=  x[0] * y[srcBLen - 1] + x[1] * y[srcBLen - 2] */
          acc0 = __SMLALDX(x0, c0, acc0);

          /* acc1 +=  x[1] * y[srcBLen - 1] + x[2] * y[srcBLen - 2] */
          acc1 = __SMLALDX(x1, c0, acc1);

          /* Read x[2], x[3] */
          x2 = read_q15x2 ((q15_t *) px);

          /* Read x[3], x[4] */
          x3 = __RV_PKBT16(*(px + 2), x2);

          /* acc2 +=  x[2] * y[srcBLen - 1] + x[3] * y[srcBLen - 2] */
          acc2 = __SMLALDX(x2, c0, acc2);

          /* acc3 +=  x[3] * y[srcBLen - 1] + x[4] * y[srcBLen - 2] */
          acc3 = __SMLALDX(x3, c0, acc3);

          /* Read y[srcBLen - 3] and y[srcBLen - 4] */
          c0 = read_q15x2_da ((q15_t **) &py);

          /* acc0 +=  x[2] * y[srcBLen - 3] + x[3] * y[srcBLen - 4] */
          acc0 = __SMLALDX(x2, c0, acc0);

          /* acc1 +=  x[3] * y[srcBLen - 3] + x[4] * y[srcBLen - 4] */
          acc1 = __SMLALDX(x3, c0, acc1);

          /* Read x[4], x[5] */
          x0 = read_q15x2 ((q15_t *) px + 2);

          /* Read x[5], x[6] */
          x1 = __RV_PKBT16(*(px + 4), x0);
          px += 4U;

          /* acc2 +=  x[4] * y[srcBLen - 3] + x[5] * y[srcBLen - 4] */
          acc2 = __SMLALDX(x0, c0, acc2);

          /* acc3 +=  x[5] * y[srcBLen - 3] + x[6] * y[srcBLen - 4] */
          acc3 = __SMLALDX(x1, c0, acc3);

        } while (--k);

        /* For the next MAC operations, SIMD is not used
         * So, the 16 bit pointer if inputB, py is updated */

        /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        k = srcBLen & 0x3U;

        if (k == 1U)
        {
          /* Read y[srcBLen - 5] */
          c0 = *(py + 1);
          c0 = c0 & 0x0000FFFF;

          /* Read x[7] */
          x3 = read_q15x2 ((q15_t *) px);
          px++;

          /* Perform the multiply-accumulate */
          acc0 = __SMLALD (x0, c0, acc0);
          acc1 = __SMLALD (x1, c0, acc1);
          acc2 = __SMLALDX(x1, c0, acc2);
          acc3 = __SMLALDX(x3, c0, acc3);
        }

        if (k == 2U)
        {
          /* Read y[srcBLen - 5], y[srcBLen - 6] */
          c0 = read_q15x2 ((q15_t *) py);

          /* Read x[7], x[8] */
          x3 = read_q15x2 ((q15_t *) px);

          /* Read x[9] */
          x2 = __RV_PKBT16(*(px + 2), x3);
          px += 2U;

          /* Perform the multiply-accumulate */
          acc0 = __SMLALDX(x0, c0, acc0);
          acc1 = __SMLALDX(x1, c0, acc1);
          acc2 = __SMLALDX(x3, c0, acc2);
          acc3 = __SMLALDX(x2, c0, acc3);
        }

        if (k == 3U)
        {
          /* Read y[srcBLen - 5], y[srcBLen - 6] */
          c0 = read_q15x2 ((q15_t *) py);

          /* Read x[7], x[8] */
          x3 = read_q15x2 ((q15_t *) px);

          /* Read x[9] */
          x2 = __RV_PKBT16(*(px + 2), x3);

          /* Perform the multiply-accumulate */
          acc0 = __SMLALDX(x0, c0, acc0);
          acc1 = __SMLALDX(x1, c0, acc1);
          acc2 = __SMLALDX(x3, c0, acc2);
          acc3 = __SMLALDX(x2, c0, acc3);

          c0 = *(py - 1);
          c0 = c0 & 0x0000FFFF;

          /* Read x[10] */
          x3 =  read_q15x2 ((q15_t *) px + 2);
          px += 3U;

          /* Perform the multiply-accumulates */
          acc0 = __SMLALDX(x1, c0, acc0);
          acc1 = __SMLALD (x2, c0, acc1);
          acc2 = __SMLALDX(x2, c0, acc2);
          acc3 = __SMLALDX(x3, c0, acc3);
        }

        /* Store the results in the accumulators in the destination buffer. */
        {
          int32_t sat0 = __SSAT((acc0 >> 15), 16);
          int32_t sat1 = __SSAT((acc1 >> 15), 16);
          int32_t sat2 = __SSAT((acc2 >> 15), 16);
          int32_t sat3 = __SSAT((acc3 >> 15), 16);
          write_q15x2_ia (&pOut, __PKHBT(sat0, sat1, 16));
          write_q15x2_ia (&pOut, __PKHBT(sat2, sat3, 16));
        }

        /* Increment the pointer pIn1 index, count by 4 */
        count += 4U;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement loop counter */
        blkCnt--;
      }

      /* If the blockSize2 is not a multiple of 4, compute any remaining output samples here.
         No loop unrolling is used. */
      blkCnt = (uint32_t) blockSize2 & 0x3U;

      while (blkCnt > 0U)
      {
        /* Accumulator is made zero for every iteration */
        sum = 0;

        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        k = srcBLen >> 2U;

        /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
           a second loop below computes MACs for the remaining 1 to 3 samples. */
        while (k > 0U)
        {
          /* Perform the multiply-accumulates */
          sum += (q63_t) ((q31_t) *px++ * *py--);
          sum += (q63_t) ((q31_t) *px++ * *py--);
          sum += (q63_t) ((q31_t) *px++ * *py--);
          sum += (q63_t) ((q31_t) *px++ * *py--);

          /* Decrement loop counter */
          k--;
        }

        /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
         ** No loop unrolling is used. */
        k = srcBLen & 0x3U;

        while (k > 0U)
        {
          /* Perform the multiply-accumulate */
          sum += (q63_t) ((q31_t) *px++ * *py--);

          /* Decrement loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = (q15_t) (__SSAT(sum >> 15, 16));

        /* Increment the pointer pIn1 index, count by 1 */
        count++;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement loop counter */
        blkCnt--;
      }
    }
    else
    {
      /* If the srcBLen is not a multiple of 4,
       * the blockSize2 loop cannot be unrolled by 4 */
      blkCnt = (uint32_t) blockSize2;

      while (blkCnt > 0U)
      {
        /* Accumulator is made zero for every iteration */
        sum = 0;

        /* srcBLen number of MACS should be performed */
        k = srcBLen;

        while (k > 0U)
        {
          /* Perform the multiply-accumulate */
          sum += (q63_t) ((q31_t) *px++ * *py--);

          /* Decrement the loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = (q15_t) (__SSAT(sum >> 15, 16));

        /* Increment the MAC count */
        count++;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement the loop counter */
        blkCnt--;
      }
    }
#endif /* defined (RISCV_MATH_VECTOR) */

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
       The count variable holds the number of MAC operations performed */
    count = srcBLen - 1U;

    /* Working pointer of inputA */
    if (firstIndex > srcALen)
    {
       pSrc1 = (pIn1 + firstIndex) - (srcBLen - 1U);
    }
    else
    {
       pSrc1 = (pIn1 + srcALen) - (srcBLen - 1U);
    }
    px = pSrc1;
#if defined (RISCV_MATH_VECTOR)
    /* Working pointer of inputB */
    pSrc2 = pIn2 + (srcBLen - 1U);
    py = pSrc2;
    while (blockSize3 > 0U)
    {
    /* Accumulator is made zero for every iteration */
    sum = 0;
    uint32_t vblkCnt = count;                               /* Loop counter */
    size_t l;
    vint16m4_t vx, vy;
    vint32m1_t temp00m1;
    ptrdiff_t bstride = -2;
    l = __riscv_vsetvl_e32m1(vblkCnt);
    temp00m1 = __riscv_vmv_v_x_i32m1(0, l);
    for (; (l = __riscv_vsetvl_e16m4(vblkCnt)) > 0; vblkCnt -= l) {
      vx = __riscv_vle16_v_i16m4(px, l);
      px += l;
      vy = __riscv_vlse16_v_i16m4(py, bstride, l);
      py -= l;
      temp00m1 = __riscv_vredsum_vs_i32m8_i32m1(__riscv_vwmul_vv_i32m8(vx, vy, l), temp00m1, l);
    }
    sum += __riscv_vmv_x_s_i32m1_i32(temp00m1);

    /* Store the result in the accumulator in the destination buffer. */
    *pOut++ = (q15_t) (__SSAT((sum >> 15), 16));

    /* Update the inputA and inputB pointers for next MAC calculation */
    px = ++pSrc1;
    py = pSrc2;

    /* Increment MAC count */
    count--;

    /* Decrement loop counter */
    blockSize3--;
    }
#else
    /* Working pointer of inputB */
    pSrc2 = pIn2 + (srcBLen - 1U);
    pIn2 = pSrc2 - 1U;
    py = pIn2;

    /* -------------------
     * Stage3 process
     * ------------------*/

    /* For loop unrolling by 4, this stage is divided into two. */
    /* First part of this stage computes the MAC operations greater than 4 */
    /* Second part of this stage computes the MAC operations less than or equal to 4 */

    /* The first part of the stage starts here */
    j = count >> 2U;

    while ((j > 0U) && (blockSize3 > 0))
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      /* Apply loop unrolling and compute 4 MACs simultaneously. */
      k = count >> 2U;

      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
      py = py + 1U;
      while (k > 0U)
      {
        /* x[srcALen - srcBLen + 1], x[srcALen - srcBLen + 2] are multiplied
         * with y[srcBLen - 1], y[srcBLen - 2] respectively */
        tmp0 = *py--;
        tmp1 = *py--;
        sum = __SMLALD(read_q15x2_ia ((q15_t **) &px), __RV_PKBB16(tmp1, tmp0), sum);
        /* x[srcALen - srcBLen + 3], x[srcALen - srcBLen + 4] are multiplied
         * with y[srcBLen - 3], y[srcBLen - 4] respectively */
        tmp0 = *py--;
        tmp1 = *py--;
        sum = __SMLALD(read_q15x2_ia ((q15_t **) &px), __RV_PKBB16(tmp1, tmp0), sum);

        /* Decrement loop counter */
        k--;
      }

      /* If the count is not a multiple of 4, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = count & 0x3U;

      while (k > 0U)
      {
        /* sum += x[srcALen - srcBLen + 5] * y[srcBLen - 5] */
        sum = __SMLALD(*px++, *py--, sum);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q15_t) (__SSAT((sum >> 15), 16));

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = ++pSrc1;
      py = pIn2;

      /* Decrement MAC count */
      count--;

      /* Decrement loop counter */
      blockSize3--;

      j--;
    }

    /* The second part of the stage starts here */
    /* SIMD is not used for the next MAC operations,
     * so pointer py is updated to read only one sample at a time */
    py = py + 1U;

    while (blockSize3 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      /* Apply loop unrolling and compute 4 MACs simultaneously. */
      k = count;

      while (k > 0U)
      {
        /* Perform the multiply-accumulates */
        /* sum +=  x[srcALen-1] * y[srcBLen-1] */
        sum = __SMLALD(*px++, *py--, sum);

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q15_t) (__SSAT((sum >> 15), 16));

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = ++pSrc1;
      py = pSrc2;

      /* Decrement MAC count */
      count--;

      /* Decrement the loop counter */
      blockSize3--;
    }
#endif /* defined (RISCV_MATH_VECTOR) */
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);

#else /* #if defined (RISCV_MATH_DSP) */

  const q15_t *pIn1 = pSrcA;                           /* InputA pointer */
  const q15_t *pIn2 = pSrcB;                           /* InputB pointer */
        q63_t sum;                                     /* Accumulator */
        uint32_t i, j;                                 /* Loop counters */
        riscv_status status;                             /* Status of Partial convolution */

  /* Check for range of output samples to be calculated */
  if ((firstIndex + numPoints) > ((srcALen + (srcBLen - 1U))))
  {
    /* Set status as RISCV_MATH_ARGUMENT_ERROR */
    status = RISCV_MATH_ARGUMENT_ERROR;
  }
  else
  {
    /* Loop to calculate convolution for output length number of values */
    for (i = firstIndex; i <= (firstIndex + numPoints - 1); i++)
    {
      /* Initialize sum with zero to carry on MAC operations */
      sum = 0;

      /* Loop to perform MAC operations according to convolution equation */
      for (j = 0U; j <= i; j++)
      {
        /* Check the array limitations */
        if (((i - j) < srcBLen) && (j < srcALen))
        {
          /* z[i] += x[i-j] * y[j] */
          sum += ((q31_t) pIn1[j] * pIn2[i - j]);
        }
      }

      /* Store the output in the destination buffer */
      pDst[i] = (q15_t) __SSAT((sum >> 15U), 16U);
    }

    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);

#endif /* defined(RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR) */

}

/**
  @} end of PartialConv group
 */
