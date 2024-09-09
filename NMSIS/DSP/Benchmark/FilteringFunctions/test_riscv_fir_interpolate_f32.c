#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_interpolate_f32/test_data.h"

BENCH_DECLARE_VAR();

void firInterpolate_riscv_fir_interpolate_f32(void)
{
    float32_t firStatef32[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    float32_t interpolate_f32_output[TEST_LENGTH_SAMPLES];

    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_interpolate_instance_f32 S;
    /* clang-format on */
    riscv_fir_interpolate_init_f32(&S, L, NUM_TAPS, firCoeffs32LP, firStatef32,
                                 TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_interpolate_f32);
    riscv_fir_interpolate_f32(&S, testInput_f32_50Hz_200Hz,
                                interpolate_f32_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_interpolate_f32);

    return;
}