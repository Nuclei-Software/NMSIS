/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_float_to_q15.c
 * Description:  Converts the elements of the floating-point vector to Q15 vector
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

#include "dsp/support_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)


/**
  @ingroup groupSupport
 */

/**
 * @defgroup f16_to_x  Convert 16-bit floating point value
 */

/**
  @addtogroup f16_to_x
  @{
 */

/**
  @brief         Converts the elements of the f16 vector to f32 vector.
  @param[in]     pSrc       points to the f16 input vector
  @param[out]    pDst       points to the f32 output vector
  @param[in]     blockSize  number of samples in each vector

 */


RISCV_DSP_ATTRIBUTE void riscv_f16_to_float(
  const float16_t * pSrc,
        float32_t * pDst,
        uint32_t blockSize)
{
    const float16_t *pIn = pSrc;      /* Src pointer */
    uint32_t  blkCnt;           /* loop counter */

    /*
     * Loop over blockSize number of values
     */
    blkCnt = blockSize;

    while (blkCnt > 0U)
    {

        *pDst++ = (float32_t) * pIn++;
        /*
         * Decrement the loop counter
         */
        blkCnt--;
    }
}

/**
  @} end of f16_to_x group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

