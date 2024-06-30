#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_f64/test_data.h"

BENCH_DECLARE_VAR();

void fir_riscv_fir_f64(void)
{
    float64_t firStatef64[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    float64_t fir_f64_output[TEST_LENGTH_SAMPLES];

    generate_rand_f64(testInput_f64_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    generate_rand_f64(testInput_f64_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    /* clang-format off */
    riscv_fir_instance_f64 S;
    /* clang-format on */
    riscv_fir_init_f64(&S, NUM_TAPS, firCoeffs64LP, firStatef64, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_f64);
    riscv_fir_f64(&S, testInput_f64_50Hz_200Hz, fir_f64_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_f64);

    return;
}