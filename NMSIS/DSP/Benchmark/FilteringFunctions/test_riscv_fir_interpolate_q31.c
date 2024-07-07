#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_interpolate_q31/test_data.h"

BENCH_DECLARE_VAR();

void firInterpolate_riscv_fir_interpolate_q31(void)
{
    q31_t firStateq31[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    q31_t interpolate_q31_output[TEST_LENGTH_SAMPLES];

    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(firCoeffs32LP, firCoeffs32LP_q31, NUM_TAPS);
    /* clang-format off */
    riscv_fir_interpolate_instance_q31 S;
    /* clang-format on */
    riscv_fir_interpolate_init_q31(&S, L, NUM_TAPS, firCoeffs32LP_q31, firStateq31,
                                 TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_interpolate_q31);
    riscv_fir_interpolate_q31(&S, testInput_q31_50Hz_200Hz,
                                interpolate_q31_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_interpolate_q31);

    return;
}
