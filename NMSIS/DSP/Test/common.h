//
// Created by lujun on 7/13/19.
//

#ifndef DSP_COMMON_H
#define DSP_COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nmsis_bench.h"

/* The threshold */
#define DELTAF64 (0.05f)
#define DELTAF32 (0.2f)
#define DELTAQ63 (1)
#define DELTAQ31 (2)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

#define DELTAQ15_LOW_PRECISION (5)
#define DELTAQ31_LOW_PRECISION (64)
#define DELTAF32_LOW_PRECISION (2.0f)

// #define LOG_DEBUG

int8_t verify_results_q7(q7_t * ref, q7_t * opt, int length)
{
    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > DELTAQ7)
        {
            printf("Q7 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q7 Output at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_q15(q15_t * ref, q15_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > DELTAQ15)
        {
            printf("Q15 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q15 Output at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_q31(q31_t * ref, q31_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > DELTAQ31)
        {
            printf("Q31 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q31 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_q63(q63_t * ref, q63_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (labs(ref[i] - opt[i]) > DELTAQ63)
        {
            printf("Q63 Output mismatch at %d, expected %lld, actual %lld\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q63 Output at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_q15_low_precision(q15_t * ref, q15_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > DELTAQ15_LOW_PRECISION)
        {
            printf("Q15 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q15 Output at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_q31_low_precision(q31_t * ref, q31_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > DELTAQ31_LOW_PRECISION)
        {
            printf("Q31 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q31 Output at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_q31_great_precision(q31_t * ref, q31_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > DELTAQ31_LOW_PRECISION + 300)
        {
            printf("Q31 Output mismatch at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("Q31 Output at %d, expected %d, actual %d\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_f32(float32_t * ref, float32_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (fabs(ref[i] - opt[i]) > DELTAF32)
        {
            printf("f32 Output mismatch at %d, expected %f, actual %f\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("f32 Output mismatch at %d, expected %f, actual %f\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_f32_low_precision(float32_t * ref, float32_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (fabs(ref[i] - opt[i]) > DELTAF32_LOW_PRECISION)
        {
            printf("f32 Output mismatch at %d, expected %f, actual %f\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("f32 Output mismatch at %d, expected %f, actual %f\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_f64(float64_t * ref, float64_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (fabs(ref[i] - opt[i]) > DELTAF64)
        {
            printf("f64 Output mismatch at %d, expected %f, actual %f\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("f64 Output at %d, expected %f, actual %f\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_u8(uint8_t * ref, uint8_t * opt, int length)
{
    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > (uint8_t)DELTAQ7)
        {
            printf("U8 Output mismatch at %d, expected %u, actual %u\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("U8 Output at %d, expected %2x, actual %2x\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_u16(uint16_t * ref, uint16_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > (uint16_t)DELTAQ15)
        {
            printf("U16 Output mismatch at %d, expected %u, actual %u\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("U16 Output at %d, expected %u, actual %u\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

int8_t verify_results_u32(uint32_t * ref, uint32_t * opt, int length)
{

    int8_t flag = 0;

    for (int i = 0; i < length; i++)
    {
        if (abs(ref[i] - opt[i]) > (uint32_t)DELTAQ31)
        {
            printf("U32 Output mismatch at %d, expected %u, actual %u\r\n", i, ref[i], opt[i]);

            flag = 1;
            break;
        }
#ifdef LOG_DEBUG
        printf("U32 Output mismatch at %d, expected %u, actual %u\r\n", i, ref[i], opt[i]);
#endif
    }

    return flag;
}

void do_srand(void)
{
    unsigned long randvar = __RV_CSR_READ(mcycle);
    srand(randvar);
    //printf("srandvar is %d\n", randvar);
}

void generate_rand_q7(q7_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (q7_t)(rand() % Q7_MAX - Q7_MAX / 2);
    }
}

void generate_rand_q15(q15_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (q15_t)(rand() % Q15_MAX - Q15_MAX / 2);
    }
}

void generate_rand_q31(q31_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (q31_t)(rand() % Q31_MAX - Q31_MAX / 2);
    }
}

void generate_rand_f32(float32_t *src, int length)
{
    do_srand();
    for (int i = 0; i < length; i++) {
        src[i] = (float32_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
}

#endif // DSP_COMMON_H
