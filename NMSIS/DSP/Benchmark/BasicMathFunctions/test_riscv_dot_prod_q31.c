#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/dot_prod_q31/test_data.h"

BENCH_DECLARE_VAR();

void dot_prod_riscv_dot_prod_q31(void)
{
    q63_t dot_prod_q63_output;

    generate_rand_q31(dot_prod_q31_input1, ARRAY_SIZE_Q31);
    generate_rand_q31(dot_prod_q31_input2, ARRAY_SIZE_Q31);

    // q31_dot
    BENCH_START(riscv_dot_prod_q31);
    riscv_dot_prod_q31(dot_prod_q31_input1, dot_prod_q31_input2, ARRAY_SIZE_Q31, &dot_prod_q63_output);
    BENCH_END(riscv_dot_prod_q31);
}
