#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/q7_to_q15/test_data.h"

BENCH_DECLARE_VAR();

void q7ToQ15_riscv_q7_to_q15(void)
{
    q15_t q15_output[ARRAY_SIZE];

    generate_rand_q7(q7_input, ARRAY_SIZE);

    BENCH_START(riscv_q7_to_q15);
    riscv_q7_to_q15(q7_input, q15_output, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_q15);

}