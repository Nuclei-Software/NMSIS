#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FilteringFunctions/biquad_cascade_df2T_f32/test_data.h"

BENCH_DECLARE_VAR();

void df2T_riscv_biquad_cascade_df2T_f32(void)
{
    float32_t biquad_cascade_df2T_f32_output[TEST_LENGTH_SAMPLES];
    float32_t IIRStateF32[2 * numStages];

    generate_rand_f32(testInput_f32_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    riscv_biquad_cascade_df2T_instance_f32 S;
    riscv_biquad_cascade_df2T_init_f32(&S, numStages, IIRCoeffs32LP, IIRStateF32);
    BENCH_START(riscv_biquad_cascade_df2T_f32);
    riscv_biquad_cascade_df2T_f32(&S, testInput_f32_50Hz_200Hz, biquad_cascade_df2T_f32_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df2T_f32);

    return;
}
