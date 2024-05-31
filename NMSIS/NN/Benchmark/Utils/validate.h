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

static inline void generate_rand_s8(int8_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (int8_t)(rand() % RAND_MAX - RAND_MAX / 2);
    }
}

static inline void generate_rand_s16(int16_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (int16_t)(rand() % RAND_MAX - RAND_MAX / 2);
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

static inline int validate_s16(int16_t *act, const int16_t *ref, int size)
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
        else
        {
            // printf("PASS at pos %d: %d\r\n", i, act[i]);
        }
    }

    if (!test_passed)
    {
        printf("%d of %d failed\r\n", count, total);
    }

    return test_passed;
}
