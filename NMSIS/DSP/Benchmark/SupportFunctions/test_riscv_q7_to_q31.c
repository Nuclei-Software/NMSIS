#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/q7_to_q31/test_data.h"

BENCH_DECLARE_VAR();

void q7ToQ31_riscv_q7_to_q31(void)
{
    q31_t q31_output[ARRAY_SIZE];

    generate_rand_q7(q7_input, ARRAY_SIZE);

    BENCH_START(riscv_q7_to_q31);
    riscv_q7_to_q31(q7_input, q31_output, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_q31);

    return;
}