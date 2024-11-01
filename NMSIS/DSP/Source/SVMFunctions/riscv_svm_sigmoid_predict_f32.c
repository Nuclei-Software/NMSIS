/* ----------------------------------------------------------------------
 * Project:      NMSIS DSP Library
 * Title:        riscv_svm_sigmoid_predict_f32.c
 * Description:  SVM Sigmoid Classifier
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
 * @addtogroup sigmoidsvm
 * @{
 */



/**
 * @brief SVM sigmoid prediction
 * @param[in]    S        Pointer to an instance of the rbf SVM structure.
 * @param[in]    in       Pointer to input vector
 * @param[out]   pResult  Decision value
 *
 */

RISCV_DSP_ATTRIBUTE void riscv_svm_sigmoid_predict_f32(
    const riscv_svm_sigmoid_instance_f32 *S,
    const float32_t * in,
    int32_t * pResult)
{
    float32_t sum=S->intercept;
    float32_t dot=0;
    uint32_t i,j;
    const float32_t *pSupport = S->supportVectors;

#if defined(RISCV_MATH_VECTOR)
    size_t blkCnt;
    size_t l;
    vfloat32m8_t v_in, v_support;
    vfloat32m8_t v_dot;
    for (i = 0; i < S->nbOfSupportVectors; i++)
    {
        const float32_t *pIn = in;
        blkCnt = S->vectorDimension;
        l = __riscv_vsetvlmax_e32m8();
        v_dot = __riscv_vfmv_v_f_f32m8(0, l);
        for (; (l = __riscv_vsetvl_e32m8(blkCnt)) > 0; blkCnt -= l)
        {
            v_in = __riscv_vle32_v_f32m8(pIn, l);
            pIn += l;
            v_support = __riscv_vle32_v_f32m8(pSupport, l);
            pSupport += l;
            v_dot = __riscv_vfmacc_vv_f32m8(v_dot, v_in, v_support, l);
        }
        l = __riscv_vsetvl_e32m8(1);
        vfloat32m1_t temp00m1 = __riscv_vfmv_v_f_f32m1(0.0f, l);
        l = __riscv_vsetvlmax_e32m8();
        temp00m1 = __riscv_vfredusum_vs_f32m8_f32m1(v_dot, temp00m1, l);
        dot = __riscv_vfmv_f_s_f32m1_f32(temp00m1);

        sum += S->dualCoefficients[i] * tanhf(S->gamma * dot + S->coef0);
    }
#else
    for(i=0; i < S->nbOfSupportVectors; i++)
    {
        dot=0;
        for(j=0; j < S->vectorDimension; j++)
        {
            dot = dot + in[j]* *pSupport++;
        }
        sum += S->dualCoefficients[i] * tanhf(S->gamma * dot + S->coef0);
    }
#endif /* defined(RISCV_MATH_VECTOR) */
    *pResult=S->classes[STEP(sum)];
}


/**
 * @} end of sigmoidsvm group
 */
