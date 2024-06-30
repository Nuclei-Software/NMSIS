
#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ConvolutionFunctions/conv_partial_f32/test_data.h"

BENCH_DECLARE_VAR();

void convPartial_riscv_conv_partial_f32(void)
{
    float32_t conv_partial_f32_output[2 * max(ARRAYA_SIZE_F32, ARRAYB_SIZE_F32)];

    generate_rand_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32);
    generate_rand_f32(test_conv_input_f32_B, ARRAYB_SIZE_F32);

    BENCH_START(riscv_conv_partial_f32);
    riscv_status result = riscv_conv_partial_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                         conv_partial_f32_output, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
