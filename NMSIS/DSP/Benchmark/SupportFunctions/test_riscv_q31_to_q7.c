#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/q31_to_q7/test_data.h"

BENCH_DECLARE_VAR();

void q31ToQ7_riscv_q31_to_q7(void)
{
    q7_t q7_output[ARRAY_SIZE];

    generate_rand_q31(q31_input, ARRAY_SIZE);

    BENCH_START(riscv_q31_to_q7);
    riscv_q31_to_q7(q31_input, q7_output, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_q7);
}