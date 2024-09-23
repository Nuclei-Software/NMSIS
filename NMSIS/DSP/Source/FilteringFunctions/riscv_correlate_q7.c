/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_correlate_q7.c
 * Description:  Correlation of Q7 sequences
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
  @addtogroup Corr
  @{
 */

/**
  @brief         Correlation of Q7 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written.  Length 2 * max(srcALen, srcBLen) - 1.

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 32-bit internal accumulator.
                   Both the inputs are represented in 1.7 format and multiplications yield a 2.14 result.
                   The 2.14 intermediate results are accumulated in a 32-bit accumulator in 18.14 format.
                   This approach provides 17 guard bits and there is no risk of overflow as long as <code>max(srcALen, srcBLen)<131072</code>.
                   The 18.14 result is then truncated to 18.7 format by discarding the low 7 bits and saturated to 1.7 format.

 @remark
                   Refer to \ref riscv_correlate_opt_q7() for a faster implementation of this function.
 */
RISCV_DSP_ATTRIBUTE void riscv_correlate_q7(
  const q7_t * pSrcA,
        uint32_t srcALen,
  const q7_t * pSrcB,
        uint32_t srcBLen,
        q7_t * pDst)
{
#if defined (RISCV_MATH_VECTOR)
  const q7_t *pIn1;                                    /* InputA pointer */
  const q7_t *pIn2;                                    /* InputB pointer */
        q7_t *pOut = pDst;                             /* Output pointer */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t outBlockSize;                         /* Loop counter */
        uint32_t j, ii, jj, kk;

  /* The algorithm implementation is based on the lengths of the inputs. */
  /* srcB is always made to slide across srcA. */
  /* So srcBLen is always considered as shorter or equal to srcALen */
  if (srcALen >= srcBLen)
  {
    /* Initialization of inputA pointer */
    pIn1 = pSrcA;

    /* Initialization of inputB pointer */
    pIn2 = pSrcB;

    /* Number of output samples is calculated */
    outBlockSize = (2U * srcALen) - 1U;

    /* When srcALen > srcBLen, zero padding is done to srcB
     * to make their lengths equal.
     * Instead, (outBlockSize - (srcALen + srcBLen - 1))
     * number of output samples are made zero */
    j = outBlockSize - (srcALen + (srcBLen - 1U));

    /* Updating the pointer position to non zero value */
    pOut += j;
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

    /* CORR(x, y) = Reverse order(CORR(y, x)) */
    /* Hence set the destination pointer to point to the last output sample */
    pOut = pDst + ((srcALen + srcBLen) - 2U);
  }
  pSrcA = pIn1;
  pSrcB = pIn2;

  size_t l;
  vint32m8_t vres0m8;
  vint8m2_t vx;
  q7_t value = 0;
  uint32_t flag = 0;

  blockSize1 = srcBLen - 1U;
  blockSize2 = srcALen - (srcBLen - 1U);
  blockSize3 = blockSize1;
  pIn2 = pSrcB + srcBLen - 1;

  for (ii = blockSize1; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e8m2(ii);
    vx = __riscv_vle8_v_i8m2(pIn1, l);
    vres0m8 = __riscv_vmv_v_x_i32m8(0.0, l);
    flag = 0;
    for (jj = 0; jj < blockSize1; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pIn2 - jj), __riscv_vwadd_vx_i16m4(vx, 0, l), l);
      if (pIn1 - jj <= pSrcA) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 - jj - 1);
      }
      vx = __riscv_vslide1up_vx_i8m2(vx, value, l);
    }
    vx = __riscv_vnclip_wx_i8m2(__riscv_vnsra_wx_i16m4(vres0m8, 7, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse8_v_i8m2(pOut, vx, l);
    pOut += l;
    pIn1 += l;
  }

  pIn2 = pSrcB;
  pIn1 = pSrcA;
  for (ii = blockSize2; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e8m2(ii);
    vx = __riscv_vle8_v_i8m2(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
    for (jj = 0; jj < srcBLen; jj++)
    {
      vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pIn2 + jj), __riscv_vwadd_vx_i16m4(vx, 0, l), l);
      vx = __riscv_vslide1down_vx_i8m2(vx, *(pIn1 + jj), l);
    }
    vx = __riscv_vnclip_wx_i8m2(__riscv_vnsra_wx_i16m4(vres0m8, 7, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse8_v_i8m2(pOut, vx, l);
    pOut += l;
  }

  pIn1 = pSrcA + blockSize2;
  for (ii = blockSize3; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e8m2(ii);
    vx = __riscv_vle8_v_i8m2(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
    flag = 0;
    for (jj = 0; jj < blockSize3; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pIn2 + jj), __riscv_vwadd_vx_i16m4(vx, 0, l), l);
      if (pIn1 + jj >= pSrcA + srcALen) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 + jj);
      }
      vx = __riscv_vslide1down_vx_i8m2(vx, value, l);
    }
    vx = __riscv_vnclip_wx_i8m2(__riscv_vnsra_wx_i16m4(vres0m8, 7, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse8_v_i8m2(pOut, vx, l);
    pOut += l;
  }
#else
  const q7_t *pIn1;                                    /* InputA pointer */
  const q7_t *pIn2;                                    /* InputB pointer */
        q7_t *pOut = pDst;                             /* Output pointer */
  const q7_t *px;                                      /* Intermediate inputA pointer */
  const q7_t *py;                                      /* Intermediate inputB pointer */
  const q7_t *pSrc1;                                   /* Intermediate pointers */
        q31_t sum;                                     /* Accumulators */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t j, k, count, blkCnt;                  /* Loop counters */
        uint32_t outBlockSize;
        int32_t inc = 1;

#if defined (RISCV_MATH_LOOPUNROLL)
        q31_t acc0, acc1, acc2, acc3;                  /* Accumulators */
        q31_t input1, input2;                          /* Temporary input variables */
        q15_t in1, in2;                                /* Temporary input variables */
        q7_t x0, x1, x2, x3, c0, c1;                   /* Temporary variables for holding input and coefficient values */
#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
        q63_t in64_1, in64_2, sum64;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */
#endif /* defined (RISCV_MATH_LOOPUNROLL) */

  /* The algorithm implementation is based on the lengths of the inputs. */
  /* srcB is always made to slide across srcA. */
  /* So srcBLen is always considered as shorter or equal to srcALen */
  /* But CORR(x, y) is reverse of CORR(y, x) */
  /* So, when srcBLen > srcALen, output pointer is made to point to the end of the output buffer */
  /* and the destination pointer modifier, inc is set to -1 */
  /* If srcALen > srcBLen, zero pad has to be done to srcB to make the two inputs of same length */
  /* But to improve the performance,
   * we include zeroes in the output instead of zero padding either of the the inputs*/
  /* If srcALen > srcBLen,
   * (srcALen - srcBLen) zeroes has to included in the starting of the output buffer */
  /* If srcALen < srcBLen,
   * (srcALen - srcBLen) zeroes has to included in the ending of the output buffer */
  if (srcALen >= srcBLen)
  {
    /* Initialization of inputA pointer */
    pIn1 = pSrcA;

    /* Initialization of inputB pointer */
    pIn2 = pSrcB;

    /* Number of output samples is calculated */
    outBlockSize = (2U * srcALen) - 1U;

    /* When srcALen > srcBLen, zero padding is done to srcB
     * to make their lengths equal.
     * Instead, (outBlockSize - (srcALen + srcBLen - 1))
     * number of output samples are made zero */
    j = outBlockSize - (srcALen + (srcBLen - 1U));

    /* Updating the pointer position to non zero value */
    pOut += j;
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

    /* CORR(x, y) = Reverse order(CORR(y, x)) */
    /* Hence set the destination pointer to point to the last output sample */
    pOut = pDst + ((srcALen + srcBLen) - 2U);

    /* Destination address modifier is set to -1 */
    inc = -1;
  }

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

  /* sum = x[0] * y[srcBlen - 1]
   * sum = x[0] * y[srcBlen - 2] + x[1] * y[srcBlen - 1]
   * ....
   * sum = x[0] * y[0] + x[1] * y[1] +...+ x[srcBLen - 1] * y[srcBLen - 1]
   */

  /* In this stage the MAC operations are increased by 1 for every iteration.
     The count variable holds the number of MAC operations performed */
  count = 1U;

  /* Working pointer of inputA */
  px = pIn1;

  /* Working pointer of inputB */
  pSrc1 = pIn2 + (srcBLen - 1U);
  py = pSrc1;

  /* ------------------------
   * Stage1 process
   * ----------------------*/

  /* The first stage starts here */
  while (blockSize1 > 0U)
  {
    /* Accumulator is made zero for every iteration */
    sum = 0;

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
    /* Loop unrolling: Compute 8 outputs at a time */
    k = count >> 3U;
    sum64 = 0;
#else
    /* Loop unrolling: Compute 4 outputs at a time */
    k = count >> 2U;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

    while (k > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      in64_1 = read_q7x8_ia ((q7_t **) &px);
      in64_2 = read_q7x8_ia ((q7_t **) &py);
      sum64 = __RV_SMAQA(sum64, in64_1, in64_2);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
      in64_1 = read_q7x8_ia ((q7_t **) &px);
      in64_2 = read_q7x8_ia ((q7_t **) &py);
      sum64 = __RV_DDSMAQA(sum64, in64_1, in64_2);
#else
      /* x[0] , x[1] */
      in1 = (q15_t) *px++;
      in2 = (q15_t) *px++;
      input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16);

      /* y[srcBLen - 4] , y[srcBLen - 3] */
      in1 = (q15_t) *py++;
      in2 = (q15_t) *py++;
      input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16);

      /* x[0] * y[srcBLen - 4] */
      /* x[1] * y[srcBLen - 3] */
      sum = __SMLAD(input1, input2, sum);

      /* x[2] , x[3] */
      in1 = (q15_t) *px++;
      in2 = (q15_t) *px++;
      input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16);

      /* y[srcBLen - 2] , y[srcBLen - 1] */
      in1 = (q15_t) *py++;
      in2 = (q15_t) *py++;
      input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16);

      /* x[2] * y[srcBLen - 2] */
      /* x[3] * y[srcBLen - 1] */
      sum = __SMLAD(input1, input2, sum);

#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      k--;
    }

    /* Loop unrolling: Compute remaining outputs */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
    k = count & 7U;
    sum = (q31_t)sum64 + (q31_t)(sum64 >> 32);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
    k = count & 7U;
    sum = (q31_t)sum64;
#else
    k = count & 3U;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#else

    /* Initialize k with number of samples */
    k = count;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
      /* x[0] * y[srcBLen - 1] */
      sum += (q31_t) ((q15_t) *px++ * *py++);

      /* Decrement loop counter */
      k--;
    }

    /* Store the result in the accumulator in the destination buffer. */
    *pOut = (q7_t) (__SSAT(sum >> 7U, 8));
    /* Destination pointer is updated according to the address modifier, inc */
    pOut += inc;

    /* Update the inputA and inputB pointers for next MAC calculation */
    py = pSrc1 - count;
    px = pIn1;

    /* Increment MAC count */
    count++;

    /* Decrement loop counter */
    blockSize1--;
  }

  /* --------------------------
   * Initializations of stage2
   * ------------------------*/

  /* sum = x[0] * y[0] + x[1] * y[1] +...+ x[srcBLen-1] * y[srcBLen-1]
   * sum = x[1] * y[0] + x[2] * y[1] +...+ x[srcBLen] * y[srcBLen-1]
   * ....
   * sum = x[srcALen-srcBLen-2] * y[0] + x[srcALen-srcBLen-1] * y[1] +...+ x[srcALen-1] * y[srcBLen-1]
   */

  /* Working pointer of inputA */
  px = pIn1;

  /* Working pointer of inputB */
  py = pIn2;

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
#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 outputs at a time */
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

      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
      do
      {
        /* Read y[0] sample */
        c0 = *py++;
        /* Read y[1] sample */
        c1 = *py++;

        /* Read x[3] sample */
        x3 = *px++;

        /* x[0] and x[1] are packed */
        in1 = (q15_t) x0;
        in2 = (q15_t) x1;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* y[0] and y[1] are packed */
        in1 = (q15_t) c0;
        in2 = (q15_t) c1;

        input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc0 += x[0] * y[0] + x[1] * y[1]  */
        acc0 = __SMLAD(input1, input2, acc0);

        /* x[1] and x[2] are packed */
        in1 = (q15_t) x1;
        in2 = (q15_t) x2;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc1 += x[1] * y[0] + x[2] * y[1] */
        acc1 = __SMLAD(input1, input2, acc1);

        /* x[2] and x[3] are packed */
        in1 = (q15_t) x2;
        in2 = (q15_t) x3;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc2 += x[2] * y[0] + x[3] * y[1]  */
        acc2 = __SMLAD(input1, input2, acc2);

        /* Read x[4] sample */
        x0 = *px++;

        /* x[3] and x[4] are packed */
        in1 = (q15_t) x3;
        in2 = (q15_t) x0;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc3 += x[3] * y[0] + x[4] * y[1]  */
        acc3 = __SMLAD(input1, input2, acc3);

        /* Read y[2] sample */
        c0 = *py++;
        /* Read y[3] sample */
        c1 = *py++;

        /* Read x[5] sample */
        x1 = *px++;

        /* x[2] and x[3] are packed */
        in1 = (q15_t) x2;
        in2 = (q15_t) x3;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* y[2] and y[3] are packed */
        in1 = (q15_t) c0;
        in2 = (q15_t) c1;

        input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc0 += x[2] * y[2] + x[3] * y[3]  */
        acc0 = __SMLAD(input1, input2, acc0);

        /* x[3] and x[4] are packed */
        in1 = (q15_t) x3;
        in2 = (q15_t) x0;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc1 += x[3] * y[2] + x[4] * y[3]  */
        acc1 = __SMLAD(input1, input2, acc1);

        /* x[4] and x[5] are packed */
        in1 = (q15_t) x0;
        in2 = (q15_t) x1;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc2 += x[4] * y[2] + x[5] * y[3]  */
        acc2 = __SMLAD(input1, input2, acc2);

        /* Read x[6] sample */
        x2 = *px++;

        /* x[5] and x[6] are packed */
        in1 = (q15_t) x1;
        in2 = (q15_t) x2;

        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* acc3 += x[5] * y[2] + x[6] * y[3]  */
        acc3 = __SMLAD(input1, input2, acc3);

      } while (--k);

      /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = srcBLen % 0x4U;

      while (k > 0U)
      {
        /* Read y[4] sample */
        c0 = *py++;
        /* Read x[7] sample */
        x3 = *px++;

        /* Perform the multiply-accumulates */
        /* acc0 +=  x[4] * y[4] */
        acc0 += ((q15_t) x0 * c0);
        /* acc1 +=  x[5] * y[4] */
        acc1 += ((q15_t) x1 * c0);
        /* acc2 +=  x[6] * y[4] */
        acc2 += ((q15_t) x2 * c0);
        /* acc3 +=  x[7] * y[4] */
        acc3 += ((q15_t) x3 * c0);

        /* Reuse the present samples for the next MAC */
        x0 = x1;
        x1 = x2;
        x2 = x3;

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut = (q7_t) (__SSAT(acc0 >> 7, 8));
      /* Destination pointer is updated according to the address modifier, inc */
      pOut += inc;

      *pOut = (q7_t) (__SSAT(acc1 >> 7, 8));
      pOut += inc;

      *pOut = (q7_t) (__SSAT(acc2 >> 7, 8));
      pOut += inc;

      *pOut = (q7_t) (__SSAT(acc3 >> 7, 8));
      pOut += inc;

	  count += 4U;
      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pIn2;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = blockSize2 % 0x4U;

#else

    /* Initialize blkCnt with number of samples */
    blkCnt = blockSize2;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (blkCnt > 0U)
    {
      /* Accumulator is made zero for every iteration */
      sum = 0;

#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
      /* Loop unrolling: Compute 8 outputs at a time */
      k = srcBLen >> 3U;
      sum64 = 0;
#else
      /* Loop unrolling: Compute 4 outputs at a time */
      k = srcBLen >> 2U;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

      while (k > 0U)
      {

#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        in64_1 = read_q7x8_ia ((q7_t **) &px);
        in64_2 = read_q7x8_ia ((q7_t **) &py);
        sum64 = __RV_SMAQA(sum64, in64_1, in64_2);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
        in64_1 = read_q7x8_ia ((q7_t **) &px);
        in64_2 = read_q7x8_ia ((q7_t **) &py);
        sum64 = __RV_DDSMAQA(sum64, in64_1, in64_2);
#else
        /* Reading two inputs of SrcA buffer and packing */
        in1 = (q15_t) *px++;
        in2 = (q15_t) *px++;
        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* Reading two inputs of SrcB buffer and packing */
        in1 = (q15_t) *py++;
        in2 = (q15_t) *py++;
        input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* Perform the multiply-accumulate */
        sum = __SMLAD(input1, input2, sum);

        /* Reading two inputs of SrcA buffer and packing */
        in1 = (q15_t) *px++;
        in2 = (q15_t) *px++;
        input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* Reading two inputs of SrcB buffer and packing */
        in1 = (q15_t) *py++;
        in2 = (q15_t) *py++;
        input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

        /* Perform the multiply-accumulate */
        sum = __SMLAD(input1, input2, sum);

        /* Decrement loop counter */
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
        k--;
      }

      /* Loop unrolling: Compute remaining outputs */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      k = srcBLen & 0x7U;
      sum = (q31_t)sum64 + (q31_t)(sum64 >> 32);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
      k = srcBLen & 0x7U;
      sum = (q31_t)sum64;
#else
      k = srcBLen & 0x3U;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#else

      /* Initialize blkCnt with number of samples */
      k = srcBLen;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

      while (k > 0U)
      {
        /* Perform the multiply-accumulate */
        sum += ((q15_t) *px++ * *py++);

        /* Decrement the loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut = (q7_t) (__SSAT(sum >> 7U, 8));
      /* Destination pointer is updated according to the address modifier, inc */
      pOut += inc;

      /* Increment the pointer pIn1 index, count by 1 */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pIn2;

      /* Decrement the loop counter */
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
        sum += ((q15_t) *px++ * *py++);

        /* Decrement the loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut = (q7_t) (__SSAT(sum >> 7U, 8));
      /* Destination pointer is updated according to the address modifier, inc */
      pOut += inc;

      /* Increment the MAC count */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pIn2;

      /* Decrement loop counter */
      blkCnt--;
    }
  }

  /* --------------------------
   * Initializations of stage3
   * -------------------------*/

  /* sum += x[srcALen-srcBLen+1] * y[0] + x[srcALen-srcBLen+2] * y[1] +...+ x[srcALen-1] * y[srcBLen-1]
   * sum += x[srcALen-srcBLen+2] * y[0] + x[srcALen-srcBLen+3] * y[1] +...+ x[srcALen-1] * y[srcBLen-1]
   * ....
   * sum +=  x[srcALen-2] * y[0] + x[srcALen-1] * y[1]
   * sum +=  x[srcALen-1] * y[0]
   */

  /* In this stage the MAC operations are decreased by 1 for every iteration.
     The count variable holds the number of MAC operations performed */
  count = srcBLen - 1U;

  /* Working pointer of inputA */
  pSrc1 = pIn1 + (srcALen - (srcBLen - 1U));
  px = pSrc1;

  /* Working pointer of inputB */
  py = pIn2;

  /* -------------------
   * Stage3 process
   * ------------------*/

  while (blockSize3 > 0U)
  {
    /* Accumulator is made zero for every iteration */
    sum = 0;
#if defined (RISCV_MATH_LOOPUNROLL)

#if defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64))
    /* Loop unrolling: Compute 8 outputs at a time */
    k = count >> 3U;
    sum64 = 0;
#else
    /* Loop unrolling: Compute 4 outputs at a time */
    k = count >> 2U;
#endif /* defined (RISCV_MATH_DSP) && (defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)) */

    while (k > 0U)
    {
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
        in64_1 = read_q7x8_ia ((q7_t **) &px);
        in64_2 = read_q7x8_ia ((q7_t **) &py);
        sum64 = __RV_SMAQA(sum64, in64_1, in64_2);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
        in64_1 = read_q7x8_ia ((q7_t **) &px);
        in64_2 = read_q7x8_ia ((q7_t **) &py);
        sum64 = __RV_DDSMAQA(sum64, in64_1, in64_2);
#else
      /* x[srcALen - srcBLen + 1] , x[srcALen - srcBLen + 2]  */
      in1 = (q15_t) *px++;
      in2 = (q15_t) *px++;
      input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

      /* y[0] , y[1] */
      in1 = (q15_t) *py++;
      in2 = (q15_t) *py++;
      input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

      /* sum += x[srcALen - srcBLen + 1] * y[0] */
      /* sum += x[srcALen - srcBLen + 2] * y[1] */
      sum = __SMLAD(input1, input2, sum);

      /* x[srcALen - srcBLen + 3] , x[srcALen - srcBLen + 4] */
      in1 = (q15_t) *px++;
      in2 = (q15_t) *px++;
      input1 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

      /* y[2] , y[3] */
      in1 = (q15_t) *py++;
      in2 = (q15_t) *py++;
      input2 = ((q31_t) in1 & 0x0000FFFF) | ((q31_t) in2 << 16U);

      /* sum += x[srcALen - srcBLen + 3] * y[2] */
      /* sum += x[srcALen - srcBLen + 4] * y[3] */
      sum = __SMLAD(input1, input2, sum);

#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      /* Decrement loop counter */
      k--;
    }

    /* Loop unrolling: Compute remaining outputs */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      k = count & 0x7U;
      sum = (q31_t)sum64 + (q31_t)(sum64 >> 32);
#else
#if defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3)
      k = count & 0x7U;
      sum = (q31_t)sum64;
#else
      k = count & 0x3U;
#endif /* defined (RISCV_MATH_DSP) && defined (NUCLEI_DSP_N3) */
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#else

    /* Initialize blkCnt with number of samples */
    k = count;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
      sum += ((q15_t) *px++ * *py++);

      /* Decrement loop counter */
      k--;
    }
    /* Store the result in the accumulator in the destination buffer. */
    *pOut = (q7_t) (__SSAT(sum >> 7U, 8));
    /* Destination pointer is updated according to the address modifier, inc */
    pOut += inc;

    /* Update the inputA and inputB pointers for next MAC calculation */
    px = ++pSrc1;
    py = pIn2;

    /* Decrement MAC count */
    count--;

    /* Decrement loop counter */
    blockSize3--;
  }
#endif /* defined (RISCV_MATH_VECTOR) */
}

/**
  @} end of Corr group
 */
