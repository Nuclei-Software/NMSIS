/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_logsumexp_f16.c
 * Description:  LogSumExp
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

#include "dsp/statistics_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

#include <limits.h>
#include <math.h>


/**
 * @addtogroup LogSumExp
 * @{
 */


/**
 * @brief Computation of the LogSumExp
 *
 * In probabilistic computations, the dynamic of the probability values can be very
 * wide because they come from gaussian functions.
 * To avoid underflow and overflow issues, the values are represented by their log.
 * In this representation, multiplying the original exp values is easy : their logs are added.
 * But adding the original exp values is requiring some special handling and it is the
 * goal of the LogSumExp function.
 *
 * If the values are x1...xn, the function is computing:
 *
 * ln(exp(x1) + ... + exp(xn)) and the computation is done in such a way that
 * rounding issues are minimised.
 *
 * The max xm of the values is extracted and the function is computing:
 * xm + ln(exp(x1 - xm) + ... + exp(xn - xm))
 *
 * @param[in]  *in         Pointer to an array of input values.
 * @param[in]  blockSize   Number of samples in the input array.
 * @return LogSumExp
 *
 */

RISCV_DSP_ATTRIBUTE float16_t riscv_logsumexp_f16(const float16_t *in, uint32_t blockSize)
{
    _Float16 maxVal;
    _Float16 tmp;
    const float16_t *pIn;
    uint32_t blkCnt;
    _Float16 accum;
 
    pIn = in;
    blkCnt = blockSize;

#if defined(RISCV_MATH_VECTOR)
     maxVal = *pIn;
     uint32_t blkCnt_v = blkCnt;                               /* Loop counter */
     size_t l;
     vfloat16m8_t v_x, v_y;
     vfloat16m1_t v_temp;
     l = __riscv_vsetvl_e16m1(1);
     v_temp = __riscv_vfmv_s_f_f16m1(maxVal, l);
     for (; (l = __riscv_vsetvl_e16m8(blkCnt_v)) > 0; blkCnt_v -= l) {
       v_x = __riscv_vle16_v_f16m8(pIn, l);
       pIn += l;
       v_temp = __riscv_vfredmax_vs_f16m8_f16m1(v_x, v_temp, l);
     }
     maxVal = __riscv_vfmv_f_s_f16m1_f16(v_temp);
#else
    maxVal = *pIn++;
    blkCnt--;
    while(blkCnt > 0)
    {
       tmp = *pIn++;

       if (tmp > maxVal)
       {
          maxVal = tmp;
       }
       blkCnt--;
    
    }
#endif /* #if defined(RISCV_MATH_VECTOR) */

    blkCnt = blockSize;
    pIn = in;
    accum = 0;
    while(blkCnt > 0)
    {
       tmp = *pIn++;
       accum += (_Float16)expf((float32_t)((_Float16)tmp - (_Float16)maxVal));
       blkCnt--;
    
    }
    accum = (_Float16)maxVal + (_Float16)logf((float32_t)accum);

    return(accum);
}

/**
 * @} end of LogSumExp group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

