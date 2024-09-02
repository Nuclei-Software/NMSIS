#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/negate_q15/test_data.h"

BENCH_DECLARE_VAR();

void negate_riscv_negate_q15(void)
{
    q15_t negate_q15_output[ARRAY_SIZE_Q15];

    generate_rand_q15(negate_q15_input, ARRAY_SIZE_Q15);

    // q15_mult
    BENCH_START(riscv_negate_q15);
    riscv_negate_q15(negate_q15_input, negate_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_negate_q15);
}