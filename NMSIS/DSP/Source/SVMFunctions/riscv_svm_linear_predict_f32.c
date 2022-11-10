/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_svm_linear_predict_f32.c
 * Description:  SVM Linear Classifier
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

#include "dsp/svm_functions.h"
#include <limits.h>
#include <math.h>


/**
 * @addtogroup linearsvm
 * @{
 */


/**
 * @brief SVM linear prediction
 * @param[in]    S          Pointer to an instance of the linear SVM structure.
 * @param[in]    in         Pointer to input vector
 * @param[out]   pResult    Decision value
 * @return none.
 *
 */
void riscv_svm_linear_predict_f32(
    const riscv_svm_linear_instance_f32 *S,
    const float32_t * in,
    int32_t * pResult)
{
    float32_t sum=S->intercept;
    float32_t dot=0;
    uint32_t i,j;
    const float32_t *pSupport = S->supportVectors;

#if defined(RISCV_MATH_VECTOR)
    uint32_t blkCnt;
    size_t l;
    vfloat32m8_t v_in, v_support;
    vfloat32m1_t v_dot;             /* init v_dot data */
    const float32_t *pIn;
    for (i = 0; i < S->nbOfSupportVectors; i++)
    {
        dot = 0;
        blkCnt = S->vectorDimension;
        l = vsetvl_e32m1(1);
        v_dot = vfmv_s_f_f32m1(v_dot, 0, l);
        pIn = in;
        for (; (l = vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
        {
            v_in = vle32_v_f32m8(pIn, l);
            pIn += l;
            v_support = vle32_v_f32m8(pSupport, l);
            pSupport += l;
            v_dot = vfredosum_vs_f32m8_f32m1(v_dot, vfmul_vv_f32m8(v_in, v_support, l), v_dot, l);
        }
        dot += vfmv_f_s_f32m1_f32(v_dot);
        sum += S->dualCoefficients[i] * dot;
    }
#else

    for(i=0; i < S->nbOfSupportVectors; i++)
    {
        dot=0;
        for(j=0; j < S->vectorDimension; j++)
        {
            dot = dot + in[j]* *pSupport++;
        }
        sum += S->dualCoefficients[i] * dot;
    }
#endif /* defined(RISCV_MATH_VECTOR) */
    *pResult=S->classes[STEP(sum)];
}

/**
 * @} end of linearsvm group
 */
