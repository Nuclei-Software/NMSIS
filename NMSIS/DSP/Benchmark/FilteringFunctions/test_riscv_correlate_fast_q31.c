#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ConvolutionFunctions/correlate_fast_q31/test_data.h"

BENCH_DECLARE_VAR();

void correlate_riscv_correlate_fast_q31(void)
{
    q31_t correlate_q31_output[2 * max(ARRAYA_SIZE_Q31, ARRAYB_SIZE_Q31)];

    generate_rand_q31(correlate_input_q31_A, ARRAYA_SIZE_Q31);
    generate_rand_q31(correlate_input_q31_B, ARRAYB_SIZE_Q31);

    BENCH_START(riscv_correlate_fast_q31);
    riscv_correlate_fast_q31(correlate_input_q31_A, ARRAYA_SIZE_Q31, correlate_input_q31_B,
                           ARRAYB_SIZE_Q31, correlate_q31_output);
    BENCH_END(riscv_correlate_fast_q31);
    return;
}
