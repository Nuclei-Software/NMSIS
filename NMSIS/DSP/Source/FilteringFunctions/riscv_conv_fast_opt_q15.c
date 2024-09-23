/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_conv_fast_opt_q15.c
 * Description:  Fast Q15 Convolution
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
  @brief         Convolution of Q15 sequences (fast version).
  @param[in]     pSrcA      points to the first input sequence
  @param[in]     srcALen    length of the first input sequence
  @param[in]     pSrcB      points to the second input sequence
  @param[in]     srcBLen    length of the second input sequence
  @param[out]    pDst       points to the location where the output result is written.  Length srcALen+srcBLen-1
  @param[in]     pScratch1  points to scratch buffer of size max(srcALen, srcBLen) + 2*min(srcALen, srcBLen) - 2
  @param[in]     pScratch2  points to scratch buffer of size min(srcALen, srcBLen

  @par           Scaling and Overflow Behavior
                   This fast version uses a 32-bit accumulator with 2.30 format.
                   The accumulator maintains full precision of the intermediate multiplication results
                   but provides only a single guard bit. There is no saturation on intermediate additions.
                   Thus, if the accumulator overflows it wraps around and distorts the result.
                   The input signals should be scaled down to avoid intermediate overflows.
                   Scale down the inputs by log2(min(srcALen, srcBLen)) (log2 is read as log to the base 2) times to avoid overflows,
                   as maximum of min(srcALen, srcBLen) number of additions are carried internally.
                   The 2.30 accumulator is right shifted by 15 bits and then saturated to 1.15 format to yield the final result.

  @remark
                   Refer to \ref riscv_conv_q15() for a slower implementation of this function which uses 64-bit accumulation to avoid wrap around distortion.
 */

RISCV_DSP_ATTRIBUTE void riscv_conv_fast_opt_q15(
  const q15_t * pSrcA,
        uint32_t srcALen,
  const q15_t * pSrcB,
        uint32_t srcBLen,
        q15_t * pDst,
        q15_t * pScratch1,
        q15_t * pScratch2)
{
        q31_t acc0;                                    /* Accumulators */
  const q15_t *pIn1;                                   /* InputA pointer */
  const q15_t *pIn2;                                   /* InputB pointer */
        q15_t *pOut = pDst;                            /* Output pointer */
        q15_t *pScr1 = pScratch1;                      /* Temporary pointer for scratch1 */
        q15_t *pScr2 = pScratch2;                      /* Temporary pointer for scratch1 */
  const q15_t *px;                                     /* Intermediate inputA pointer */
        q15_t *py;                                     /* Intermediate inputB pointer */
        uint32_t j, k, blkCnt;                         /* Loop counter */
        uint32_t tapCnt;                               /* Loop count */

#if defined (RISCV_MATH_VECTOR)
        uint32_t blockSize1, blockSize2, blockSize3;   /* Loop counters */
        uint32_t ii, jj, kk;

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
  vint32m8_t vres0m8;
  vint16m4_t vx;
  q15_t value = 0;
  uint32_t flag = 0;

  blockSize1 = srcBLen - 1U;
  blockSize2 = srcALen - (srcBLen - 1U);
  blockSize3 = blockSize1;

  for (ii = blockSize1; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e16m4(ii);
    vx = __riscv_vle16_v_i16m4(pIn1, l);
    vres0m8 = __riscv_vmv_v_x_i32m8(0.0, l);
    flag = 0;
    for (jj = 0; jj < blockSize1; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pIn2 + jj), vx, l);
      if (pIn1 - jj <= pSrcA) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 - jj - 1);
      }
      vx = __riscv_vslide1up_vx_i16m4(vx, value, l);
    }
    vx = __riscv_vnclip_wx_i16m4(__riscv_vsra_vx_i32m8(vres0m8, 15, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse16_v_i16m4(pOut, vx, l);
    pOut += l;
    pIn1 += l;
  }

  pIn2 += srcBLen - 1;
  pIn1 = pSrcA;
  for (ii = blockSize2; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e16m4(ii);
    vx = __riscv_vle16_v_i16m4(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
    for (jj = 0; jj < srcBLen; jj++)
    {
      vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pIn2 - jj), vx, l);
      vx = __riscv_vslide1down_vx_i16m4(vx, *(pIn1 + jj), l);
    }
    vx = __riscv_vnclip_wx_i16m4(__riscv_vsra_vx_i32m8(vres0m8, 15, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse16_v_i16m4(pOut, vx, l);
    pOut += l;
  }

  pIn1 = pSrcA + blockSize2;
  for (ii = blockSize3; ii > 0; ii -= l)
  {
    l = __riscv_vsetvl_e16m4(ii);
    vx = __riscv_vle16_v_i16m4(pIn1, l);
    pIn1 += l;
    vres0m8 = __riscv_vmv_v_x_i32m8(0, l);
    flag = 0;
    for (jj = 0; jj < blockSize3; jj++)
    {
      if (flag >= l)
        break;
      vres0m8 = __riscv_vwmacc_vx_i32m8(vres0m8, *(pIn2 - jj), vx, l);
      if (pIn1 + jj >= pSrcA + srcALen) {
        value = 0;
        flag++;
      } else {
        value = *(pIn1 + jj);
      }
      vx = __riscv_vslide1down_vx_i16m4(vx, value, l);
    }
    vx = __riscv_vnclip_wx_i16m4(__riscv_vsra_vx_i32m8(vres0m8, 15, l), 0, __RISCV_VXRM_RNU, l);
    __riscv_vse16_v_i16m4(pOut, vx, l);
    pOut += l;
  }

#else

#if defined (RISCV_MATH_LOOPUNROLL)
        q31_t acc1, acc2, acc3;                        /* Accumulators */
        q31_t x1, x2, x3;                              /* Temporary variables to hold state and coefficient values */
        q31_t y1, y2;                                  /* State variables */
#endif


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

  /* Pointer to take end of scratch2 buffer */
  pScr2 = pScratch2 + srcBLen - 1;

  /* points to smaller length sequence */
  px = pIn2;

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  k = srcBLen >> 2U;

  /* Copy smaller length input sequence in reverse order into second scratch buffer */
  while (k > 0U)
  {
    /* copy second buffer in reversal manner */
    *pScr2-- = *px++;
    *pScr2-- = *px++;
    *pScr2-- = *px++;
    *pScr2-- = *px++;

    /* Decrement loop counter */
    k--;
  }

  /* Loop unrolling: Compute remaining outputs */
  k = srcBLen & 0x3U;

#else

  /* Initialize k with number of samples */
  k = srcBLen;

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  while (k > 0U)
  {
    /* copy second buffer in reversal manner for remaining samples */
    *pScr2-- = *px++;

    /* Decrement loop counter */
    k--;
  }
  /* Initialze temporary scratch pointer */
  pScr1 = pScratch1;

  /* Assuming scratch1 buffer is aligned by 32-bit */
  /* Fill (srcBLen - 1U) zeros in scratch1 buffer */
  riscv_fill_q15(0, pScr1, (srcBLen - 1U));

  /* Update temporary scratch pointer */
  pScr1 += (srcBLen - 1U);

  /* Copy bigger length sequence(srcALen) samples in scratch1 buffer */

  /* Copy (srcALen) samples in scratch buffer */
  riscv_copy_q15(pIn1, pScr1, srcALen);

  /* Update pointers */
  pScr1 += srcALen;


  /* Fill (srcBLen - 1U) zeros at end of scratch buffer */
  riscv_fill_q15(0, pScr1, (srcBLen - 1U));

  /* Update pointer */
  pScr1 += (srcBLen - 1U);

  /* Temporary pointer for scratch2 */
  py = pScratch2;


  /* Initialization of pIn2 pointer */
  pIn2 = py;

#if defined (RISCV_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = (srcALen + srcBLen - 1U) >> 2;

  while (blkCnt > 0)
  {
    /* Initialze temporary scratch pointer as scratch1 */
    pScr1 = pScratch1;

    /* Clear Accumlators */
    acc0 = 0;
    acc1 = 0;
    acc2 = 0;
    acc3 = 0;

    /* Read two samples from scratch1 buffer */
    x1 = read_q15x2_ia (&pScr1);

    /* Read next two samples from scratch1 buffer */
    x2 = read_q15x2_ia (&pScr1);

    tapCnt = (srcBLen) >> 2U;

    while (tapCnt > 0U)
    {

      /* Read four samples from smaller buffer */
      y1 = read_q15x2_ia ((q15_t **) &pIn2);
      y2 = read_q15x2_ia ((q15_t **) &pIn2);

      /* multiply and accumulate */
      acc0 = __SMLAD(x1, y1, acc0);
      acc2 = __SMLAD(x2, y1, acc2);

      /* pack input data */
      x3 = __PKHBT(x2, x1, 0);

      /* multiply and accumulate */
      acc1 = __SMLADX(x3, y1, acc1);

      /* Read next two samples from scratch1 buffer */
      x1 = read_q15x2_ia (&pScr1);

      /* multiply and accumulate */
      acc0 = __SMLAD(x2, y2, acc0);
      acc2 = __SMLAD(x1, y2, acc2);

      /* pack input data */
      x3 = __PKHBT(x1, x2, 0);

      acc3 = __SMLADX(x3, y1, acc3);
      acc1 = __SMLADX(x3, y2, acc1);

      x2 = read_q15x2_ia (&pScr1);

      x3 = __PKHBT(x2, x1, 0);

      acc3 = __SMLADX(x3, y2, acc3);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Update scratch pointer for remaining samples of smaller length sequence */
    pScr1 -= 4U;

    /* apply same above for remaining samples of smaller length sequence */
    tapCnt = (srcBLen) & 3U;

    while (tapCnt > 0U)
    {
      /* accumulate the results */
      acc0 += (*pScr1++ * *pIn2);
      acc1 += (*pScr1++ * *pIn2);
      acc2 += (*pScr1++ * *pIn2);
      acc3 += (*pScr1++ * *pIn2++);

      pScr1 -= 3U;

      /* Decrement loop counter */
      tapCnt--;
    }

    blkCnt--;

    /* Store the results in the accumulators in the destination buffer. */
    write_q15x2_ia (&pOut, __PKHBT(__SSAT((acc0 >> 15), 16), __SSAT((acc1 >> 15), 16), 16));
    write_q15x2_ia (&pOut, __PKHBT(__SSAT((acc2 >> 15), 16), __SSAT((acc3 >> 15), 16), 16));

    /* Initialization of inputB pointer */
    pIn2 = py;

    pScratch1 += 4U;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = (srcALen + srcBLen - 1U) & 0x3;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = (srcALen + srcBLen - 1U);

#endif /* #if defined (RISCV_MATH_LOOPUNROLL) */

  /* Calculate convolution for remaining samples of Bigger length sequence */
  while (blkCnt > 0)
  {
    /* Initialze temporary scratch pointer as scratch1 */
    pScr1 = pScratch1;

    /* Clear Accumlators */
    acc0 = 0;

    tapCnt = (srcBLen) >> 1U;

    while (tapCnt > 0U)
    {

      /* Read next two samples from scratch1 buffer */
      acc0 += (*pScr1++ * *pIn2++);
      acc0 += (*pScr1++ * *pIn2++);

      /* Decrement loop counter */
      tapCnt--;
    }

    tapCnt = (srcBLen) & 1U;

    /* apply same above for remaining samples of smaller length sequence */
    while (tapCnt > 0U)
    {

      /* accumulate the results */
      acc0 += (*pScr1++ * *pIn2++);

      /* Decrement loop counter */
      tapCnt--;
    }

    blkCnt--;

    /* The result is in 2.30 format.  Convert to 1.15 with saturation.
       Then store the output in the destination buffer. */
    *pOut++ = (q15_t) (__SSAT((acc0 >> 15), 16));

    /* Initialization of inputB pointer */
    pIn2 = py;

    pScratch1 += 1U;
  }
#endif /*defined (RISCV_MATH_VECTOR)*/
}

/**
  @} end of Conv group
 */
