#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/add_q7/test_data.h"

BENCH_DECLARE_VAR();

void add_riscv_add_q7(void)
{
    q7_t add_q7_output[ARRAY_SIZE_Q7];

    generate_rand_q7(add_q7_input1, ARRAY_SIZE_Q7);
    generate_rand_q7(add_q7_input2, ARRAY_SIZE_Q7);
    // riscv_add_q7.c
    BENCH_START(riscv_add_q7);
    riscv_add_q7(add_q7_input1, add_q7_input2, add_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_add_q7);
}
