
/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_correlation_distance_f16.c
 * Description:  Correlation distance between two vectors
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

#include "dsp/distance_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

#include <limits.h>
#include <math.h>


/**
  @ingroup FloatDist
 */

/**
  @defgroup Correlation Correlation distance

  Correlation distance
 */

/**
  @addtogroup Correlation
  @{
 */


/**
 * @brief        Correlation distance between two vectors
 *
 * The input vectors are modified in place !
 *
 * @param[in]    pA         First vector
 * @param[in]    pB         Second vector
 * @param[in]    blockSize  vector length
 * @return distance
 *
 */

RISCV_DSP_ATTRIBUTE float16_t riscv_correlation_distance_f16(float16_t *pA,float16_t *pB, uint32_t blockSize)
{
    float16_t ma,mb,pwra,pwrb,dot,tmp;

    riscv_mean_f16(pA, blockSize, &ma);
    riscv_mean_f16(pB, blockSize, &mb);

    riscv_offset_f16(pA, -(_Float16)ma, pA, blockSize);
    riscv_offset_f16(pB, -(_Float16)mb, pB, blockSize);

    riscv_power_f16(pA, blockSize, &pwra);
    riscv_power_f16(pB, blockSize, &pwrb);

    riscv_dot_prod_f16(pA,pB,blockSize,&dot);

    dot = (_Float16)dot / (_Float16)blockSize;
    pwra = (_Float16)pwra / (_Float16)blockSize;
    pwrb = (_Float16)pwrb / (_Float16)blockSize;

    riscv_sqrt_f16((_Float16)pwra * (_Float16)pwrb,&tmp);
 
    return(1.0f16 - (_Float16)dot / (_Float16)tmp);

   
}



/**
 * @} end of Correlation group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */ 

