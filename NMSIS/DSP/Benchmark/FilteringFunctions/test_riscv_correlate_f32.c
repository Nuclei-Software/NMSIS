#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ConvolutionFunctions/correlate_f32/test_data.h"

BENCH_DECLARE_VAR();

void correlate_riscv_correlate_f32(void)
{
    float32_t correlate_f32_output[2 * max(ARRAYA_SIZE_F32, ARRAYB_SIZE_F32)];

    generate_rand_f32(correlate_input_f32_A, ARRAYA_SIZE_F32);
    generate_rand_f32(correlate_input_f32_B, ARRAYB_SIZE_F32);

    BENCH_START(riscv_correlate_f32);
    riscv_correlate_f32(correlate_input_f32_A, ARRAYA_SIZE_F32, correlate_input_f32_B, ARRAYB_SIZE_F32,
                      correlate_f32_output);
    BENCH_END(riscv_correlate_f32);
    return;
}