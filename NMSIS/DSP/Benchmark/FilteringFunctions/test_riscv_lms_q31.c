#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/lms_q31/test_data.h"

BENCH_DECLARE_VAR();

void lms_riscv_lms_q31(void)
{
    q31_t error_ones_q31[TEST_LENGTH_SAMPLES];
    q31_t lms_q31_output[TEST_LENGTH_SAMPLES];
    q31_t firStateQ31_LMS[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];

    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_q31(expectoutput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    float32_t firCoeffs32LP[NUM_TAPS];
    for (int i = 0; i < NUM_TAPS; i++) {
        firCoeffs32LP[i] = (float32_t)rand() / RAND_MAX * 2 - 1;
    }
    riscv_float_to_q31(firCoeffs32LP, firCoeffs32LP_q31, NUM_TAPS);

    /* clang-format off */
    riscv_lms_instance_q31 S;
    riscv_lms_init_q31(&S, NUM_TAPS, firCoeffs32LP_q31, firStateQ31_LMS, MU_SIZE_Q31, TEST_LENGTH_SAMPLES, 1);
    BENCH_START(riscv_lms_q31);
    riscv_lms_q31(&S, testInput_q31_50Hz_200Hz, expectoutput_q31_50Hz_200Hz, lms_q31_output, error_ones_q31, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_lms_q31);

}
