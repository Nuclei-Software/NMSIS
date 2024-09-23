/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_conv_partial_q31.c
 * Description:  Partial convolution of Q31 sequences
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
  @brief         Partial convolution of Q31 sequences.
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
                   Refer to \ref riscv_conv_partial_fast_q31() for a faster but less precise implementation of this function.
 */

RISCV_DSP_ATTRIBUTE riscv_status riscv_conv_partial_q31(
  const q31_t * pSrcA,
        uint32_t srcALen,
  const q31_t * pSrcB,
        uint32_t srcBLen,
        q31_t * pDst,
        uint32_t firstIndex,
        uint32_t numPoints)
{

#if defined(RISCV_MATH_DSP) || defined (RISCV_MATH_VECTOR)

  const q31_t *pIn1;                                   /* InputA pointer */
  const q31_t *pIn2;                                   /* InputB pointer */
        q31_t *pOut = pDst;                            /* Output pointer */
  const q31_t *px;                                     /* Intermediate inputA pointer */
  const q31_t *py;                                     /* Intermediate inputB pointer */
  const q31_t *pSrc1, *pSrc2;                          /* Intermediate pointers */
        q63_t sum;                                     /* Accumulator */
        uint32_t j, k, count, blkCnt, check;
        int32_t blockSize1, blockSize2, blockSize3;    /* Loop counters */
        riscv_status status;                             /* Status of Partial convolution */

#if defined (RISCV_MATH_LOOPUNROLL)
        q63_t acc0, acc1, acc2, acc3;                        /* Accumulator */
        q31_t x0, x1, x2, x3, c0;                          /* Temporary variables */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        q63_t px64, py64, c064;
        q31_t tmp0, tmp1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
#endif

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

    /* The first stage starts here */
    while (blockSize1 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
      uint32_t vblkCnt = count;                               /* Loop counter */
      size_t l;
      vint32m4_t vx, vy;
      vint64m1_t temp00m1;
      ptrdiff_t bstride = -4;
      l = __riscv_vsetvl_e64m1(1);
      temp00m1 = __riscv_vmv_v_x_i64m1(0, l);
      for (; (l = __riscv_vsetvl_e32m4(vblkCnt)) > 0; vblkCnt -= l) {
        vx = __riscv_vle32_v_i32m4(px, l);
        px += l;
        vy = __riscv_vlse32_v_i32m4(py, bstride, l);
        py -= l;
        temp00m1 = __riscv_vredsum_vs_i64m8_i64m1(__riscv_vwmul_vv_i64m8(vx, vy, l), temp00m1, l);
      }
      sum += __riscv_vmv_x_s_i64m1_i64(temp00m1);
#else
#if defined (RISCV_MATH_LOOPUNROLL)

      /* Loop unrolling: Compute 4 outputs at a time */
      k = count >> 2U;

      while (k > 0U)
      {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        px64 = read_q31x2_ia ((q31_t **) &px);
        tmp0 = *py--;
        tmp1 = *py--;
        py64 = __RV_PKBB32(tmp1, tmp0);
        sum = __RV_KMADA32(sum, px64, py64);

        px64 = read_q31x2_ia ((q31_t **) &px);
        tmp0 = *py--;
        tmp1 = *py--;
        py64 = __RV_PKBB32(tmp1, tmp0);
        sum = __RV_KMADA32(sum, px64, py64);
#else
        /* x[0] * y[srcBLen - 1] */
        sum += (q63_t) *px++ * (*py--);

        /* x[1] * y[srcBLen - 2] */
        sum += (q63_t) *px++ * (*py--);

        /* x[2] * y[srcBLen - 3] */
        sum += (q63_t) *px++ * (*py--);

        /* x[3] * y[srcBLen - 4] */
        sum += (q63_t) *px++ * (*py--);

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        /* Decrement loop counter */
        k--;
      }
      /* Loop unrolling: Compute remaining outputs */
      k = count & 0x3U;

#else

      /* Initialize k with number of samples */
      k = count;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
        sum += (q63_t) *px++ * (*py--);

        /* Decrement loop counter */
        k--;
      }
#endif /* #if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q31_t) (sum >> 31);

      /* Update the inputA and inputB pointers for next MAC calculation */
      py = ++pSrc2;
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

    /* count is index by which the pointer pIn1 to be incremented */
    count = 0U;

    /* -------------------
     * Stage2 process
     * ------------------*/
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    blkCnt = blockSize2;

    while (blkCnt > 0U)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;
      uint32_t vblkCnt = srcBLen;                               /* Loop counter */
      size_t l;
      vint32m4_t vx, vy;
      vint64m1_t temp00m1;
      ptrdiff_t bstride = -4;
      l = __riscv_vsetvl_e64m1(1);
      temp00m1 = __riscv_vmv_v_x_i64m1(0, l);
      for (; (l = __riscv_vsetvl_e32m4(vblkCnt)) > 0; vblkCnt -= l) {
        vx = __riscv_vle32_v_i32m4(px, l);
        px += l;
        vy = __riscv_vlse32_v_i32m4(py, bstride, l);
        py -= l;
        temp00m1 = __riscv_vredsum_vs_i64m8_i64m1(__riscv_vwmul_vv_i64m8(vx, vy, l), temp00m1, l);
      }
      sum += __riscv_vmv_x_s_i64m1_i64(temp00m1);
      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q31_t) (sum >> 31);

      /* Increment MAC count */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pSrc2;

      /* Decrement loop counter */
      blkCnt--;
    }
#else
    /* Stage2 depends on srcBLen as in this stage srcBLen number of MACS are performed.
     * So, to loop unroll over blockSize2,
     * srcBLen should be greater than or equal to 4 */
    if (srcBLen >= 4U)
    {
#if defined (RISCV_MATH_LOOPUNROLL)

      /* Loop unroll over blkCnt */
      blkCnt = blockSize2 >> 2U;

      while (blkCnt > 0U)
      {
        /* Set all accumulators to zero */
        acc0 = 0;
        acc1 = 0;
        acc2 = 0;
        acc3 = 0;

        /* read x[0], x[1], x[2] samples */
        x0 = *px++;
        x1 = *px++;
        x2 = *px++;

        /* Apply loop unrolling and compute 4 MACs simultaneously. */
        k = srcBLen >> 2U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        py -= 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

        /* First part of the processing with loop unrolling.  Compute 3 MACs at a time.
         ** a second loop below computes MACs for the remaining 1 to 2 samples. */
        do
        {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
          c064 = read_q31x2_da ((q31_t **) &py);
          x3 = *px++;
          px64 = __RV_PKBB32(x1, x0);
          acc0 = __RV_KMAXDA32(acc0, px64, c064);
          px64 = __RV_PKBB32(x2, x1);
          acc1 = __RV_KMAXDA32(acc1, px64, c064);
          px64 = __RV_PKBB32(x3, x2);
          acc2 = __RV_KMAXDA32(acc2, px64, c064);
          x0 = *px++;
          px64 = __RV_PKBB32(x0, x3);
          acc3 = __RV_KMAXDA32(acc3, px64, c064);

          c064 = read_q31x2_da ((q31_t **) &py);
          px64 = __RV_PKBB32(x3, x2);
          acc0 = __RV_KMAXDA32(acc0, px64, c064);
          px64 = __RV_PKBB32(x0, x3);
          acc1 = __RV_KMAXDA32(acc1, px64, c064);
          x1 = *px++;
          px64 = __RV_PKBB32(x1, x0);
          acc2 = __RV_KMAXDA32(acc2, px64, c064);
          x2 = *px++;
          px64 = __RV_PKBB32(x2, x1);
          acc3 = __RV_KMAXDA32(acc3, px64, c064);
#else
          /* Read y[srcBLen - 1] sample */
          c0 = *py--;

          /* Read x[3] sample */
          x3 = *px++;

          /* Perform the multiply-accumulate */
          /* acc0 +=  x[0] * y[srcBLen - 1] */
          acc0 += (q63_t) x0 * c0;
          /* acc1 +=  x[1] * y[srcBLen - 1] */
          acc1 += (q63_t) x1 * c0;
          /* acc2 +=  x[2] * y[srcBLen - 1] */
          acc2 += (q63_t) x2 * c0;
          /* acc3 +=  x[3] * y[srcBLen - 1] */
          acc3 += (q63_t) x3 * c0;

          /* Read y[srcBLen - 2] sample */
          c0 = *py--;

          /* Read x[4] sample */
          x0 = *px++;

          /* Perform the multiply-accumulate */
          /* acc0 +=  x[1] * y[srcBLen - 2] */
          acc0 += (q63_t) x1 * c0;
          /* acc1 +=  x[2] * y[srcBLen - 2] */
          acc1 += (q63_t) x2 * c0;
          /* acc2 +=  x[3] * y[srcBLen - 2] */
          acc2 += (q63_t) x3 * c0;
          /* acc3 +=  x[4] * y[srcBLen - 2] */
          acc3 += (q63_t) x0 * c0;

          /* Read y[srcBLen - 3] sample */
          c0 = *py--;

          /* Read x[5] sample */
          x1 = *px++;

          /* Perform the multiply-accumulate */
          /* acc0 +=  x[2] * y[srcBLen - 3] */
          acc0 += (q63_t) x2 * c0;
          /* acc1 +=  x[3] * y[srcBLen - 2] */
          acc1 += (q63_t) x3 * c0;
          /* acc2 +=  x[4] * y[srcBLen - 2] */
          acc2 += (q63_t) x0 * c0;
          /* acc3 +=  x[4] * y[srcBLen - 2] */
          acc3 += (q63_t) x1 * c0;

          /* Read y[srcBLen - 4] sample */
          c0 = *py--;

          /* Read x[6] sample */
          x2 = *px++;

          /* Perform the multiply-accumulate */
          /* acc0 +=  x[2] * y[srcBLen - 3] */
          acc0 += (q63_t) x3 * c0;
          /* acc1 +=  x[3] * y[srcBLen - 2] */
          acc1 += (q63_t) x0 * c0;
          /* acc2 +=  x[4] * y[srcBLen - 2] */
          acc2 += (q63_t) x1 * c0;
          /* acc3 +=  x[4] * y[srcBLen - 2] */
          acc3 += (q63_t) x2 * c0;

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        } while (--k);

        /* If the srcBLen is not a multiple of 3, compute any remaining MACs here.
         ** No loop unrolling is used. */
        k = srcBLen & 3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        py += 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

        while (k > 0U)
        {
          /* Read y[srcBLen - 5] sample */
          c0 = *py--;
          /* Read x[7] sample */
          x3 = *px++;

          /* Perform the multiply-accumulates */
          /* acc0 +=  x[4] * y[srcBLen - 5] */
          acc0 += (q63_t) x0 * c0;
          /* acc1 +=  x[5] * y[srcBLen - 5] */
          acc1 += (q63_t) x1 * c0;
          /* acc2 +=  x[6] * y[srcBLen - 5] */
          acc2 += (q63_t) x2 * c0;
          /* acc3 +=  x[7] * y[srcBLen - 5] */
          acc3 += (q63_t) x3 * c0;

          /* Reuse the present samples for the next MAC */
          x0 = x1;
          x1 = x2;
          x2 = x3;

          /* Decrement the loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = (q31_t) (acc0 >> 31);
        *pOut++ = (q31_t) (acc1 >> 31);
        *pOut++ = (q31_t) (acc2 >> 31);
        *pOut++ = (q31_t) (acc3 >> 31);

        /* Increment the pointer pIn1 index, count by 3 */
        count += 4U;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement loop counter */
        blkCnt--;
      }

      /* Loop unrolling: Compute remaining outputs */
      blkCnt = blockSize2 & 3U;

#else

      /* Initialize blkCnt with number of samples */
      blkCnt = blockSize2;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

      while (blkCnt > 0U)
      {
        /* Accumulator is made zero for every iteration */
        sum = 0;

#if defined (RISCV_MATH_LOOPUNROLL)

        /* Loop unrolling: Compute 4 outputs at a time */
        k = srcBLen >> 2U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        py -= 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        while (k > 0U)
        {

          /* Perform the multiply-accumulates */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
          tmp0 = *px++;
          tmp1 = *px++;
          px64 = __RV_PKBB32(tmp1, tmp0);
          py64 = read_q31x2_da ((q31_t **) &py);
          sum = __RV_KMAXDA32(sum, px64, py64);

          tmp0 = *px++;
          tmp1 = *px++;
          px64 = __RV_PKBB32(tmp1, tmp0);
          py64 = read_q31x2_da ((q31_t **) &py);
          sum = __RV_KMAXDA32(sum, px64, py64);
#else
          sum += (q63_t) *px++ * (*py--);
          sum += (q63_t) *px++ * (*py--);
          sum += (q63_t) *px++ * (*py--);
          sum += (q63_t) *px++ * (*py--);

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
          /* Decrement loop counter */
          k--;
        }
        /* Loop unrolling: Compute remaining outputs */
        k = srcBLen & 0x3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      py += 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#else

        /* Initialize blkCnt with number of samples */
        k = srcBLen;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

        while (k > 0U)
        {
          /* Perform the multiply-accumulate */
          sum += (q63_t) *px++ * *py--;

          /* Decrement loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = (q31_t) (sum >> 31);

        /* Increment MAC count */
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
          sum += (q63_t) *px++ * *py--;

          /* Decrement loop counter */
          k--;
        }

        /* Store the result in the accumulator in the destination buffer. */
        *pOut++ = (q31_t) (sum >> 31);

        /* Increment the MAC count */
        count++;

        /* Update the inputA and inputB pointers for next MAC calculation */
        px = pSrc1 + count;
        py = pSrc2;

        /* Decrement the loop counter */
        blkCnt--;
      }
    }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */

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

    /* Working pointer of inputB */
    pSrc2 = pIn2 + (srcBLen - 1U);
    py = pSrc2;

    /* -------------------
     * Stage3 process
     * ------------------*/
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
    while (blockSize3 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

      uint32_t vblkCnt = count;                               /* Loop counter */
      size_t l;
      vint32m4_t vx, vy;
      vint64m1_t temp00m1;
      ptrdiff_t bstride = -4;
      l = __riscv_vsetvl_e64m1(1);
      temp00m1 = __riscv_vmv_v_x_i64m1(0, l);
      for (; (l = __riscv_vsetvl_e32m4(vblkCnt)) > 0; vblkCnt -= l) {
        vx = __riscv_vle32_v_i32m4(px, l);
        px += l;
        vy = __riscv_vlse32_v_i32m4(py, bstride, l);
        py -= l;
        temp00m1 = __riscv_vredsum_vs_i64m8_i64m1(__riscv_vwmul_vv_i64m8(vx, vy, l), temp00m1, l);
      }
      sum += __riscv_vmv_x_s_i64m1_i64(temp00m1);
      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q31_t) (sum >> 31);

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = ++pSrc1;
      py = pSrc2;

      /* Decrement MAC count */
      count--;

      /* Decrement the loop counter */
      blockSize3--;
    }
#else
    while (blockSize3 > 0)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

#if defined (RISCV_MATH_LOOPUNROLL)
      /* Loop unrolling: Compute 4 outputs at a time */
      k = count >> 2U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      py -= 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

      while (k > 0U)
      {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        tmp0 = *px++;
        tmp1 = *px++;
        px64 = __RV_PKBB32(tmp1, tmp0);
        py64 = read_q31x2_da ((q31_t **) &py);
        sum = __RV_KMAXDA32(sum, px64, py64);
        tmp0 = *px++;
        tmp1 = *px++;
        px64 = __RV_PKBB32(tmp1, tmp0);
        py64 = read_q31x2_da ((q31_t **) &py);
        sum = __RV_KMAXDA32(sum, px64, py64);
#else
        /* sum += x[srcALen - srcBLen + 1] * y[srcBLen - 1] */
        sum += (q63_t) *px++ * *py--;

        /* sum += x[srcALen - srcBLen + 2] * y[srcBLen - 2] */
        sum += (q63_t) *px++ * *py--;

        /* sum += x[srcALen - srcBLen + 3] * y[srcBLen - 3] */
        sum += (q63_t) *px++ * *py--;

        /* sum += x[srcALen - srcBLen + 4] * y[srcBLen - 4] */
        sum += (q63_t) *px++ * *py--;

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        /* Decrement loop counter */
        k--;
      }
      /* Loop unrolling: Compute remaining outputs */
      k = count & 0x3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      py += 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#else
      /* Initialize blkCnt with number of samples */
      k = count;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
        /* sum +=  x[srcALen-1] * y[srcBLen-1] */
        sum += (q63_t) *px++ * *py--;

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q31_t) (sum >> 31);

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = ++pSrc1;
      py = pSrc2;

      /* Decrement MAC count */
      count--;

      /* Decrement the loop counter */
      blockSize3--;
    }
#endif /* defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64) */
    /* Set status as RISCV_MATH_SUCCESS */
    status = RISCV_MATH_SUCCESS;
  }

  /* Return to application */
  return (status);

#else
/* alternate version for CM0_FAMILY */

  const q31_t *pIn1 = pSrcA;                           /* InputA pointer */
  const q31_t *pIn2 = pSrcB;                           /* InputB pointer */
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
          sum += ((q63_t) pIn1[j] * pIn2[i - j]);
        }
      }

      /* Store the output in the destination buffer */
      pDst[i] = (q31_t) (sum >> 31U);
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
