/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmin_no_idx_q31.c
 * Description:  Minimum value of absolute values of a Q31 vector
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
  @addtogroup AbsMin
  @{
 */

/**
  @brief         Minimum value of absolute values of a Q31 vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
  @return        none
 */

#if defined(RISCV_MATH_DSP)
void riscv_absmin_no_idx_q31(const q31_t *pSrc, uint32_t blockSize,
                             q31_t *pResult)
{
    q31_t cur_absmin, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;       /* Loop counter */
    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;
    out = (out > 0) ? out : (q31_t)__QSUB(0, out);
        /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = (blockSize - 1U) >> 2U;

    /* Initialize cur_absmin to next consecutive values one by one */
    while (blkCnt > 0U) {
        cur_absmin = *pSrc++;
        /* compare for the extrema value */
        cur_absmin = (cur_absmin > 0) ? cur_absmin : (q31_t)__QSUB(0, cur_absmin);
        if (cur_absmin < out) { /* Update the extrema value and it's index */
            out = cur_absmin;
        }

        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0) ? cur_absmin : (q31_t)__QSUB(0, cur_absmin);
        if (cur_absmin < out) {
            out = cur_absmin;
        }

        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0) ? cur_absmin : (q31_t)__QSUB(0, cur_absmin);
        if (cur_absmin < out) {
            out = cur_absmin;
        }

        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0) ? cur_absmin : (q31_t)__QSUB(0, cur_absmin);
        if (cur_absmin < out) {
            out = cur_absmin;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Loop unrolling: Compute remaining outputs */
    blkCnt = (blockSize - 1U) % 4U;

    while (blkCnt > 0U) {
        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0) ? cur_absmin : (q31_t)__QSUB(0, cur_absmin);
        if (cur_absmin < out) {
            out = cur_absmin;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
    /* Store the extrema value and it's index into destination pointers */
    *pResult = out;
}
#else
void riscv_absmin_no_idx_q31(const q31_t *pSrc, uint32_t blockSize,
                             q31_t *pResult)
{
    q31_t minVal, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;   /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
    blkCnt = blockSize;
    size_t l;
    vint32m8_t v_x, v_zero;
    vint32m1_t v_temp;
    q31_t *pData = pSrc;
    out = 0x7fffffff;
    l = vsetvlmax_e32m8();
    v_zero = vmv_v_x_i32m8(0, l);
    l = vsetvlmax_e32m1();
    v_temp = vmv_v_x_i32m1(out, l);

    for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = vle32_v_i32m8(pData, l);
        pData += l;
        vbool4_t mask = vmslt_vx_i32m8_b4(v_x, 0, l);
        v_x = vssub_vv_i32m8_m(mask, v_x, v_zero, v_x, l);
        minVal =
            vmv_x_s_i32m1_i32(vredmin_vs_i32m8_i32m1(v_temp, v_x, v_temp, l));
        if (minVal < out) {
            out = minVal;
        }
    }
#else

    /* Load first input value that act as reference value for comparision */
    out = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
    pSrc++;
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = (*pSrc > 0) ? *pSrc : ((*pSrc == INT32_MIN) ? INT32_MAX : -*pSrc);
        pSrc++;
        /* compare for the minimum value */
        if (out > minVal) {
            /* Update the minimum value and it's index */
            out = minVal;
        }
        /* Decrement loop counter */
        blkCnt--;
    }
#endif /* defined(RISCV_MATH_VECTOR) */
    /* Store the minimum value and it's index into destination pointers */
    *pResult = out;
}
#endif /* defined(RISCV_MATH_DSP) */
/**
  @} end of AbsMin group
 */