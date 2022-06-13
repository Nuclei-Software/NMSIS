/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_absmin_no_idx_f32.c
 * Description:  Minimum value of absolute values of a floating-point vector
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
  @brief         Minimum value of absolute values of a floating-point vector.
  @param[in]     pSrc       points to the input vector
  @param[in]     blockSize  number of samples in input vector
  @param[out]    pResult    minimum value returned here
  @return        none
 */

#if defined(RISCV_MATH_LOOPUNROLL)
void riscv_absmin_no_idx_f32(const float32_t *pSrc, uint32_t blockSize,
                             float32_t *pResult)
{
    float32_t cur_absmin, out;   /* Temporary variables to store the output value. */
    uint32_t blkCnt;             /* Loop counter */
    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;
    out = (out > 0.0f) ? out : -out;
    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = (blockSize - 1U) >> 2U;

    while (blkCnt > 0U) {
        /* Initialize cur_absmin to next consecutive values one by one */
        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0.0f) ? cur_absmin : -cur_absmin;
        /* compare for the extrema value */
        if (cur_absmin < out) {
            /* Update the extrema value and it's index */
            out = cur_absmin;
        }

        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0.0f) ? cur_absmin : -cur_absmin;
        if (cur_absmin < out) {
            out = cur_absmin;
        }

        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0.0f) ? cur_absmin : -cur_absmin;
        if (cur_absmin < out) {
            out = cur_absmin;
        }

        cur_absmin = *pSrc++;
        cur_absmin = (cur_absmin > 0.0f) ? cur_absmin : -cur_absmin;
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
        cur_absmin = (cur_absmin > 0.0f) ? cur_absmin : -cur_absmin;
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
void riscv_absmin_no_idx_f32(const float32_t *pSrc, uint32_t blockSize,
                             float32_t *pResult)
{
    float32_t minVal, out; /* Temporary variables to store the output value. */
    uint32_t blkCnt;       /* Loop counter */

#if defined(RISCV_MATH_VECTOR)
    blkCnt = blockSize;
    float32_t temp_min;
    size_t l;
    vfloat32m8_t v_x;
    vfloat32m1_t v_temp;
    float32_t *pIN = pSrc;
    out = fabsf(*pIN);
    l = vsetvl_e32m1(1);
    v_temp = vfmv_v_f_f32m1(out, l);
    for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l) {
        v_x = vle32_v_f32m8(pIN, l);
        pIN += l;
        v_x = vfsgnjx_vv_f32m8(v_x, v_x, l);
        temp_min = vfmv_f_s_f32m1_f32(vfredmin_vs_f32m8_f32m1(v_temp, v_x, v_temp, l));
        if (temp_min < out) {
            out = temp_min;
        }
    }
#else

    /* Load first input value that act as reference value for comparision */
    out = fabsf(*pSrc++);
    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);

    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = fabsf(*pSrc++);

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
#endif /* defined(RISCV_MATH_LOOPUNROLL) */
/**
  @} end of AbsMin group
 */
