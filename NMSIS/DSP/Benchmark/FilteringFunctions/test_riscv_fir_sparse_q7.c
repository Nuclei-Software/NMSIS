#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_sparse_q7/test_data.h"

BENCH_DECLARE_VAR();

void firSparse_riscv_fir_sparse_q7(void)
{
    q7_t firStateq7[TEST_LENGTH_SAMPLES_Q7 + NUM_TAPS_Q7 - 1];
    q7_t fir_sparse_q7_output[TEST_LENGTH_SAMPLES_Q7];
    q31_t pTapDelayScratchOUT[TEST_LENGTH_SAMPLES_Q7];

    generate_rand_q7(testInput_q7_50Hz_200Hz, TEST_LENGTH_SAMPLES_Q7);
    riscv_float_to_q7(firCoeffs32LP, firCoeffLP_q7, NUM_TAPS_Q7);
    /* clang-format off */
    riscv_fir_sparse_instance_q7 S;
    /* clang-format on */
    for (int i = 0; i < NUM_TAPS_Q7; i++) {
        pTapDelay[i] = i + 1;
    }

    riscv_fir_sparse_init_q7(&S, NUM_TAPS_Q7, firCoeffLP_q7, firStateq7, pTapDelay, MAXDelayQ7, TEST_LENGTH_SAMPLES_Q7);
    BENCH_START(riscv_fir_sparse_q7);
    riscv_fir_sparse_q7(&S, testInput_q7_50Hz_200Hz, fir_sparse_q7_output, pTapDelayScratch_q7, pTapDelayScratchOUT, TEST_LENGTH_SAMPLES_Q7);
    BENCH_END(riscv_fir_sparse_q7);

    return;
}
