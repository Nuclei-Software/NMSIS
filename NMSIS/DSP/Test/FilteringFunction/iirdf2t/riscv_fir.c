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

#define TEST_LENGTH_SAMPLES 320 /* 采样点数 */
#define numStages 2             /* 2阶IIR滤波的个数 */
// f32
float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float32_t testOutput_f32[TEST_LENGTH_SAMPLES * 2];
float32_t testOutput_f32_ref[TEST_LENGTH_SAMPLES * 2];
float32_t IIRCoeffs32LP[5 * numStages] = {1.0f, 2.0f, 1.0f,    1.11302985416334787593939381622476503253f,  - 0.574061915083954765748330828500911593437f,1.0f,  2.0f,  1.0f,   0.855397932775170177777113167394418269396f, - 0.209715357756554754420363906319835223258f};
float32_t IIRStateF32[2 * numStages];
float32_t IIRStateSteF32[4 * numStages];
#if defined (RISCV_FLOAT16_SUPPORTED)
// f16
float16_t testInput_f16_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float16_t testOutput_f16[TEST_LENGTH_SAMPLES * 2];
float16_t testOutput_f16_ref[TEST_LENGTH_SAMPLES * 2];
float16_t IIRCoeffs16LP[5 * numStages];
float16_t IIRStateF16[2 * numStages];
float16_t IIRStateSteF16[4 * numStages];
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

// f64
float64_t testInput_f64_50Hz_200Hz[TEST_LENGTH_SAMPLES];
float64_t testOutput_f64[TEST_LENGTH_SAMPLES];
float64_t testOutput_f64_ref[TEST_LENGTH_SAMPLES];
float64_t IIRStateF64[2 * numStages];
float64_t IIRStateSteF64[4 * numStages];
/* clang-format on */
//***************************************************************************************
//				iir		df2t
//***************************************************************************************
static void riscv_iir_df2t_f32_lp(void)
{
    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_biquad_cascade_df2T_instance_f32 S;
    /* clang-format on */
    float32_t ScaleValue;
    riscv_biquad_cascade_df2T_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateF32);
    BENCH_START(riscv_biquad_cascade_df2T_f32);
    riscv_biquad_cascade_df2T_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df2T_f32);
    riscv_biquad_cascade_df2T_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateF32);
    ref_biquad_cascade_df2T_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df2T_f32);
        printf("f32 biquad_cascade_df2T failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df2T_f32);
#else
    float snr =
        riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0], TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df2T_f32);
        printf("f32 biquad_cascade_df2T failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df2T_f32);
#endif
}

#if defined (RISCV_FLOAT16_SUPPORTED)
static void riscv_iir_df2t_f16_lp(void)
{
    generate_rand_f16(testInput_f16_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_f16(IIRCoeffs32LP, IIRCoeffs16LP, 5 * numStages);
    /* clang-format off */
    riscv_biquad_cascade_df2T_instance_f16 S;
    /* clang-format on */
    float16_t ScaleValue;
    riscv_biquad_cascade_df2T_init_f16(&S, numStages, IIRCoeffs16LP, IIRStateF16);
    BENCH_START(riscv_biquad_cascade_df2T_f16);
    riscv_biquad_cascade_df2T_f16(&S, testInput_f16_50Hz_200Hz, testOutput_f16, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df2T_f16);
    riscv_biquad_cascade_df2T_init_f16(&S, numStages, IIRCoeffs16LP, IIRStateF16);
    ref_biquad_cascade_df2T_f16(&S, testInput_f16_50Hz_200Hz, testOutput_f16_ref, TEST_LENGTH_SAMPLES);
    riscv_f16_to_float(testOutput_f16, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_f16_to_float(testOutput_f16_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df2T_f16);
        printf("q31 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df2T_f16);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_df2T_f16);
        printf("q31 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_df2T_f16);
#endif
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

static void riscv_iir_stereo_df2t_f32_lp(void)
{
    /* clang-format off */
    riscv_biquad_cascade_stereo_df2T_instance_f32 S;
    /* clang-format on */
    // float32_t ScaleValue;
    riscv_biquad_cascade_stereo_df2T_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateSteF32);
    BENCH_START(riscv_biquad_cascade_stereo_df2T_f32);
    riscv_biquad_cascade_stereo_df2T_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_stereo_df2T_f32);
    riscv_biquad_cascade_stereo_df2T_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateSteF32);
    ref_biquad_cascade_stereo_df2T_f32(&S, testInput_f32_50Hz_200Hz, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
    // ScaleValue = 0.052219514664161221f * 0.04279801741658381f;
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_stereo_df2T_f32);
        printf("f32 biquad_cascade_stereo_df2T failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_stereo_df2T_f32);
#else
    float snr =
        riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0], TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_stereo_df2T_f32);
        printf("f32 biquad_cascade_stereo_df2T failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_stereo_df2T_f32);
#endif
}

#if defined (RISCV_FLOAT16_SUPPORTED)
static void riscv_iir_stereo_df2t_f16_lp(void)
{
    /* clang-format off */
    riscv_biquad_cascade_stereo_df2T_instance_f16 S;
    /* clang-format on */
    riscv_biquad_cascade_stereo_df2T_init_f16(&S, numStages, IIRCoeffs16LP, IIRStateSteF16);
    BENCH_START(riscv_biquad_cascade_stereo_df2T_f16);
    riscv_biquad_cascade_stereo_df2T_f16(&S, testInput_f16_50Hz_200Hz, testOutput_f16, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_stereo_df2T_f16);
    riscv_biquad_cascade_stereo_df2T_init_f16(&S, numStages, IIRCoeffs16LP, IIRStateSteF16);
    ref_biquad_cascade_stereo_df2T_f16(&S, testInput_f16_50Hz_200Hz, testOutput_f16_ref, TEST_LENGTH_SAMPLES);
    riscv_f16_to_float(testOutput_f16, testOutput_f32, TEST_LENGTH_SAMPLES);
    riscv_f16_to_float(testOutput_f16_ref, testOutput_f32_ref, TEST_LENGTH_SAMPLES);
#ifndef WITH_FRONT
    float snr = riscv_snr_f32(&testOutput_f32_ref[50], &testOutput_f32[50], 269);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_stereo_df2T_f16);
        printf("q31 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_stereo_df2T_f16);
#else
    float snr = riscv_snr_f32(&testOutput_f32_ref[0], &testOutput_f32[0],
                            TEST_LENGTH_SAMPLES);

    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_biquad_cascade_stereo_df2T_f16);
        printf("q31 lms_norm failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_biquad_cascade_stereo_df2T_f16);
#endif
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

int main()
{
    BENCH_INIT();
    riscv_iir_df2t_f32_lp();
    riscv_iir_stereo_df2t_f32_lp();
#if defined (RISCV_FLOAT16_SUPPORTED)
    riscv_iir_df2t_f16_lp();
    riscv_iir_stereo_df2t_f16_lp();
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
