#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/mult_q7/test_data.h"

BENCH_DECLARE_VAR();

void mul_riscv_mul_q7(void)
{
    q7_t mult_q7_output[ARRAY_SIZE_Q7];
    generate_rand_q7(mult_q7_input1, ARRAY_SIZE_Q7);
    generate_rand_q7(mult_q7_input2, ARRAY_SIZE_Q7);

    // q7_mult
    BENCH_START(riscv_mult_q7);
    riscv_mult_q7(mult_q7_input1, mult_q7_input2, mult_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_mult_q7);
}