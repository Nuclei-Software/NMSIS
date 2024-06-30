#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/biquad_cascade_df1_q31/test_data.h"

BENCH_DECLARE_VAR();

void df1_riscv_biquad_cas_df1_32x64_ins_q31(void)
{
    q31_t biquad_cas_df1_32x64_q31_output[TEST_LENGTH_SAMPLES];
    q63_t IIRState32x64[4 * numStages];

    riscv_biquad_cas_df1_32x64_ins_q31 S;
    generate_rand_q31(testInput_q31_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_q31(IIRCoeffs32LP, IIRCoeffsQ31LP, 5 * numStages);
    
    riscv_biquad_cas_df1_32x64_init_q31(&S, numStages, IIRCoeffsQ31LP, IIRState32x64, 0);
    BENCH_START(riscv_biquad_cas_df1_32x64_q31);
    riscv_biquad_cas_df1_32x64_q31(&S, testInput_q31_50Hz_200Hz, biquad_cas_df1_32x64_q31_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cas_df1_32x64_q31);

    return;
}
