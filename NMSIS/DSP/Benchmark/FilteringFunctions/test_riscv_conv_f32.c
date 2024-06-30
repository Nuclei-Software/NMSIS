#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ConvolutionFunctions/conv_f32/test_data.h"

BENCH_DECLARE_VAR();

void conv_riscv_conv_f32(void)
{
    float32_t conv_f32_output[2 * max(ARRAYA_SIZE_F32, ARRAYB_SIZE_F32)];

    generate_rand_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32);
    generate_rand_f32(test_conv_input_f32_B, ARRAYB_SIZE_F32);

    BENCH_START(riscv_conv_f32);
    riscv_conv_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                 conv_f32_output);
    BENCH_END(riscv_conv_f32);
    return;
}
