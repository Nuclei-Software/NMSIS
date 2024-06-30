#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ConvolutionFunctions/conv_opt_q7/test_data.h"

BENCH_DECLARE_VAR();

void conv_riscv_conv_opt_q7(void)
{
    q7_t conv_opt_q7_output[2 * max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];

    generate_rand_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7);
    generate_rand_q7(test_conv_input_q7_B, ARRAYB_SIZE_Q7);

    BENCH_START(riscv_conv_opt_q7);
    riscv_conv_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                    conv_opt_q7_output, pScratch1, pScratch2);
    BENCH_END(riscv_conv_opt_q7);
}