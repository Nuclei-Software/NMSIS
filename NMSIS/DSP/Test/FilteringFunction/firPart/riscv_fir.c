//
// Created by lujun on 19-6-28.
//
// This contains fir, iir and lms filters
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
//
// This part only tests result after 50 dots.If you want to test results' all
// dots, please define  WITH_FRONT
//
// If there are too much numbers which makes it hard to read,in VScode you can
// press alt and z can shrimp them to one line
//
// You MUST be careful about overflow.
#include "riscv_math.h"
#include <stdint.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#define SNR_THRESHOLD_F32 (80.0f)

int test_flag_error = 0;

// #define WITH_FRONT

BENCH_DECLARE_VAR();

#define TEST_LENGTH_SAMPLES 320
#define NUM_TAPS 30             /* Must be even */
float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float32_t firStatef32[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
float32_t testOutput_f32[TEST_LENGTH_SAMPLES];
float32_t testOutput_f32_ref[TEST_LENGTH_SAMPLES];
const float32_t firCoeffs32LP[NUM_TAPS] = {
  -0.001822523074f, -0.001587929321f, 1.226008847e-18f, 0.003697750857f, 0.008075430058f,
  0.008530221879f, -4.273456581e-18f, -0.01739769801f, -0.03414586186f, -0.03335915506f,
  8.073562366e-18f, 0.06763084233f, 0.1522061825f, 0.2229246944f, 0.2504960895f,
  0.2229246944f, 0.1522061825f, 0.06763084233f, 8.073562366e-18f, -0.03335915506f,
  -0.03414586186f, -0.01739769801f, -4.273456581e-18f, 0.008530221879f, 0.008075430058f,
  0.003697750857f, 1.226008847e-18f, -0.001587929321f, -0.001822523074f, 0.0f};

// f16
#if defined (RISCV_FLOAT16_SUPPORTED)
float16_t testInput_f16_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float16_t firStatef16[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
float16_t testOutput_f16[TEST_LENGTH_SAMPLES];
float16_t testOutput_f16_ref[TEST_LENGTH_SAMPLES];
float16_t firCoeffs16LP[NUM_TAPS];
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

// q31
q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q31_t firStateq31[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
q31_t testOutput_q31[TEST_LENGTH_SAMPLES];
q31_t testOutput_q31_ref[TEST_LENGTH_SAMPLES];
q31_t firCoeffs32LP_q31[NUM_TAPS];

// q15
q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q15_t firStateq15[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
q15_t testOutput_q15[TEST_LENGTH_SAMPLES];
q15_t testOutput_q15_ref[TEST_LENGTH_SAMPLES];
q15_t firCoeffs32LP_q15[NUM_TAPS];

// q7
q7_t testInput_q7_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q7_t firStateq7[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
q7_t testOutput_q7[TEST_LENGTH_SAMPLES];
q7_t testOutput_q7_ref[TEST_LENGTH_SAMPLES];
q7_t firCoeffs32LP_q7[NUM_TAPS] = {0};

/* clang-format on */
//***************************************************************************************
//				fir
//***************************************************************************************
static int riscv_fir_f32_lp(void)
{
    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_instance_f32 S;
    /* clang-format on */
    riscv_fir_init_f32(&S, NUM_TAPS, firCoeffs32LP, firStatef32, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_f32);
    riscv_fir_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_f32);
    ref_fir_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_f32);
        printf("f32 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_f32);
#else
    float snr =
        riscv_snr_f32(&testOutput_ref[0], &testOutput[0], TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_f32);
        printf("f32 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_f32);
#endif
}

#if defined (RISCV_FLOAT16_SUPPORTED)
static int riscv_fir_f16_lp(void)
{
    riscv_float_to_f16(testInput_f32_50Hz_200Hz, testInput_f16_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_f16(firCoeffs32LP, firCoeffs16LP, NUM_TAPS);
    /* clang-format off */
    riscv_fir_instance_f16 S;
    /* clang-format on */
    riscv_fir_init_f16(&S, NUM_TAPS, firCoeffs16LP, firStatef16, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_f16);
    riscv_fir_f16(&S, testInput_f16_50Hz_200Hz, testOutput_f16, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_f16);
    ref_fir_f16(&S, testInput_f16_50Hz_200Hz, testOutput_f16_ref, TEST_LENGTH_SAMPLES);
    riscv_f16_to_float(testOutput_f16, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_f16_to_float(testOutput_f16_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_f16);
        printf("f16 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_f16);
#else
    float snr = riscv_snr_f32(&testOutput_ref[0], &testOutput[0], TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_f16);
        printf("f16 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_f16);
#endif
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

static int riscv_fir_q31_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_fir_instance_q31 S;
    /* clang-format on */
    riscv_float_to_q31(testInput_f32_50Hz_200Hz, testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(firCoeffs32LP, firCoeffs32LP_q31, NUM_TAPS);
    riscv_fir_init_q31(&S, NUM_TAPS, firCoeffs32LP_q31, firStateq31, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_q31);
    riscv_fir_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_q31);
    ref_fir_q31(&S, testInput_q31_50Hz_200Hz, testOutput_q31_ref, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_q31);
        printf("q31 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_q31);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_q31);
        printf("q31 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_q31);
#endif
}
static int riscv_fir_q15_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_fir_instance_q15 S;
    /* clang-format on */
    riscv_float_to_q15(testInput_f32_50Hz_200Hz, testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q15(firCoeffs32LP, firCoeffs32LP_q15, NUM_TAPS);
    riscv_fir_init_q15(&S, NUM_TAPS, firCoeffs32LP_q15, firStateq15, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_q15);
    riscv_fir_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_q15);
    ref_fir_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15_ref, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_q15);
        printf("q15 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_q15);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_q15);
        printf("q15 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_q15);
#endif
}
static int riscv_fir_q7_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_fir_instance_q7 S;
    /* clang-format on */
    riscv_float_to_q7(testInput_f32_50Hz_200Hz, testInput_q7_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q7(firCoeffs32LP, firCoeffs32LP_q7, NUM_TAPS);
    riscv_fir_init_q7(&S, NUM_TAPS, firCoeffs32LP_q7, firStateq7, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_q7);
    riscv_fir_q7(&S, testInput_q7_50Hz_200Hz, testOutput_q7, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_q7);
    ref_fir_q7(&S, testInput_q7_50Hz_200Hz, testOutput_q7_ref, TEST_LENGTH_SAMPLES);
    riscv_q7_to_float(testOutput_q7, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q7_to_float(testOutput_q7_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_q7);
        printf("q7 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_q7);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_q7);
        printf("q7 fir failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_q7);
#endif
}
static int riscv_fir_fast_q31_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_fir_instance_q31 S;
    /* clang-format on */
    riscv_float_to_q31(testInput_f32_50Hz_200Hz, testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(firCoeffs32LP, firCoeffs32LP_q31, NUM_TAPS);
    riscv_fir_init_q31(&S, NUM_TAPS, firCoeffs32LP_q31, firStateq31, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_fast_q31);
    riscv_fir_fast_q31(&S, testInput_q31_50Hz_200Hz,
                       testOutput_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_fast_q31);
    ref_fir_fast_q31(&S, testInput_q31_50Hz_200Hz,
                     testOutput_q31_ref, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_fast_q31);
        printf("q31 fir_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_fast_q31);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_fast_q31);
        printf("q31 fir_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_fast_q31);
#endif
}
static int riscv_fir_fast_q15_lp(void)
{
    uint32_t i;
    /* clang-format off */
    riscv_fir_instance_q15 S;
    /* clang-format on */
    riscv_float_to_q15(testInput_f32_50Hz_200Hz, testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q15(firCoeffs32LP, firCoeffs32LP_q15, NUM_TAPS);
    riscv_fir_init_q15(&S, NUM_TAPS, firCoeffs32LP_q15, firStateq15, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_fast_q15);
    riscv_fir_fast_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_fast_q15);
    ref_fir_fast_q15(&S, testInput_q15_50Hz_200Hz, testOutput_q15_ref, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_fast_q15);
        printf("q15 fir_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_fast_q15);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_fir_fast_q15);
        printf("q15 fir_fast failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_fir_fast_q15);
#endif
}

int main()
{
    BENCH_INIT();

    riscv_fir_f32_lp();
    riscv_fir_q31_lp();
    riscv_fir_q15_lp();
    riscv_fir_q7_lp();
    riscv_fir_fast_q31_lp();
    riscv_fir_fast_q15_lp();
#if defined (RISCV_FLOAT16_SUPPORTED)
    riscv_fir_f16_lp();
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
