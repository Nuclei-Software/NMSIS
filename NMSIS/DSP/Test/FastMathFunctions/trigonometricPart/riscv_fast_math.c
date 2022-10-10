//
// Created by lujun on 19-6-28.
//
// This contains sin, cos and sqrt calculates
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

int test_flag_error = 0;

#define BLOCK_TESTSZ        256

BENCH_DECLARE_VAR();
static int DSP_COS(void)
{
    // f32_cos
    volatile int i;
    float32_t f32_cosoutput[BLOCK_TESTSZ], f32_cosoutput_ref[BLOCK_TESTSZ];
    float32_t f32_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pIN[i] = (float32_t)rand();
    }

    BENCH_START(riscv_cos_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_cosoutput[i] = riscv_cos_f32(f32_pIN[i]);
    }
    BENCH_END(riscv_cos_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_cosoutput_ref[i] = ref_cos_f32(f32_pIN[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if (fabs(f32_cosoutput[i] - f32_cosoutput_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_cos_f32);
            printf("expect: %f, actual: %f\n", f32_cosoutput_ref[i], f32_cosoutput[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cos_f32);
    // q31_cos
    q31_t q31_cosoutput[BLOCK_TESTSZ], q31_cosoutput_ref[BLOCK_TESTSZ];
    q31_t q31_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pIN[i] = (q31_t)rand();
    }

    BENCH_START(riscv_cos_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_cosoutput[i] = riscv_cos_q31(q31_pIN[i]);
    }
    BENCH_END(riscv_cos_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_cosoutput_ref[i] = ref_cos_q31(q31_pIN[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if (labs(q31_cosoutput[i] - q31_cosoutput_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_cos_q31);
            printf("expect: %x, actual: %x\n", q31_cosoutput_ref[i], q31_cosoutput[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cos_q31);
    // q15_cos
    q15_t q15_cosoutput[BLOCK_TESTSZ], q15_cosoutput_ref[BLOCK_TESTSZ];
    q15_t q15_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_pIN[i] = (q15_t)(rand() % 0x7fff);
    }

    BENCH_START(riscv_cos_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_cosoutput[i] = riscv_cos_q15(q15_pIN[i]);
    }
    BENCH_END(riscv_cos_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_cosoutput_ref[i] = ref_cos_q15(q15_pIN[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if (abs(q15_cosoutput[i] - q15_cosoutput_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_cos_q15);
            printf("expect: %x, actual: %x\n", q15_cosoutput_ref[i], q15_cosoutput[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cos_q15);
    printf("all cos tests are passed,well done!\n");
}
/*
*********************************************************************************************************
* 函 数 名: DSP_Sine
* 功能说明: 求sine函数
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
static int DSP_SIN(void)
{
    // f32_sin
    volatile int i;
    float32_t f32_sinoutput[BLOCK_TESTSZ], f32_sinoutput_ref[BLOCK_TESTSZ];
    float32_t f32_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pIN[i] = (float32_t)(rand() / 0x7fff);
    }

    BENCH_START(riscv_sin_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_sinoutput[i] = riscv_sin_f32(f32_pIN[i]);
    }
    BENCH_END(riscv_sin_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_sinoutput_ref[i] = ref_sin_f32(f32_pIN[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if (fabs(f32_sinoutput[i] - f32_sinoutput_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_sin_f32);
            printf("expect: %f, actual: %f\n", f32_sinoutput_ref[i], f32_sinoutput[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_sin_f32);
    // q31_sin
    q31_t q31_sinoutput[BLOCK_TESTSZ], q31_sinoutput_ref[BLOCK_TESTSZ];
    q31_t q31_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pIN[i] = (q31_t)rand();
    }

    BENCH_START(riscv_sin_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_sinoutput[i] = riscv_sin_q31(q31_pIN[i]);
    }
    BENCH_END(riscv_sin_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_sinoutput_ref[i] = ref_sin_q31(q31_pIN[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if (labs(q31_sinoutput[i] - q31_sinoutput_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_sin_q31);
            printf("expect: %x, actual: %x\n", q31_sinoutput_ref[i], q31_sinoutput[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_sin_q31);
    // q15_sin
    q15_t q15_sinoutput[BLOCK_TESTSZ], q15_sinoutput_ref[BLOCK_TESTSZ];
    q15_t q15_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_pIN[i] = (q15_t)(rand() % 0x7fff);
    }

    BENCH_START(riscv_sin_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_sinoutput[i] = riscv_sin_q15(q15_pIN[i]);
    }
    BENCH_END(riscv_sin_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_sinoutput_ref[i] = ref_sin_q15(q15_pIN[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if (abs(q15_sinoutput[i] - q15_sinoutput_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_sin_q15);
            printf("expect: %x, actual: %x\n", q15_sinoutput_ref[i], q15_sinoutput[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_sin_q15);
    printf("all sin tests are passed,well done!\n");
}

int main()
{
    BENCH_INIT();

    srand(__RV_CSR_READ(mcycle));

    DSP_COS();
    DSP_SIN();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
