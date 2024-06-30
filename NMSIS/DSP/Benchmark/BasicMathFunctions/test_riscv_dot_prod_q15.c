#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/dot_prod_q15/test_data.h"

BENCH_DECLARE_VAR();

void dot_prod_riscv_dot_prod_q15(void)
{
    q63_t dot_prod_q63_output[ARRAY_SIZE_Q15];
    generate_rand_q15(dot_prod_q15_input1, ARRAY_SIZE_Q15);
    generate_rand_q15(dot_prod_q15_input2, ARRAY_SIZE_Q15);

    // q15_dot
    BENCH_START(riscv_dot_prod_q15);
    riscv_dot_prod_q15(dot_prod_q15_input1, dot_prod_q15_input2, ARRAY_SIZE_Q15, dot_prod_q63_output);
    BENCH_END(riscv_dot_prod_q15);
}
