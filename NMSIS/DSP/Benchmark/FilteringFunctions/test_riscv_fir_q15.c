#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_q15/test_data.h"

BENCH_DECLARE_VAR();

void fir_riscv_fir_q15(void)
{
    q15_t firStateq15[TEST_LENGTH_SAMPLES_Q15 + NUM_TAPS_Q15 - 1];
    q15_t fir_q15_output[TEST_LENGTH_SAMPLES_Q15];

    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES_Q15);
    riscv_float_to_q15(firCoeffs32LP, firCoeffLP_q15, NUM_TAPS_Q15);

    /* clang-format off */
    riscv_fir_instance_q15 S;
    /* clang-format on */
    riscv_fir_init_q15(&S, NUM_TAPS_Q15, firCoeffLP_q15, firStateq15, TEST_LENGTH_SAMPLES_Q15);
    BENCH_START(riscv_fir_q15);
    riscv_fir_q15(&S, testInput_q15_50Hz_200Hz, fir_q15_output, TEST_LENGTH_SAMPLES_Q15);
    BENCH_END(riscv_fir_q15);

    return;
}
