#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_fast_q31/test_data.h"

BENCH_DECLARE_VAR();

void fir_riscv_fir_fast_q31(void)
{
    q31_t firStateq31[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    q31_t fir_q31_output[TEST_LENGTH_SAMPLES];

    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    float32_t firCoeff32LP[NUM_TAPS];
    for (int i = 0; i < NUM_TAPS; i++) {
        firCoeff32LP[i] = (float32_t)rand() / RAND_MAX * 2 - 1;
    }
    riscv_float_to_q31(firCoeff32LP, firCoeffLP_q31, NUM_TAPS);

    /* clang-format off */
    riscv_fir_instance_q31 S;
    /* clang-format on */
    riscv_fir_init_q31(&S, NUM_TAPS, firCoeffLP_q31, firStateq31, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_fast_q31);
    riscv_fir_fast_q31(&S, testInput_q31_50Hz_200Hz, fir_q31_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_fast_q31);

    return;
}
