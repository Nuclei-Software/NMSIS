/*
 * Copyright (C) 2010-2018 Arm Limited or its affiliates. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_relu_q15.c
 * Description:  Q15 version of ReLU
 *
 * $Date:        17. January 2018
 * $Revision:    V.1.0.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_math.h"
#include "riscv_nnfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup Acti
 * @{
 */

  /**
   * @brief Q15 RELU function
   * @param[in,out]   data        pointer to input
   * @param[in]       size        number of elements
   * @return none.
   * 
   * @details
   *
   * Optimized relu with QSUB instructions.
   *
   */

void riscv_relu_q15(q15_t * data, uint16_t size)
{

#if defined (RISCV_MATH_DSP)
    /* Run the following code for RISC-V Core with DSP enabled */

    q15_t    *pIn = data;
    q15_t    *pOut = data;
    q31_t     in;
    q31_t     buf;
    q31_t     mask;
    q31_t      zero;
#if __RISCV_XLEN == 64
    uint16_t  i = size >> 2;
    q63_t in64;
    while (i)
    {   
        in64 = *__SIMD64(pIn)++;

        *__SIMD64(pOut)++ = __RV_SMAX16 (in64,zero);

        i--;
    }
    i = size & 0x3;
    while (i)
    {
        if (*pIn < 0)
        {
            *pIn = 0;
        }
        pIn++;
        i--;
    }
#else
    uint16_t  i = size >> 1;
    while (i)
    {   


        // in = *__SIMD32(pIn)++;

        //*__SIMD32(pOut)++ = __SMAX8 (in,zero);
    
        in = *__SIMD32(pIn)++;

        /* extract the first bit */
        //buf = __ROR(in & 0x80008000, 15);

        /* if MSB=1, mask will be 0xFF, 0x0 otherwise */
       // mask = __QSUB16(0x00000000, buf);
        *__SIMD32(pOut)++ = __RV_SMAX16 (in,zero);
       // *__SIMD32(pOut)++ = in & (~mask);
        i--;
    }

    if (size & 0x1)
    {
        if (*pIn < 0)
        {
            *pIn = 0;
        }
        pIn++;
    }
#endif /* __RISCV_XLEN == 64 */
#else
    /* Run the following code as reference implementation for RISC-V Core without DSP */
    uint16_t  i;

    for (i = 0; i < size; i++)
    {
        if (data[i] < 0)
            data[i] = 0;
    }

#endif                          /* RISCV_MATH_DSP */

}

/**
 * @} end of Acti group
 */
