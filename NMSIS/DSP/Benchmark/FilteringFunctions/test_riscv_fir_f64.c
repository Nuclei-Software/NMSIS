#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_f64/test_data.h"

BENCH_DECLARE_VAR();

void fir_riscv_fir_f64(void)
{
#ifdef F64
    float64_t firStatef64[TEST_LENGTH_SAMPLES_F64 + NUM_TAPS_F64 - 1];
    float64_t fir_f64_output[TEST_LENGTH_SAMPLES_F64];

    generate_rand_f64(testInput_f64_50Hz_200Hz, TEST_LENGTH_SAMPLES_F64);
    generate_rand_f64(testInput_f64_50Hz_200Hz, TEST_LENGTH_SAMPLES_F64);

    /* clang-format off */
    riscv_fir_instance_f64 S;
    /* clang-format on */
    riscv_fir_init_f64(&S, NUM_TAPS_F64, firCoeffs64LP, firStatef64, TEST_LENGTH_SAMPLES_F64);
    BENCH_START(riscv_fir_f64);
    riscv_fir_f64(&S, testInput_f64_50Hz_200Hz, fir_f64_output, TEST_LENGTH_SAMPLES_F64);
    BENCH_END(riscv_fir_f64);
#endif
}
