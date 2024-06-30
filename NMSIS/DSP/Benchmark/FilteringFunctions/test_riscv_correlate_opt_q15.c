#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ConvolutionFunctions/correlate_opt_q15/test_data.h"

BENCH_DECLARE_VAR();

void correlate_riscv_correlate_opt_q15(void)
{
    q15_t correlate_q15_output[2 * max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15)];

    generate_rand_q15(correlate_input_q15_A, ARRAYA_SIZE_Q15);
    generate_rand_q15(correlate_input_q15_B, ARRAYB_SIZE_Q15);

    BENCH_START(riscv_correlate_opt_q15);
    riscv_correlate_opt_q15(correlate_input_q15_A, ARRAYA_SIZE_Q15, correlate_input_q15_B, ARRAYB_SIZE_Q15,
                          correlate_q15_output, q15_pScratch1);
    BENCH_END(riscv_correlate_opt_q15);
    return;
}
