#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/dot_prod_q7/test_data.h"

BENCH_DECLARE_VAR();

void dot_prod_riscv_dot_prod_q7(void)
{
    q31_t dot_prod_q31_output;

    generate_rand_q7(dot_prod_q7_input1, ARRAY_SIZE_Q7);
    generate_rand_q7(dot_prod_q7_input2, ARRAY_SIZE_Q7);

    // q7_dot
    BENCH_START(riscv_dot_prod_q7);
    riscv_dot_prod_q7(dot_prod_q7_input1, dot_prod_q7_input2, ARRAY_SIZE_Q7, &dot_prod_q31_output);
    BENCH_END(riscv_dot_prod_q7);
}
