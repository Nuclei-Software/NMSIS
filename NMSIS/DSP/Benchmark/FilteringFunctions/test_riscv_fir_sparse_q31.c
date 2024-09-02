#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/fir_sparse_q31/test_data.h"

BENCH_DECLARE_VAR();

void firSparse_riscv_fir_sparse_q31(void)
{
    q31_t firStateq31[TEST_LENGTH_SAMPLES_Q31 + NUM_TAPS_Q31 - 1];
    q31_t fir_sparse_q31_output[TEST_LENGTH_SAMPLES_Q31];

    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES_Q31);
    riscv_float_to_q31(firCoeffs32LP, firCoeffLP_q31, NUM_TAPS_Q31);
    /* clang-format off */
    riscv_fir_sparse_instance_q31 S;
    /* clang-format on */
    for (int i = 0; i < NUM_TAPS_Q31; i++) {
        pTapDelay[i] = i + 1;
    }

    riscv_fir_sparse_init_q31(&S, NUM_TAPS_Q31, firCoeffLP_q31, firStateq31, pTapDelay, MAXDelayQ31, TEST_LENGTH_SAMPLES_Q31);
    BENCH_START(riscv_fir_sparse_q31);
    riscv_fir_sparse_q31(&S, testInput_q31_50Hz_200Hz, fir_sparse_q31_output, pTapDelayScratch_q31, TEST_LENGTH_SAMPLES_Q31);
    BENCH_END(riscv_fir_sparse_q31);
}
