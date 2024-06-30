#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/biquad_cascade_df1_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void df1_riscv_biquad_cascade_df1_f16(void)
{
    float16_t biquad_cascade_df1_f16_output[TEST_LENGTH_SAMPLES];
    float16_t IIRStateF16[4 * numStages];

    riscv_biquad_casd_df1_inst_f16 S;
    generate_rand_f16(testInput_f16_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_f16(IIRCoeffs32LP, IIRCoeffsF16LP, 5 * numStages);
    riscv_biquad_cascade_df1_init_f16(&S, numStages, IIRCoeffsF16LP, IIRStateF16);
    BENCH_START(riscv_biquad_cascade_df1_f16);
    riscv_biquad_cascade_df1_f16(&S, testInput_f16_50Hz_200Hz, biquad_cascade_df1_f16_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df1_f16);

    return;
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */