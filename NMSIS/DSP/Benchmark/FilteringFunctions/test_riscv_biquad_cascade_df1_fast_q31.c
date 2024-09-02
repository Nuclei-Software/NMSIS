#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/biquad_cascade_df1_q31/test_data.h"

BENCH_DECLARE_VAR();

void df1_riscv_biquad_cascade_df1_fast_q31(void)
{
    q31_t biquad_cascade_df1_q31_output[TEST_LENGTH_SAMPLES];
    q31_t IIRStateq31[4 * numStages];

    riscv_biquad_casd_df1_inst_q31 S;
    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    float32_t IIRCoeffs32LP[5 * numStages];

    for (int i = 0; i < 5 * numStages; i++) {
        IIRCoeffs32LP[i] = (float32_t)rand() / RAND_MAX * 2 - 1;
    }
    riscv_float_to_q31(IIRCoeffs32LP, IIRCoeffsQ31LP, 5 * numStages);

    riscv_biquad_cascade_df1_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRStateq31, 0);
    BENCH_START(riscv_biquad_cascade_df1_fast_q31);
    riscv_biquad_cascade_df1_fast_q31(&S, testInput_q31_50Hz_200Hz, biquad_cascade_df1_q31_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_fast_q31);

    return;
}
