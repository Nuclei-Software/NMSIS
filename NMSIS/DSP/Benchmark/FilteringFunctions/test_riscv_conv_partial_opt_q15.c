
#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ConvolutionFunctions/conv_partial_opt_q15/test_data.h"

BENCH_DECLARE_VAR();

void convPartial_riscv_conv_partial_opt_q15(void)
{
    q15_t conv_q15_output[2 * max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15)];

    generate_rand_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15);
    generate_rand_q15(test_conv_input_q15_B, ARRAYB_SIZE_Q15);

    BENCH_START(riscv_conv_partial_opt_q15);
    riscv_status result = riscv_conv_partial_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                            conv_q15_output, firstIndex, numPoints, pScratch1, pScratch2);
    BENCH_END(riscv_conv_partial_opt_q15);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
