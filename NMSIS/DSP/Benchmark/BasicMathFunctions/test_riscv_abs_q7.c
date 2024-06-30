#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/abs_q7/test_data.h"

BENCH_DECLARE_VAR();
void abs_riscv_abs_q7(void)
{
    
    q7_t abs_q7_output[ARRAY_SIZE_Q7];
    
    generate_rand_q7(abs_q7_input, ARRAY_SIZE_Q7);

    // riscv_abs_q7.c
    BENCH_START(riscv_abs_q7);
    riscv_abs_q7(abs_q7_input, abs_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_abs_q7);

    return;
}
