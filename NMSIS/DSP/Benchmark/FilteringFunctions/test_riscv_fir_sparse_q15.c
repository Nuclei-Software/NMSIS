#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_sparse_q15/test_data.h"

BENCH_DECLARE_VAR();

void firSparse_riscv_fir_sparse_q15(void)
{
    q15_t firStateq15[TEST_LENGTH_SAMPLES_Q15 + NUM_TAPS_Q15 - 1];
    q15_t fir_sparse_q15_output[TEST_LENGTH_SAMPLES_Q15];
    q31_t pTapDelayScratchOUT[TEST_LENGTH_SAMPLES_Q15];

    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES_Q15);
    riscv_float_to_q15(firCoeffs32LP, firCoeffLP_q15, NUM_TAPS_Q15);
    /* clang-format off */
    riscv_fir_sparse_instance_q15 S;
    /* clang-format on */
    for (int i = 0; i < NUM_TAPS_Q15; i++) {
        pTapDelay[i] = i + 1;
    }

    riscv_fir_sparse_init_q15(&S, NUM_TAPS_Q15, firCoeffLP_q15, firStateq15, pTapDelay, MAXDelayQ15, TEST_LENGTH_SAMPLES_Q15);
    BENCH_START(riscv_fir_sparse_q15);
    riscv_fir_sparse_q15(&S, testInput_q15_50Hz_200Hz, fir_sparse_q15_output, pTapDelayScratch_q15, pTapDelayScratchOUT, TEST_LENGTH_SAMPLES_Q15);
    BENCH_END(riscv_fir_sparse_q15);
}
