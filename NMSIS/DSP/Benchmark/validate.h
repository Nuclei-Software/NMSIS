/*
 * SPDX-FileCopyrightText: Copyright 2010-2022 Arm Limited and/or its affiliates <open-source-office@riscv.com>
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

#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

#define TEST_ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            printf("Assertion failed: %s, file %s, line %d\n", #condition, __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("Assertion failed: %s != %s, file %s, line %d\n", #expected, #actual, __FILE__, __LINE__); \
        } \
    } while(0)


static inline void do_srand(void)
{
    unsigned long randvar = __RV_CSR_READ(CSR_MCYCLE);
    srand(randvar);
   // printf("srandvar is %d\n", randvar);
}

static void generate_rand_q7(q7_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (q7_t)(rand() % Q7_MAX - Q7_MAX / 2);
    }
}

static void generate_rand_q15(q15_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (q15_t)(rand() % Q15_MAX - Q15_MAX / 2);
    }
}

static void generate_rand_q31(q31_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (q31_t)(rand() % Q31_MAX - Q31_MAX / 2);
    }
}

#if defined (RISCV_FLOAT16_SUPPORTED)
static void generate_rand_f16(float16_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (float16_t)((rand() % Q15_MAX - Q15_MAX / 2) * 1.0 / Q15_MAX);
    }
}

static void generate_posi_def_symme_f16(const riscv_matrix_instance_f16 *pSrc, riscv_matrix_instance_f16 *pUnitMat,
                                riscv_matrix_instance_f16 *pDot,  riscv_matrix_instance_f16 *pDst)
{
    int dim = pSrc->numRows;
    riscv_mat_trans_f16(pSrc, pDst);
    riscv_mat_mult_f16(pDst, pSrc, pDot);
    riscv_mat_add_f16(pDot, pUnitMat, pDst);
}
#endif

static void generate_posi_def_symme_f32(const riscv_matrix_instance_f32 *pSrc, riscv_matrix_instance_f32 *pUnitMat,
                                riscv_matrix_instance_f32 *pDot,  riscv_matrix_instance_f32 *pDst)
{
    int dim = pSrc->numRows;
    riscv_mat_trans_f32(pSrc, pDst);
    riscv_mat_mult_f32(pDst, pSrc, pDot);
    riscv_mat_add_f32(pDot, pUnitMat, pDst);
}

static void generate_rand_f32(float32_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (float32_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
}

static void generate_rand_f64(float64_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (float64_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
}

static inline int validate(int8_t *act, const int8_t *ref, int size)
{
    int test_passed = true;
    int count = 0;
    int total = 0;

    for (int i = 0; i < size; ++i)
    {
        total++;
        if (act[i] != ref[i])
        {
            count++;
            printf("ERROR at pos %d: Act: %d Ref: %d\r\n", i, act[i], ref[i]);
            test_passed = false;
        }
    }

    if (!test_passed)
    {
        printf("%d of %d failed\r\n", count, total);
    }

    return test_passed;
}
