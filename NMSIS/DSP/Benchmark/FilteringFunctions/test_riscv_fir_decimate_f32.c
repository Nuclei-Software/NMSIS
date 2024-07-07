#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_decimate_f32/test_data.h"

BENCH_DECLARE_VAR();

void firDecimate_riscv_fir_decimate_f32(void)
{
    float32_t firStatef32[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    float32_t decimate_f32_output[TEST_LENGTH_SAMPLES];

    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    for (int i = 0; i < NUM_TAPS; i++) {
        firCoeffs32LP[i] = (float32_t)rand() / RAND_MAX * 2 - 1;
    }

    /* clang-format off */
    riscv_fir_decimate_instance_f32 S;
    /* clang-format on */
    riscv_status result = riscv_fir_decimate_init_f32(&S, NUM_TAPS, M, firCoeffs32LP, firStatef32,
                              TEST_LENGTH_SAMPLES);

    BENCH_START(riscv_fir_decimate_f32);
    riscv_fir_decimate_f32(&S, testInput_f32_50Hz_200Hz, decimate_f32_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_decimate_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
