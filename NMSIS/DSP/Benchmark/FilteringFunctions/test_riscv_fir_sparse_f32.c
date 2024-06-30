#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_sparse_f32/test_data.h"

BENCH_DECLARE_VAR();

void firSparse_riscv_fir_sparse_f32(void)
{
    float32_t firStatef32[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    float32_t fir_sparse_f32_output[TEST_LENGTH_SAMPLES];

    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_sparse_instance_f32 S;
    /* clang-format on */
    for (int i = 0; i < NUM_TAPS; i++) {
        pTapDelay[i] = i + 1;
    }

    riscv_fir_sparse_init_f32(&S, NUM_TAPS, firCoeffs32LP, firStatef32, pTapDelay, MAXDelay, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_sparse_f32);
    riscv_fir_sparse_f32(&S, testInput_f32_50Hz_200Hz, fir_sparse_f32_output, pTapDelayScratch_f32, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_sparse_f32);

    return;
}
