
#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ConvolutionFunctions/conv_partial_q31/test_data.h"

BENCH_DECLARE_VAR();

void convPartial_riscv_conv_partial_q31(void)
{
    q31_t conv_partial_q31_output[2 * max(ARRAYA_SIZE_Q31, ARRAYB_SIZE_Q31)];

    generate_rand_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31);
    generate_rand_q31(test_conv_input_q31_B, ARRAYB_SIZE_Q31);

    BENCH_START(riscv_conv_partial_q31);
    riscv_status result = riscv_conv_partial_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                         conv_partial_q31_output, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}