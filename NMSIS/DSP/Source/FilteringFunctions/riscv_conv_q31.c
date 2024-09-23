/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_conv_q31.c
 * Description:  Convolution of Q31 sequences
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
  @brief         Convolution of Q31 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written.  Length srcALen+srcBLen-1.

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   There is no saturation on intermediate additions.
                   Thus, if the accumulator overflows it wraps around and distorts the result.
                   The input signals should be scaled down to avoid intermediate overflows.
                   Scale down the inputs by log2(min(srcALen, srcBLen)) (log2 is read as log to the base 2) times to avoid overflows,
                   as maximum of min(srcALen, srcBLen) number of additions are carried internally.
                   The 2.62 accumulator is right shifted by 31 bits and saturated to 1.31 format to yield the final result.

  @remark
                   Refer to \ref riscv_conv_fast_q31() for a faster but less precise implementation of this function.
 */
RISCV_DSP_ATTRIBUTE void riscv_conv_q31(
  const q31_t * pSrcA,
        uint32_t srcALen,
  const q31_t * pSrcB,
        uint32_t srcBLen,
        q31_t * pDst)
{
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  const q31_t *pIn1;                               /* InputA pointer */
  const q31_t *pIn2;                               /* InputB pointer */
        q31_t *pOut = pDst;                        /* Output pointer */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
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
  pSrcA = pIn1;
  pSrcB = pIn2;

  size_t l;
  vint64m8_t vres0m8;
  vint32m4_t vx;
  q31_t value = 0;
  uint32_t flag = 0;

  blockSize1 = srcBLen - 1U;
  blockSize2 = srcALen - (srcBLen - 1U);
  blockSize3 = blockSize1;

  for (ii = blockSize1; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e32m4(ii);
    vx = __riscv_vle32_v_i32m4(pIn1, l);
    vres0m8 = __riscv_vmv_v_x_i64m8(0.0, l);
    flag = 0;
    for (jj = 0; jj < blockSize1; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i64m8 (vres0m8, *(pIn2 + jj), vx, l);
      if (pIn1 - jj <= pSrcA) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 - jj - 1);
      }
      vx = __riscv_vslide1up_vx_i32m4(vx, value, l);
    }
    vx = __riscv_vnsra_wx_i32m4(vres0m8, 31, l);
    __riscv_vse32_v_i32m4(pOut, vx, l);
    pOut += l;
    pIn1 += l;
  }

  pIn2 += srcBLen - 1;
  pIn1 = pSrcA;
  for (ii = blockSize2; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e32m4(ii);
    vx = __riscv_vle32_v_i32m4(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i64m8(0, l);
    for (jj = 0; jj < srcBLen; jj++)
    {
      vres0m8 = __riscv_vwmacc_vx_i64m8(vres0m8, *(pIn2 - jj), vx, l);
      vx = __riscv_vslide1down_vx_i32m4(vx, *(pIn1 + jj), l);
    }
    vx = __riscv_vnsra_wx_i32m4(vres0m8, 31, l);
    __riscv_vse32_v_i32m4(pOut, vx, l);
    pOut += l;
  }

  pIn1 = pSrcA + blockSize2;
  for (ii = blockSize3; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e32m4(ii);
    vx = __riscv_vle32_v_i32m4(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i64m8(0, l);
    flag = 0;
    for (jj = 0; jj < blockSize3; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i64m8(vres0m8, *(pIn2 - jj), vx, l);
      if (pIn1 + jj >= pSrcA + srcALen) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 + jj);
      }
      vx = __riscv_vslide1down_vx_i32m4(vx, value, l);
    }
    vx = __riscv_vnsra_wx_i32m4(vres0m8, 31, l);
    __riscv_vse32_v_i32m4(pOut, vx, l);
    pOut += l;
  }
#else
  const q31_t *pIn1;                                   /* InputA pointer */
  const q31_t *pIn2;                                   /* InputB pointer */
        q31_t *pOut = pDst;                            /* Output pointer */
  const q31_t *px;                                     /* Intermediate inputA pointer */
  const q31_t *py;                                     /* Intermediate inputB pointer */
  const q31_t *pSrc1, *pSrc2;                          /* Intermediate pointers */
        q63_t sum;                                     /* Accumulators */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t j, k, count, blkCnt;                  /* Loop counters */

#if defined (RISCV_MATH_LOOPUNROLL)
        q63_t acc0, acc1, acc2, acc3;                        /* Accumulators */
        q31_t x0, x1, x2, x3, c0;                          /* Temporary variables to hold state and coefficient values */
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
       q63_t px64, py64, c064;
       q31_t tmp0, tmp1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
#endif /* defined (RISCV_MATH_LOOPUNROLL) */

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
    sum = 0;

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
      sum += (q63_t) *px++ * *py--;

      /* Decrement loop counter */
      k--;
    }

    /* Store the result in the accumulator in the destination buffer. */
    *pOut++ = (q31_t) (sum >> 31);

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
   * sum = x[1] * y[srcBLen-1] + x[2] * y[srcBLen-2] +...+ x[srcBLen]   * y[0]
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
#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unroll by 4 */
    blkCnt = blockSize2 >> 2;

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
      k = srcBLen >> 2;
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
        acc0 += ((q63_t) x0 * c0);
        /* acc1 +=  x[1] * y[srcBLen - 1] */
        acc1 += ((q63_t) x1 * c0);
        /* acc2 +=  x[2] * y[srcBLen - 1] */
        acc2 += ((q63_t) x2 * c0);
        /* acc3 +=  x[3] * y[srcBLen - 1] */
        acc3 += ((q63_t) x3 * c0);

        /* Read y[srcBLen - 2] sample */
        c0 = *py--;
        /* Read x[4] sample */
        x0 = *px++;
        /* Perform the multiply-accumulate */
        /* acc0 +=  x[1] * y[srcBLen - 2] */
        acc0 += ((q63_t) x1 * c0);
        /* acc1 +=  x[2] * y[srcBLen - 2] */
        acc1 += ((q63_t) x2 * c0);
        /* acc2 +=  x[3] * y[srcBLen - 2] */
        acc2 += ((q63_t) x3 * c0);
        /* acc3 +=  x[4] * y[srcBLen - 2] */
        acc3 += ((q63_t) x0 * c0);

        /* Read y[srcBLen - 3] sample */
        c0 = *py--;
        /* Read x[5] sample */
        x1 = *px++;
        /* Perform the multiply-accumulate */
        /* acc0 +=  x[2] * y[srcBLen - 3] */
        acc0 += ((q63_t) x2 * c0);
        /* acc1 +=  x[3] * y[srcBLen - 2] */
        acc1 += ((q63_t) x3 * c0);
        /* acc2 +=  x[4] * y[srcBLen - 2] */
        acc2 += ((q63_t) x0 * c0);
        /* acc2 +=  x[5] * y[srcBLen - 2] */
        acc3 += ((q63_t) x1 * c0);

        /* Read y[srcBLen - 3] sample */
        c0 = *py--;
        /* Read x[5] sample */
        x2 = *px++;
        /* Perform the multiply-accumulate */
        /* acc0 +=  x[2] * y[srcBLen - 3] */
        acc0 += ((q63_t) x3 * c0);
        /* acc1 +=  x[3] * y[srcBLen - 2] */
        acc1 += ((q63_t) x0 * c0);
        /* acc2 +=  x[4] * y[srcBLen - 2] */
        acc2 += ((q63_t) x1 * c0);
        /* acc2 +=  x[5] * y[srcBLen - 2] */
        acc3 += ((q63_t) x2 * c0);

#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */
      } while (--k);

      /* If the srcBLen is not a multiple of 3, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = srcBLen & 0x3U;
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
        acc0 += ((q63_t) x0 * c0);
        /* acc1 +=  x[5] * y[srcBLen - 5] */
        acc1 += ((q63_t) x1 * c0);
        /* acc2 +=  x[6] * y[srcBLen - 5] */
        acc2 += ((q63_t) x2 * c0);
        /* acc3 +=  x[7] * y[srcBLen - 5] */
        acc3 += ((q63_t) x3 * c0);

        /* Reuse the present samples for the next MAC */
        x0 = x1;
        x1 = x2;
        x2 = x3;

        /* Decrement loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut++ = (q31_t) (acc0 >> 31);
      *pOut++ = (q31_t) (acc1 >> 31);
      *pOut++ = (q31_t) (acc2 >> 31);
      *pOut++ = (q31_t) (acc3 >> 31);

      /* Increment the pointer pIn1 index, count by 4 */
      count += 4U;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pSrc2;

      /* Decrement loop counter */
      blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = blockSize2 & 0x3U;

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
        sum += (q63_t) *px++ * *py--;
        sum += (q63_t) *px++ * *py--;
        sum += (q63_t) *px++ * *py--;
        sum += (q63_t) *px++ * *py--;

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

        /* Decrement the loop counter */
        k--;
      }

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
        sum += (q63_t) *px++ * *py--;

        /* Decrement the loop counter */
        k--;
      }

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
    sum = 0;

#if defined (RISCV_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 outputs at a time */
    k = blockSize3 >> 2U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      py -= 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
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
    k = blockSize3 & 0x3U;
#if defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64)
      py += 1;
#endif /* defined (RISCV_MATH_DSP) && (__RISCV_XLEN == 64) */

#else

    /* Initialize blkCnt with number of samples */
    k = blockSize3;

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

    /* Decrement loop counter */
    blockSize3--;
  }
#endif /* defined (RISCV_MATH_VECTOR) */

}

/**
  @} end of Conv group
 */
