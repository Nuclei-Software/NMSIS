/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_svm_polynomial_predict_f16.c
 * Description:  SVM Polynomial Classifier
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

#include "dsp/svm_functions_f16.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

#include <limits.h>
#include <math.h>


/*

_Float16 is not supported in g++ so we avoid putting _Float16 definitions
in the public headers.

This function should at some point be moved in FastMath.

*/
__STATIC_INLINE float16_t riscv_exponent_f16(float16_t x, int32_t nb)
{
    float16_t r = x;
    nb --;
    while(nb > 0)
    {
        r = (_Float16)r * (_Float16)x;
        nb--;
    }
    return(r);
}

/**
 * @addtogroup polysvm
 * @{
 */






/**
 * @brief SVM polynomial prediction
 * @param[in]    S          Pointer to an instance of the polynomial SVM structure.
 * @param[in]    in         Pointer to input vector
 * @param[out]   pResult    Decision value
 * @return none.
 *
 */
void riscv_svm_polynomial_predict_f16(
    const riscv_svm_polynomial_instance_f16 *S,
    const float16_t * in,
    int32_t * pResult)
{
    _Float16 sum=S->intercept;
    _Float16 dot=0;
    uint32_t i,j;
    const float16_t *pSupport = S->supportVectors;

#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt;
    size_t l;
    vfloat16m8_t v_in, v_support;
    vfloat16m1_t v_dot;
    const float16_t *pIn;
    for (i = 0; i < S->nbOfSupportVectors; i++)
    {
        pIn = in;
        blkCnt = S->vectorDimension;
        l = __riscv_vsetvl_e16m1(1);
        v_dot = __riscv_vfmv_s_f_f16m1(0, l);
        for (; (l = __riscv_vsetvl_e16m8(blkCnt)) > 0; blkCnt -= l)
        {
            v_in = __riscv_vle16_v_f16m8(pIn, l);
            pIn += l;
            v_support = __riscv_vle16_v_f16m8(pSupport, l);
            pSupport += l;
            v_dot = __riscv_vfredusum_vs_f16m8_f16m1(__riscv_vfmul_vv_f16m8(v_in, v_support, l), v_dot, l);
        }
        dot = __riscv_vfmv_f_s_f16m1_f16(v_dot);
        sum += S->dualCoefficients[i] * riscv_exponent_f16(S->gamma * dot + S->coef0, S->degree);
    }
#else

    for(i=0; i < S->nbOfSupportVectors; i++)
    {
        dot=0;
        for(j=0; j < S->vectorDimension; j++)
        {
            dot = (_Float16)dot + (_Float16)in[j]* (_Float16)*pSupport++;
        }
        sum += (_Float16)S->dualCoefficients[i] * (_Float16)riscv_exponent_f16((_Float16)S->gamma * (_Float16)dot + (_Float16)S->coef0, S->degree);
    }
#endif /* defined(RISCV_MATH_VECTOR) */
    *pResult=S->classes[STEP(sum)];
}


/**
 * @} end of polysvm group
 */

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
