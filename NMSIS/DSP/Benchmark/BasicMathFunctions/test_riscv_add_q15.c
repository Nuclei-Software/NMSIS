#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/add_q15/test_data.h"

BENCH_DECLARE_VAR();

void add_riscv_add_q15(void)
{
    q15_t add_q15_output[ARRAY_SIZE_Q15];

    generate_rand_q15(add_q15_input1, ARRAY_SIZE_Q15);
    generate_rand_q15(add_q15_input2, ARRAY_SIZE_Q15);
    // riscv_add_q15.c
    BENCH_START(riscv_add_q15);
    riscv_add_q15(add_q15_input1, add_q15_input2, add_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_add_q15);
}
