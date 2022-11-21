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

BENCH_DECLARE_VAR();
// #define WITH_FRONT

#define TEST_LENGTH_SAMPLES 320
#define NUM_TAPS 29

// float32_t
float32_t firCoeffs32LP[NUM_TAPS] = {
  -0.001822523074f, -0.001587929321f, 1.226008847e-18f, 0.003697750857f, 0.008075430058f,
  0.008530221879f, -4.273456581e-18f, -0.01739769801f, -0.03414586186f, -0.03335915506f,
  8.073562366e-18f, 0.06763084233f, 0.1522061825f, 0.2229246944f, 0.2504960895f,
  0.2229246944f, 0.1522061825f, 0.06763084233f, 8.073562366e-18f, -0.03335915506f,
  -0.03414586186f, -0.01739769801f, -4.273456581e-18f, 0.008530221879f, 0.008075430058f,
  0.003697750857f, 1.226008847e-18f, -0.001587929321f, -0.001822523074f};
float32_t firCoeffs32LP_tmp[NUM_TAPS];

float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float32_t expectoutput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float32_t error_ones[TEST_LENGTH_SAMPLES];
float32_t testOutput_f32[TEST_LENGTH_SAMPLES];
float32_t testOutput_f32_ref[TEST_LENGTH_SAMPLES];
float32_t firStateF32_LMS[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];

//q31_t
q31_t expectoutput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q31_t error_ones_q31[TEST_LENGTH_SAMPLES];
q31_t firStateq31_LMS[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
q31_t testInput_q31_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {0}, testOutput_q31[TEST_LENGTH_SAMPLES] = {0},testOutput_q31_ref[TEST_LENGTH_SAMPLES] = {0};
q31_t firCoeffs32LP_q31[NUM_TAPS] = {0};

//q15_t
q15_t expectoutput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES];
q15_t error_ones_q15[TEST_LENGTH_SAMPLES];
q15_t firStateq15_LMS[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
q15_t testInput_q15_50Hz_200Hz[TEST_LENGTH_SAMPLES] = {0}, testOutput_q15[TEST_LENGTH_SAMPLES] = {0},testOutput_q15_ref[TEST_LENGTH_SAMPLES] = {0};
q15_t firCoeffs32LP_q15[NUM_TAPS] = {0};

//***************************************************************************************
// Normalized LMS Filters
//***************************************************************************************
static void riscv_fir_f32_NLMS_lp(void)
{
    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_f32(expectoutput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    uint32_t i;
    /* clang-format off */
    riscv_lms_norm_instance_f32 S;

    memcpy(firCoeffs32LP_tmp, firCoeffs32LP, sizeof(firCoeffs32LP));
    riscv_lms_norm_init_f32(&S, NUM_TAPS, firCoeffs32LP, firStateF32_LMS, 0.2f, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_lms_norm_f32);
    riscv_lms_norm_f32(&S, testInput_f32_50Hz_200Hz, expectoutput_f32_50Hz_200Hz, testOutput_f32, &error_ones[0], TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_lms_norm_f32);
    riscv_lms_norm_init_f32(&S, NUM_TAPS, firCoeffs32LP_tmp, firStateF32_LMS, 0.2f, TEST_LENGTH_SAMPLES);
    ref_lms_norm_f32(&S, testInput_f32_50Hz_200Hz, expectoutput_f32_50Hz_200Hz, testOutput_f32_ref, &error_ones[0], TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_lms_norm_f32);
        printf("f32 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_lms_norm_f32);
#else
    float snr =
        riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0], TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_lms_norm_f32);
        printf("f32 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_lms_norm_f32);
#endif
}
static void riscv_fir_q31_NLMS_lp(void)
{
    /* clang-format off */
    riscv_lms_norm_instance_q31 S;
    riscv_float_to_q31(testInput_f32_50Hz_200Hz, testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(expectoutput_f32_50Hz_200Hz, expectoutput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    riscv_float_to_q31(firCoeffs32LP, firCoeffs32LP_q31, NUM_TAPS);
    riscv_lms_norm_init_q31(&S, NUM_TAPS, firCoeffs32LP_q31, firStateq31_LMS, 15, TEST_LENGTH_SAMPLES, 1);
    BENCH_START(riscv_lms_norm_q31);
    riscv_lms_norm_q31(&S, testInput_q31_50Hz_200Hz, expectoutput_q31_50Hz_200Hz, testOutput_q31, error_ones_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_lms_norm_q31);
    riscv_float_to_q31(firCoeffs32LP, firCoeffs32LP_q31, NUM_TAPS);
    riscv_lms_norm_init_q31(&S, NUM_TAPS, firCoeffs32LP_q31, firStateq31_LMS, 15, TEST_LENGTH_SAMPLES, 1);
    ref_lms_norm_q31(&S, testInput_q31_50Hz_200Hz, expectoutput_q31_50Hz_200Hz, testOutput_q31_ref, error_ones_q31, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q31_to_float(testOutput_q31_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_lms_norm_q31);
        printf("q31 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_lms_norm_q31);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32_L) {
        BENCH_ERROR(riscv_lms_norm_q31);
        printf("q31 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_lms_norm_q31);
#endif
}
static void riscv_fir_q15_NLMS_lp(void)
{
    /* clang-format off */
    riscv_lms_norm_instance_q15 S;
    riscv_float_to_q15(testInput_f32_50Hz_200Hz, testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q15(expectoutput_f32_50Hz_200Hz, expectoutput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q15(firCoeffs32LP, firCoeffs32LP_q15, NUM_TAPS);
    riscv_lms_norm_init_q15(&S, NUM_TAPS, firCoeffs32LP_q15, firStateq15_LMS, 3, TEST_LENGTH_SAMPLES, 1);
    BENCH_START(riscv_lms_norm_q15);
    riscv_lms_norm_q15(&S, testInput_q15_50Hz_200Hz, expectoutput_q15_50Hz_200Hz, testOutput_q15, error_ones_q15, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_lms_norm_q15);
    riscv_float_to_q15(firCoeffs32LP, firCoeffs32LP_q15, NUM_TAPS);
    riscv_lms_norm_init_q15(&S, NUM_TAPS, firCoeffs32LP_q15, firStateq15_LMS, 3, TEST_LENGTH_SAMPLES, 1);
    ref_lms_norm_q15(&S, testInput_q15_50Hz_200Hz, expectoutput_q15_50Hz_200Hz, testOutput_q15_ref, error_ones_q15, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_q15_to_float(testOutput_q15_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_lms_norm_q15);
        printf("q15 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_lms_norm_q15);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_lms_norm_q15);
        printf("q15 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_lms_norm_q15);
#endif
}

int main()
{
    BENCH_INIT();

    riscv_fir_f32_NLMS_lp();
    riscv_fir_q31_NLMS_lp();
    riscv_fir_q15_NLMS_lp();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
