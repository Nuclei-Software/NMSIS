#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/biquad_cascade_df1_q15/test_data.h"

BENCH_DECLARE_VAR();

void df1_riscv_biquad_cascade_df1_fast_q15(void)
{
    q15_t biquad_cascade_df1_q15_output[TEST_LENGTH_SAMPLES];
    q15_t IIRStateq15[4 * numStages];

    riscv_biquad_casd_df1_inst_q15 S;
    generate_rand_q15(testInput_q15_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    riscv_biquad_cascade_df1_init_q15(&S, numStages, IIRCoeffsQ15LP, IIRStateq15, 1);
    BENCH_START(riscv_biquad_cascade_df1_fast_q15);
    riscv_biquad_cascade_df1_fast_q15(&S, testInput_q15_50Hz_200Hz, biquad_cascade_df1_q15_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_fast_q15);

    return;
}
