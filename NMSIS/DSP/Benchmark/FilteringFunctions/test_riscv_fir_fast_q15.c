#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_fast_q15/test_data.h"

BENCH_DECLARE_VAR();

void fir_riscv_fir_fast_q15(void)
{
    q15_t firStateq15[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    q15_t fir_q15_output[TEST_LENGTH_SAMPLES];

    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    /* clang-format off */
    riscv_fir_instance_q15 S;
    /* clang-format on */
    riscv_fir_init_q15(&S, NUM_TAPS, firCoeffLP_q15, firStateq15, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_fast_q15);
    riscv_fir_fast_q15(&S, testInput_q15_50Hz_200Hz, fir_q15_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_fast_q15);

    return;
}