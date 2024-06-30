#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_sparse_q7/test_data.h"

BENCH_DECLARE_VAR();

void firSparse_riscv_fir_sparse_q7(void)
{
    q7_t firStateq7[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    q7_t fir_sparse_q7_output[TEST_LENGTH_SAMPLES];
    q31_t pTapDelayScratchOUT[TEST_LENGTH_SAMPLES];

    generate_rand_q7(testInput_q7_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    /* clang-format off */
    riscv_fir_sparse_instance_q7 S;
    /* clang-format on */
    for (int i = 0; i < NUM_TAPS; i++) {
        pTapDelay[i] = i + 1;
    }

    riscv_fir_sparse_init_q7(&S, NUM_TAPS, firCoeffLP_q7, firStateq7, pTapDelay, MAXDelay, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_sparse_q7);
    riscv_fir_sparse_q7(&S, testInput_q7_50Hz_200Hz, fir_sparse_q7_output, pTapDelayScratch_q7, pTapDelayScratchOUT, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_sparse_q7);

    return;
}
