/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmax_no_idx_q7.c
 * Description:  Maximum value of absolute values of a Q7 vector
 *
 * $Date:        16 November 2021
 * $Revision:    V1.10.0
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

#include "dsp/statistics_functions.h"

/**
  @ingroup groupStats
 */

/**
  @addtogroup AbsMax
  @{
 */

/**
  @brief         Maximum value of absolute values of a Q7 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    maximum value returned here
  @return        none
 */

#if defined(RISCV_MATH_DSP)
void riscv_absmax_no_idx_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult)
{
    q7_t cur_absmax, out;                /* Temporary variables to store the output value. */
    uint32_t blkCnt;                     /* Loop counter */

    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;
    out = (out > 0) ? out : (q7_t)__QSUB8(0, out);


    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = (blockSize - 1U) >> 2U;

    while (blkCnt > 0U) {
        /* Initialize cur_absmax to next consecutive values one by one */
        cur_absmax = *pSrc++;
        cur_absmax = (cur_absmax > 0) ? cur_absmax : (q7_t)__QSUB8(0, cur_absmax);
        /* compare for the extrema value */
        if (cur_absmax > out) {
            /* Update the extrema value and it's index */
            out = cur_absmax;
        }

        cur_absmax = *pSrc++;
        cur_absmax = (cur_absmax > 0) ? cur_absmax : (q7_t)__QSUB8(0, cur_absmax);
        if (cur_absmax > out)
        {
            out = cur_absmax;
        }

        cur_absmax = *pSrc++;
        cur_absmax = (cur_absmax > 0) ? cur_absmax : (q7_t)__QSUB8(0, cur_absmax);
        if (cur_absmax > out) {
            out = cur_absmax;
        }

        cur_absmax = *pSrc++;
        cur_absmax = (cur_absmax > 0) ? cur_absmax : (q7_t)__QSUB8(0, cur_absmax);
        if (cur_absmax > out) {
            out = cur_absmax;
        }

    /* Decrement loop counter */
        blkCnt--;
   }
  /* Loop unrolling: Compute remaining outputs */
    blkCnt = (blockSize - 1U) % 4U;

    while (blkCnt > 0U) {
        cur_absmax = *pSrc++;
        cur_absmax = (cur_absmax > 0) ? cur_absmax : (q7_t)__QSUB8(0, cur_absmax);
        if (cur_absmax > out) {
            out = cur_absmax;
        }

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store the extrema value and it's index into destination pointers */
    *pResult = out;
}
#else
void riscv_absmax_no_idx_q7(
  const q7_t * pSrc,
        uint32_t blockSize,
        q7_t * pResult)
{
    q7_t maxVal, out;                    /* Temporary variables to store the output value. */
    uint32_t blkCnt;                     /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
     blkCnt = blockSize;
    size_t l;
    vint8m8_t v_x, v_max, v_zero;
    vint8m1_t v_temp;
    l = vsetvlmax_e8m8();
    v_zero = vmv_v_x_i8m8(0, l);
    q7_t *pData = pSrc;
    out = *pData;
    l = vsetvlmax_e8m1();
    v_temp = vmv_s_x_i8m1(v_temp, 0, l);
    for (; (l = vsetvl_e8m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = vle8_v_i8m8(pData, l);
        pData += l;
        vbool1_t mask = vmslt_vx_i8m8_b1(v_x, 0, l);
        v_x = vssub_vv_i8m8_m(mask, v_x, v_zero, v_x, l);
        maxVal = vmv_x_s_i8m1_i8(vredmax_vs_i8m8_i8m1(v_temp, v_x, v_temp, l));
        if (maxVal > out) {
            out = maxVal;
        }
    }
#else

    /* Load first input value that act as reference value for comparision */
    out = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
    pSrc++;

    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize maxVal to the next consecutive values one by one */
        maxVal = (*pSrc > 0) ? *pSrc : ((*pSrc == (q7_t) 0x80) ? (q7_t) 0x7f : -*pSrc);
        pSrc++;

        /* compare for the maximum value */
        if (out < maxVal) {
            /* Update the maximum value and it's index */
            out = maxVal;
        }

        /* Decrement loop counter */
        blkCnt--;
    }
#endif  /* defined(RISCV_MATH_VECTOR) */
    /* Store the maximum value and it's index into destination pointers */
    *pResult = out;
}

#endif /* defined(RISCV_MATH_DSP) */
/**
  @} end of AbsMax group
 */