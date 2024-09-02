#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/biquad_cascade_df2T_f16/test_data.h"

BENCH_DECLARE_VAR();

void df2T_riscv_biquad_cascade_df2T_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t biquad_cascade_df2T_f16_output[TEST_LENGTH_SAMPLES_F16];
    float16_t IIRStateF16[2 * numStagesF16];

    generate_rand_f16(testInput_f16_50Hz_200Hz, TEST_LENGTH_SAMPLES_F16);
    riscv_float_to_f16(IIRCoeffs32LP, IIRCoeffsF16LP, 5 * numStagesF16);

    riscv_biquad_cascade_df2T_instance_f16 S;
    riscv_biquad_cascade_df2T_init_f16(&S, numStagesF16, IIRCoeffsF16LP, IIRStateF16);
    BENCH_START(riscv_biquad_cascade_df2T_f16);
    riscv_biquad_cascade_df2T_f16(&S, testInput_f16_50Hz_200Hz, biquad_cascade_df2T_f16_output, TEST_LENGTH_SAMPLES_F16);
    BENCH_END(riscv_biquad_cascade_df2T_f16);

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
}

