#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ConvolutionFunctions/conv_opt_q15/test_data.h"

BENCH_DECLARE_VAR();

void conv_riscv_conv_opt_q15(void)
{
    q15_t conv_opt_q15_output[2 * max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15)];

    generate_rand_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15);
    generate_rand_q15(test_conv_input_q15_B, ARRAYB_SIZE_Q15);

    BENCH_START(riscv_conv_opt_q15);
    riscv_conv_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                    conv_opt_q15_output, q15_pScratch1, q15_pScratch2);
    BENCH_END(riscv_conv_opt_q15);
}
