#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/lms_norm_f32/test_data.h"

BENCH_DECLARE_VAR();

void lmsNorm_riscv_lms_norm_f32(void)
{
    float32_t error_ones[TEST_LENGTH_SAMPLES];
    float32_t lms_norm_f32_output[TEST_LENGTH_SAMPLES];
    float32_t firStateF32_LMS[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];

    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_f32(expectoutput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    /* clang-format off */
    riscv_lms_norm_instance_f32 S;

    riscv_lms_norm_init_f32(&S, NUM_TAPS, firCoeffs32LP, firStateF32_LMS, MU_SIZE, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_lms_norm_f32);
    riscv_lms_norm_f32(&S, testInput_f32_50Hz_200Hz, expectoutput_f32_50Hz_200Hz, lms_norm_f32_output, error_ones, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_lms_norm_f32);

    return;
}