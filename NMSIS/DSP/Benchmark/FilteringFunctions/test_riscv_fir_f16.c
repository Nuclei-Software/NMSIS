#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FilteringFunctions/fir_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void fir_riscv_fir_f16(void)
{
    float16_t firStatef16[TEST_LENGTH_SAMPLES + NUM_TAPS - 1];
    float16_t fir_f16_output[TEST_LENGTH_SAMPLES];

    generate_rand_f16(testInput_f16_50Hz_200Hz, TEST_LENGTH_SAMPLES);
    riscv_float_to_f16(firCoeffs32LP, firCoeffs16LP, NUM_TAPS);

    /* clang-format off */
    riscv_fir_instance_f16 S;
    /* clang-format on */
    riscv_fir_init_f16(&S, NUM_TAPS, firCoeffs16LP, firStatef16, TEST_LENGTH_SAMPLES);
    BENCH_START(riscv_fir_f16);
    riscv_fir_f16(&S, testInput_f16_50Hz_200Hz, fir_f16_output, TEST_LENGTH_SAMPLES);
    BENCH_END(riscv_fir_f16);

    return;
}
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
