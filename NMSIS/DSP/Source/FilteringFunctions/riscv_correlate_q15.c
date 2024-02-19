/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_correlate_q15.c
 * Description:  Correlation of Q15 sequences
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
  @brief         Correlation of Q15 sequences.
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written.  Length 2 * max(srcALen, srcBLen) - 1.
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 64-bit internal accumulator.
                   Both inputs are in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   This approach provides 33 guard bits and there is no risk of overflow.
                   The 34.30 result is then truncated to 34.15 format by discarding the low 15 bits and then saturated to 1.15 format.

  @remark
                   Refer to \ref riscv_correlate_fast_q15() for a faster but less precise version of this function.
  @remark
                   Refer to \ref riscv_correlate_opt_q15() for a faster implementation of this function using scratch buffers.
 */
void riscv_correlate_q15(
  const q15_t * pSrcA,
        uint32_t srcALen,
  const q15_t * pSrcB,
        uint32_t srcBLen,
        q15_t * pDst)
{
#if defined (RISCV_MATH_VECTOR) && (__RISCV_XLEN == 64)
  const q15_t *pIn1;                                   /* InputA pointer */
  const q15_t *pIn2;                                   /* InputB pointer */
        q15_t *pOut = pDst;                            /* Output pointer */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t outBlockSize;
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
    outBlockSize = (srcALen * 2U) - 1U;

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
  vint64m8_t vres0m8;
  vint16m2_t vx;
  q15_t value = 0;
  uint32_t flag = 0;

  blockSize1 = srcBLen - 1U;
  blockSize2 = srcALen - (srcBLen - 1U);
  blockSize3 = blockSize1;
  pIn2 = pSrcB + srcBLen - 1;

  for (ii = blockSize1; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e16m2(ii);
    vx = __riscv_vle16_v_i16m2(pIn1, l);
    vres0m8 = __riscv_vmv_v_x_i64m8(0.0, l);
    flag = 0;
    for (jj = 0; jj < blockSize1; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i64m8(vres0m8, *(pIn2 - jj), __riscv_vwadd_vx_i32m4 (vx, 0, l), l);
      if (pIn1 - jj <= pSrcA) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 - jj - 1);
      }
      vx = __riscv_vslide1up_vx_i16m2(vx, value, l);
    }
    vx = __riscv_vnclip_wx_i16m2(__riscv_vnsra_wx_i32m4(vres0m8, 15, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse16_v_i16m2(pOut, vx, l);
    pOut += l;
    pIn1 += l;
  }
  pIn2 = pSrcB;
  pIn1 = pSrcA;
  for (ii = blockSize2; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e16m2(ii);
    vx = __riscv_vle16_v_i16m2(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i64m8(0, l);
    for (jj = 0; jj < srcBLen; jj++)
    {
      vres0m8 = __riscv_vwmacc_vx_i64m8(vres0m8, *(pIn2 + jj), __riscv_vwadd_vx_i32m4 (vx, 0, l), l);
      vx = __riscv_vslide1down_vx_i16m2(vx, *(pIn1 + jj), l);
    }
    vx = __riscv_vnclip_wx_i16m2(__riscv_vnsra_wx_i32m4(vres0m8, 15, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse16_v_i16m2(pOut, vx, l);
    pOut += l;
  }

  pIn1 = pSrcA + blockSize2;
  for (ii = blockSize3; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e16m2(ii);
    vx = __riscv_vle16_v_i16m2(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i64m8(0, l);
    flag = 0;
    for (jj = 0; jj < blockSize3; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i64m8(vres0m8, *(pIn2 + jj), __riscv_vwadd_vx_i32m4 (vx, 0, l), l);
      if (pIn1 + jj >= pSrcA + srcALen) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 + jj);
      }
      vx = __riscv_vslide1down_vx_i16m2(vx, value, l);
    }
    vx = __riscv_vnclip_wx_i16m2(__riscv_vnsra_wx_i32m4(vres0m8, 15, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse16_v_i16m2(pOut, vx, l);
    pOut += l;
  }
#else
#if defined (RISCV_MATH_DSP)

  const q15_t *pIn1;                                   /* InputA pointer */
  const q15_t *pIn2;                                   /* InputB pointer */
        q15_t *pOut = pDst;                            /* Output pointer */
        q63_t sum, acc0, acc1, acc2, acc3;             /* Accumulators */
  const q15_t *px;                                     /* Intermediate inputA pointer */
  const q15_t *py;                                     /* Intermediate inputB pointer */
  const q15_t *pSrc1;                                  /* Intermediate pointers */
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t j, k, count, blkCnt;                  /* Loop counters */
        uint32_t outBlockSize;
        int32_t inc = 1;                               /* Destination address modifier */
        q31_t x0, x1, x2, x3, c0;
#if defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64)    /* Temporary input variables for holding input and coefficient values */
        q63_t x064, x164, x264, x364, c064;
#endif/* defined (NUCLEI_DSP_N3) || (__RISCV_XLEN == 64) */

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
    outBlockSize = (srcALen * 2U) - 1U;

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

  /* The first loop starts here */
  /* RISCV_MATH_DSP branch */
  while (blockSize1 > 0U)
  {
    /* Accumulator is made zero for every iteration */
    sum = 0;

    /* Apply loop unrolling and compute 4 MACs simultaneously. */
    k = count >> 2U;

    /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
     ** a second loop below computes MACs for the remaining 1 to 3 samples. */
    while (k > 0U)
    {
#if __RISCV_XLEN == 64
      /* x[0] * y[srcBLen - 4] , x[1] * y[srcBLen - 3] , x[2] * y[srcBLen - 2] , x[3] * y[srcBLen - 1]*/
      sum = __SMLALD(read_q15x4_ia ((q15_t **) &px), read_q15x4_ia ((q15_t **) &py), sum);
#else
#ifdef NUCLEI_DSP_N3
      /* x[0] * y[srcBLen - 4] , x[1] * y[srcBLen - 3] , x[2] * y[srcBLen - 2] , x[3] * y[srcBLen - 1]*/
      sum = __RV_DSMALDA(sum, read_q15x4_ia ((q15_t **) &px), read_q15x4_ia ((q15_t **) &py));
#else
      /* Perform the multiply-accumulate */
      /* x[0] * y[srcBLen - 4] , x[1] * y[srcBLen - 3] */
      sum = __SMLALD(read_q15x2_ia ((q15_t **) &px), read_q15x2_ia ((q15_t **) &py), sum);
      /* x[3] * y[srcBLen - 1] , x[2] * y[srcBLen - 2] */
      sum = __SMLALD(read_q15x2_ia ((q15_t **) &px), read_q15x2_ia ((q15_t **) &py), sum);

#endif/* #ifdef NUCLEI_DSP_N3 */
#endif/* __RISCV_XLEN == 64 */
      /* Decrement loop counter */
      k--;
    }

    /* If the count is not a multiple of 4, compute any remaining MACs here.
     ** No loop unrolling is used. */
    k = count & 0x3U;

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
      /* x[0] * y[srcBLen - 1] */
      sum = __SMLALD(*px++, *py++, sum);

      /* Decrement loop counter */
      k--;
    }

    /* Store the result in the accumulator in the destination buffer. */
    *pOut = (q15_t) (__SSAT((sum >> 15), 16));
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

  /* count is the index by which the pointer pIn1 to be incremented */
  count = 0U;

  /* -------------------
   * Stage2 process
   * ------------------*/
  /* Stage2 depends on srcBLen as in this stage srcBLen number of MACS are performed.
   * So, to loop unroll over blockSize2,
   * srcBLen should be greater than or equal to 4 */
  if (srcBLen >= 4U)
  {
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = blockSize2 >> 2U;

    while (blkCnt > 0U)
    {
      /* Set all accumulators to zero */
      acc0 = 0;
      acc1 = 0;
      acc2 = 0;
      acc3 = 0;
      /* read x[0], x[1] samples */
      x0 = read_q15x2 ((q15_t *) px);

      /* read x[1], x[2] samples */
      x1 = read_q15x2 ((q15_t *) px + 1);

      px += 2U;

      /* Apply loop unrolling and compute 4 MACs simultaneously. */
      k = srcBLen >> 2U;

      /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
       ** a second loop below computes MACs for the remaining 1 to 3 samples. */
      do
      {
#if __RISCV_XLEN == 64
        /* Read the first two inputB samples using SIMD:*/
        // y[0], y[1], y[2] and y[3]
        c064 = read_q15x4_ia ((q15_t **) &py);

        /* Read x[2], x[3] */
        x2 = read_q15x2 ((q15_t *) px);

        /* Read x[3], x[4] */
        x3 = read_q15x2 ((q15_t *) px + 1);

        x064 = __RV_PKBB32((uint64_t)x2, (uint64_t)x0);
        acc0 = __SMLALD(x064, c064, acc0);

        x164 = __RV_PKBB32((uint64_t)x3, (uint64_t)x1);
        acc1 = __SMLALD(x164, c064, acc1);

        x0 = read_q15x2 ((q15_t *) px + 2);
        x1 = read_q15x2 ((q15_t *) px + 3);

        px += 4;

        x264 = __RV_PKBB32((uint64_t)x0, (uint64_t)x2);
        acc2 = __SMLALD(x264, c064, acc2);

        x364 = __RV_PKBB32((uint64_t)x1, (uint64_t)x3);
        acc3 = __SMLALD(x364, c064, acc3);
#else
#ifdef NUCLEI_DSP_N3
        /* Read the first two inputB samples using SIMD:*/
        // y[0], y[1], y[2] and y[3]
        c064 = read_q15x4_ia ((q15_t **) &py);

        /* Read x[2], x[3] */
        x2 = read_q15x2 ((q15_t *) px);

        /* Read x[3], x[4] */
        x3 = read_q15x2 ((q15_t *) px + 1);

        x064 = __RV_DPACK32((uint64_t)x2, (uint64_t)x0);
        acc0 = __RV_DSMALDA(acc0, x064, c064);

        x164 = __RV_DPACK32((uint64_t)x3, (uint64_t)x1);
        acc1 = __RV_DSMALDA(acc1, x164, c064);

        x0 = read_q15x2 ((q15_t *) px + 2);
        x1 = read_q15x2 ((q15_t *) px + 3);

        px += 4;

        x264 = __RV_DPACK32((uint64_t)x0, (uint64_t)x2);
        acc2 = __RV_DSMALDA(acc2, x264, c064);

        x364 = __RV_DPACK32((uint64_t)x1, (uint64_t)x3);
        acc3 = __RV_DSMALDA(acc3, x364, c064);
#else
        /* Read the first two inputB samples using SIMD:
         * y[0] and y[1] */
        c0 = read_q15x2_ia ((q15_t **) &py);

        /* acc0 +=  x[0] * y[0] + x[1] * y[1] */
        acc0 = __SMLALD(x0, c0, acc0);

        /* acc1 +=  x[1] * y[0] + x[2] * y[1] */
        acc1 = __SMLALD(x1, c0, acc1);

        /* Read x[2], x[3] */
        x2 = read_q15x2 ((q15_t *) px);

        /* Read x[3], x[4] */
        x3 = read_q15x2 ((q15_t *) px + 1);

        /* acc2 +=  x[2] * y[0] + x[3] * y[1] */
        acc2 = __SMLALD(x2, c0, acc2);

        /* acc3 +=  x[3] * y[0] + x[4] * y[1] */
        acc3 = __SMLALD(x3, c0, acc3);

        /* Read y[2] and y[3] */
        c0 = read_q15x2_ia ((q15_t **) &py);

        /* acc0 +=  x[2] * y[2] + x[3] * y[3] */
        acc0 = __SMLALD(x2, c0, acc0);

        /* acc1 +=  x[3] * y[2] + x[4] * y[3] */
        acc1 = __SMLALD(x3, c0, acc1);

        /* Read x[4], x[5] */
        x0 = read_q15x2 ((q15_t *) px + 2);

        /* Read x[5], x[6] */
        x1 = read_q15x2 ((q15_t *) px + 3);
        px += 4U;

        /* acc2 +=  x[4] * y[2] + x[5] * y[3] */
        acc2 = __SMLALD(x0, c0, acc2);

        /* acc3 +=  x[5] * y[2] + x[6] * y[3] */
        acc3 = __SMLALD(x1, c0, acc3);

#endif/* #ifdef NUCLEI_DSP_N3 */
#endif/* #if __RISCV_XLEN == 64 */
      } while (--k);

      /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = srcBLen & 0x3U;

      if (k == 1U)
      {
        /* Read y[4] */
        c0 = *py;
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
        /* Read y[4], y[5] */
        c0 = read_q15x2 ((q15_t *) py);

        /* Read x[7], x[8] */
        x3 = read_q15x2 ((q15_t *) px);

        /* Read x[9] */
        x2 = read_q15x2 ((q15_t *) px + 1);
        px += 2U;

        /* Perform the multiply-accumulate */
        acc0 = __SMLALD(x0, c0, acc0);
        acc1 = __SMLALD(x1, c0, acc1);
        acc2 = __SMLALD(x3, c0, acc2);
        acc3 = __SMLALD(x2, c0, acc3);
      }

      if (k == 3U)
      {
        /* Read y[4], y[5] */
        c0 = read_q15x2_ia ((q15_t **) &py);

        /* Read x[7], x[8] */
        x3 = read_q15x2 ((q15_t *) px);

        /* Read x[9] */
        x2 = read_q15x2 ((q15_t *) px + 1);

        /* Perform the multiply-accumulate */
        acc0 = __SMLALD(x0, c0, acc0);
        acc1 = __SMLALD(x1, c0, acc1);
        acc2 = __SMLALD(x3, c0, acc2);
        acc3 = __SMLALD(x2, c0, acc3);

        c0 = (*py);

        /* Read y[6] */
        c0 = c0 & 0x0000FFFF;

        /* Read x[10] */
        x3 = read_q15x2 ((q15_t *) px + 2);
        px += 3U;

        /* Perform the multiply-accumulates */
        acc0 = __SMLALDX(x1, c0, acc0);
        acc1 = __SMLALD (x2, c0, acc1);
        acc2 = __SMLALDX(x2, c0, acc2);
        acc3 = __SMLALDX(x3, c0, acc3);
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut = (q15_t) (__SSAT(acc0 >> 15, 16));
      /* Destination pointer is updated according to the address modifier, inc */
      pOut += inc;

      *pOut = (q15_t) (__SSAT(acc1 >> 15, 16));
      pOut += inc;

      *pOut = (q15_t) (__SSAT(acc2 >> 15, 16));
      pOut += inc;

      *pOut = (q15_t) (__SSAT(acc3 >> 15, 16));
      pOut += inc;

      /* Increment the count by 4 as 4 output values are computed */
      count += 4U;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pIn2;

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
        sum += ((q63_t) *px++ * *py++);
        sum += ((q63_t) *px++ * *py++);
        sum += ((q63_t) *px++ * *py++);
        sum += ((q63_t) *px++ * *py++);

        /* Decrement loop counter */
        k--;
      }

      /* If the srcBLen is not a multiple of 4, compute any remaining MACs here.
       ** No loop unrolling is used. */
      k = srcBLen & 0x3U;

      while (k > 0U)
      {
        /* Perform the multiply-accumulates */
        sum += ((q63_t) *px++ * *py++);

        /* Decrement the loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut = (q15_t) (__SSAT(sum >> 15, 16));
      /* Destination pointer is updated according to the address modifier, inc */
      pOut += inc;

      /* Increment count by 1, as one output value is computed */
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
        sum += ((q63_t) *px++ * *py++);

        /* Decrement the loop counter */
        k--;
      }

      /* Store the result in the accumulator in the destination buffer. */
      *pOut = (q15_t) (__SSAT(sum >> 15, 16));
      /* Destination pointer is updated according to the address modifier, inc */
      pOut += inc;

      /* Increment the MAC count */
      count++;

      /* Update the inputA and inputB pointers for next MAC calculation */
      px = pIn1 + count;
      py = pIn2;

      /* Decrement the loop counter */
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
  pSrc1 = (pIn1 + srcALen) - (srcBLen - 1U);
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
    /* Apply loop unrolling and compute 4 MACs simultaneously. */
    k = count >> 2U;

    /* First part of the processing with loop unrolling.  Compute 4 MACs at a time.
     ** a second loop below computes MACs for the remaining 1 to 3 samples. */
    while (k > 0U)
    {
#if  __RISCV_XLEN == 64
      /* sum += x[srcALen - srcBLen + 4] * y[3] , sum += x[srcALen - srcBLen + 3] * y[2]
         sum += x[srcALen - srcBLen + 2] * y[1] , sum += x[srcALen - srcBLen + 1] * y[0] */
      sum = __SMLALD(read_q15x4_ia ((q15_t **) &px), read_q15x4_ia ((q15_t **) &py), sum);
#else
#ifdef NUCLEI_DSP_N3
      /* sum += x[srcALen - srcBLen + 4] * y[3] , sum += x[srcALen - srcBLen + 3] * y[2]
         sum += x[srcALen - srcBLen + 2] * y[1] , sum += x[srcALen - srcBLen + 1] * y[0] */
      sum = __dsmalda(sum, read_q15x4_ia ((q15_t **) &px), read_q15x4_ia ((q15_t **) &py));
#else
      /* Perform the multiply-accumulate */
      /* sum += x[srcALen - srcBLen + 4] * y[3] , sum += x[srcALen - srcBLen + 3] * y[2] */
      sum = __SMLALD(read_q15x2_ia ((q15_t **) &px), read_q15x2_ia ((q15_t **) &py), sum);
      /* sum += x[srcALen - srcBLen + 2] * y[1] , sum += x[srcALen - srcBLen + 1] * y[0] */
      sum = __SMLALD(read_q15x2_ia ((q15_t **) &px), read_q15x2_ia ((q15_t **) &py), sum);

#endif/* #ifdef NUCLEI_DSP_N3 */
#endif/* #if  __RISCV_XLEN == 64 */
      /* Decrement loop counter */
      k--;
    }

    /* If the count is not a multiple of 4, compute any remaining MACs here.
     ** No loop unrolling is used. */
    k = count & 0x3U;

    while (k > 0U)
    {
      /* Perform the multiply-accumulate */
      sum = __SMLALD(*px++, *py++, sum);

      /* Decrement loop counter */
      k--;
    }

    /* Store the result in the accumulator in the destination buffer. */
    *pOut = (q15_t) (__SSAT((sum >> 15), 16));
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

#else /* #if defined (RISCV_MATH_DSP) */

  const q15_t *pIn1 = pSrcA;                           /* InputA pointer */
  const q15_t *pIn2 = pSrcB + (srcBLen - 1U);          /* InputB pointer */
        q63_t sum;                                     /* Accumulators */
        uint32_t i = 0U, j;                            /* Loop counters */
        uint32_t inv = 0U;                             /* Reverse order flag */
        uint32_t tot = 0U;                             /* Length */

  /* The algorithm implementation is based on the lengths of the inputs. */
  /* srcB is always made to slide across srcA. */
  /* So srcBLen is always considered as shorter or equal to srcALen */
  /* But CORR(x, y) is reverse of CORR(y, x) */
  /* So, when srcBLen > srcALen, output pointer is made to point to the end of the output buffer */
  /* and a varaible, inv is set to 1 */
  /* If lengths are not equal then zero pad has to be done to  make the two
   * inputs of same length. But to improve the performance, we include zeroes
   * in the output instead of zero padding either of the the inputs*/
  /* If srcALen > srcBLen, (srcALen - srcBLen) zeroes has to included in the
   * starting of the output buffer */
  /* If srcALen < srcBLen, (srcALen - srcBLen) zeroes has to included in the
   * ending of the output buffer */
  /* Once the zero padding is done the remaining of the output is calcualted
   * using convolution but with the shorter signal time shifted. */

  /* Calculate the length of the remaining sequence */
  tot = ((srcALen + srcBLen) - 2U);

  if (srcALen > srcBLen)
  {
    /* Calculating the number of zeros to be padded to the output */
    j = srcALen - srcBLen;

    /* Initialise the pointer after zero padding */
    pDst += j;
  }

  else if (srcALen < srcBLen)
  {
    /* Initialization to inputB pointer */
    pIn1 = pSrcB;

    /* Initialization to the end of inputA pointer */
    pIn2 = pSrcA + (srcALen - 1U);

    /* Initialisation of the pointer after zero padding */
    pDst = pDst + tot;

    /* Swapping the lengths */
    j = srcALen;
    srcALen = srcBLen;
    srcBLen = j;

    /* Setting the reverse flag */
    inv = 1;
  }

  /* Loop to calculate convolution for output length number of values */
  for (i = 0U; i <= tot; i++)
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
        sum += ((q31_t) pIn1[j] * pIn2[-((int32_t) i - (int32_t) j)]);
      }
    }

    /* Store the output in the destination buffer */
    if (inv == 1)
      *pDst-- = (q15_t) __SSAT((sum >> 15U), 16U);
    else
      *pDst++ = (q15_t) __SSAT((sum >> 15U), 16U);
  }

#endif /* defined (RISCV_MATH_DSP) */
#endif /* defined (RISCV_MATH_VECTOR) */
}

/**
  @} end of Corr group
 */
