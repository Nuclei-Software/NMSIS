#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/add_q31/test_data.h"

BENCH_DECLARE_VAR();

void add_riscv_add_q31(void)
{
    q31_t add_q31_output[ARRAY_SIZE_Q31];

    generate_rand_q31(add_q31_input1, ARRAY_SIZE_Q31);
    generate_rand_q31(add_q31_input2, ARRAY_SIZE_Q31);
    // riscv_add_q31.c
    BENCH_START(riscv_add_q31);
    riscv_add_q31(add_q31_input1, add_q31_input2, add_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_add_q31);
}
