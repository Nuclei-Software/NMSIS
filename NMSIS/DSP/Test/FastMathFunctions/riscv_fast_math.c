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

#define BLOCK_TESTSZ        127

float32_t f32_pOUT[BLOCK_TESTSZ], f32_pOUT_ref[BLOCK_TESTSZ];
float32_t f32_pIN[BLOCK_TESTSZ];

q31_t q31_pOUT[BLOCK_TESTSZ], q31_pOUT_ref[BLOCK_TESTSZ];
q31_t q31_pIN[BLOCK_TESTSZ];

q15_t q15_pOUT[BLOCK_TESTSZ], q15_pOUT_ref[BLOCK_TESTSZ];
q15_t q15_pIN1[BLOCK_TESTSZ], q15_pIN2[BLOCK_TESTSZ];

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

static int DSP_SQRT(void)
{
    int i = 0;

    // f32_sqrt
    BENCH_START(riscv_sqrt_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_sqrt_f32(f32_pIN[i], &f32_pOUT[i]);
    }
    BENCH_END(riscv_sqrt_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pOUT_ref[i] = sqrtf(f32_pIN[i]);
    }
    s = verify_results_f32(f32_pOUT_ref, f32_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_sqrt_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sqrt_f32);

    // q31_sqrt
    BENCH_START(riscv_sqrt_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_sqrt_q31(q31_pIN[i], &q31_pOUT[i]);
    }
    BENCH_END(riscv_sqrt_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        ref_sqrt_q31(q31_pIN[i], &q31_pOUT_ref[i]);
    }
    s = verify_results_q31_low_precision(q31_pOUT_ref, q31_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_sqrt_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sqrt_q31);

    // q15_sqrt
    BENCH_START(riscv_sqrt_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_sqrt_q15(q15_pIN1[i], &q15_pOUT[i]);
    }
    BENCH_END(riscv_sqrt_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        ref_sqrt_q15(q15_pIN1[i], &q15_pOUT_ref[i]);
    }
    s = verify_results_q15_low_precision(q15_pOUT_ref, q15_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_sqrt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sqrt_q15);
}

static int DSP_DIVIDE(void)
{
    int i;
    int16_t shift[BLOCK_TESTSZ], shift_ref[BLOCK_TESTSZ];
    int8_t s1, s2;

    BENCH_START(riscv_divide_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_divide_q15(q15_pIN1[i], q15_pIN2[i], &q15_pOUT[i], &shift[i]);
    }
    BENCH_END(riscv_divide_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        ref_divide_q15(q15_pIN1[i], q15_pIN2[i], &q15_pOUT_ref[i], &shift_ref[i]);
    }
    s1 = verify_results_q15(q15_pOUT_ref, q15_pOUT, BLOCK_TESTSZ);
    s2  = verify_results_q15(shift_ref, shift, BLOCK_TESTSZ);
    if ((s1 != 0) || (s2 != 0)) {
        BENCH_ERROR(riscv_divide_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_divide_q15);
}

static int DSP_EXP(void)
{
    BENCH_START(riscv_vexp_f32);
    riscv_vexp_f32(f32_pIN, f32_pOUT, BLOCK_TESTSZ);
    BENCH_END(riscv_vexp_f32);
    ref_vexp_f32(f32_pIN, f32_pOUT_ref, BLOCK_TESTSZ);
    s = verify_results_f32(f32_pOUT_ref, f32_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_vexp_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_vexp_f32);
}

static int DSP_LOG(void)
{
    BENCH_START(riscv_vlog_f32);
    riscv_vlog_f32(f32_pIN, f32_pOUT, BLOCK_TESTSZ);
    BENCH_END(riscv_vlog_f32);
    ref_vlog_f32(f32_pIN, f32_pOUT_ref, BLOCK_TESTSZ);
    s = verify_results_f32(f32_pOUT_ref, f32_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_vlog_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_vlog_f32);
}

static int DSP_COS(void)
{
    // f32_cos
    int i;

    BENCH_START(riscv_cos_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pOUT[i] = riscv_cos_f32(f32_pIN[i]);
    }
    BENCH_END(riscv_cos_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pOUT_ref[i] = ref_cos_f32(f32_pIN[i]);
    }
    s = verify_results_f32(f32_pOUT_ref, f32_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_cos_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cos_f32);

    // q31_cos
    BENCH_START(riscv_cos_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pOUT[i] = riscv_cos_q31(q31_pIN[i]);
    }
    BENCH_END(riscv_cos_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pOUT_ref[i] = ref_cos_q31(q31_pIN[i]);
    }
    s = verify_results_q31(q31_pOUT_ref, q31_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_cos_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cos_q31);

    // q15_cos
    BENCH_START(riscv_cos_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_pOUT[i] = riscv_cos_q15(q15_pIN1[i]);
    }
    BENCH_END(riscv_cos_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_pOUT_ref[i] = ref_cos_q15(q15_pIN1[i]);
    }
    s = verify_results_q15(q15_pOUT_ref, q15_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_cos_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cos_q15);
}

static int DSP_SIN(void)
{
    // f32_sin
    int i;

    BENCH_START(riscv_sin_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pOUT[i] = riscv_sin_f32(f32_pIN[i]);
    }
    BENCH_END(riscv_sin_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pOUT_ref[i] = ref_sin_f32(f32_pIN[i]);
    }
    s = verify_results_f32(f32_pOUT_ref, f32_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_sin_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sin_f32);

    // q31_sin
    BENCH_START(riscv_sin_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pOUT[i] = riscv_sin_q31(q31_pIN[i]);
    }
    BENCH_END(riscv_sin_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pOUT_ref[i] = ref_sin_q31(q31_pIN[i]);
    }
    s = verify_results_q31(q31_pOUT_ref, q31_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_sin_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sin_q31);

    // q15_sin
    BENCH_START(riscv_sin_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_pOUT[i] = riscv_sin_q15(q15_pIN1[i]);
    }
    BENCH_END(riscv_sin_q15);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q15_pOUT_ref[i] = ref_sin_q15(q15_pIN1[i]);
    }
    s = verify_results_q15(q15_pOUT_ref, q15_pOUT, BLOCK_TESTSZ);
    if (s != 0) {
        BENCH_ERROR(riscv_sin_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sin_q15);
}

int main(void)
{
    BENCH_INIT();
    generate_rand_q15(q15_pIN1, BLOCK_TESTSZ);
    generate_rand_q15(q15_pIN2, BLOCK_TESTSZ);
    generate_rand_q31(q31_pIN, BLOCK_TESTSZ);
    generate_rand_f32(f32_pIN, BLOCK_TESTSZ);

    DSP_SQRT();
    DSP_DIVIDE();
    DSP_EXP();
    DSP_LOG();
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
