#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/negate_q7/test_data.h"

BENCH_DECLARE_VAR();

void negate_riscv_negate_q7(void)
{
    q7_t negate_q7_output[ARRAY_SIZE_Q7];
    generate_rand_q7(negate_q7_input, ARRAY_SIZE_Q7);

    // q7_mult
    BENCH_START(riscv_negate_q7);
    riscv_negate_q7(negate_q7_input, negate_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_negate_q7);
}