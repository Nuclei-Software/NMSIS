#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/biquad_cascade_df2T_f64/test_data.h"

BENCH_DECLARE_VAR();

void df2T_riscv_biquad_cascade_df2T_f64(void)
{
    float64_t biquad_cascade_df2T_f64_output[TEST_LENGTH_SAMPLES];
    float64_t IIRStateF64[2 * numStages];

    generate_rand_f64(testInput_f64_50Hz_200Hz, TEST_LENGTH_SAMPLES);

    for (int i = 0; i < 5 * numStages; i++) {
        IIRCoeffs64LP[i] = (float64_t)rand() / RAND_MAX * 2 - 1;
    }

    riscv_biquad_cascade_df2T_instance_f64 S;
    riscv_biquad_cascade_df2T_init_f64(&S, numStages, IIRCoeffs64LP, IIRStateF64);
    BENCH_START(riscv_biquad_cascade_df2T_f64);
    riscv_biquad_cascade_df2T_f64(&S, testInput_f64_50Hz_200Hz, biquad_cascade_df2T_f64_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_biquad_cascade_df2T_f64);

    return;
}
