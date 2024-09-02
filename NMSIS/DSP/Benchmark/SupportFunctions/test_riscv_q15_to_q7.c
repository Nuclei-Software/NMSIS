#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/q15_to_q7/test_data.h"

BENCH_DECLARE_VAR();

void q15ToQ7_riscv_q15_to_q7(void)
{
    q7_t q7_output[ARRAY_SIZE];

    generate_rand_q15(q15_input, ARRAY_SIZE);

    BENCH_START(riscv_q15_to_q7);
    riscv_q15_to_q7(q15_input, q7_output, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_q7);
}