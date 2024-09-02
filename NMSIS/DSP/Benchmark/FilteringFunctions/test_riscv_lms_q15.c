#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/lms_q15/test_data.h"

BENCH_DECLARE_VAR();

void lms_riscv_lms_q15(void)
{
    q15_t error_ones_q15[TEST_LENGTH_SAMPLES];
    q15_t lms_q15_output[TEST_LENGTH_SAMPLES];
    q15_t firStateQ15_LMS[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];

    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q15(expectoutput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    float32_t firCoeffs32LP[NUM_TAPS];
    for (int i = 0; i < NUM_TAPS; i++) {
        firCoeffs32LP[i] = (float32_t)rand() / RAND_MAX * 2 - 1;
    }
    riscv_float_to_q15(firCoeffs32LP, firCoeffs32LP_q15, NUM_TAPS);

    /* clang-format off */
    riscv_lms_instance_q15 S;
    riscv_lms_init_q15(&S, NUM_TAPS, firCoeffs32LP_q15, firStateQ15_LMS, MU_SIZE_Q15, TEST_LENGTH_SAMPLES, 1);
    BENCH_START(riscv_lms_q15);
    riscv_lms_q15(&S, testInput_q15_50Hz_200Hz, expectoutput_q15_50Hz_200Hz, lms_q15_output, error_ones_q15, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_lms_q15);
}
