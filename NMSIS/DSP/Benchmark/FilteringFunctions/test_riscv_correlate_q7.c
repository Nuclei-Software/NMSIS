#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ConvolutionFunctions/correlate_q7/test_data.h"

BENCH_DECLARE_VAR();

void correlate_riscv_correlate_q7(void)
{
    q7_t correlate_q7_output[2 * max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];

    generate_rand_q7(correlate_input_q7_A, ARRAYA_SIZE_Q7);
    generate_rand_q7(correlate_input_q7_B, ARRAYB_SIZE_Q7);

    BENCH_START(riscv_correlate_q7);
    riscv_correlate_q7(correlate_input_q7_A, ARRAYA_SIZE_Q7, correlate_input_q7_B, ARRAYB_SIZE_Q7,
                      correlate_q7_output);
    BENCH_END(riscv_correlate_q7);
    return;
}
