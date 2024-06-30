#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/abs_q15/test_data.h"

BENCH_DECLARE_VAR();

void abs_riscv_abs_q15(void)
{
    
    q15_t abs_q15_output[ARRAY_SIZE_Q15];
    
    generate_rand_q15(abs_q15_input, ARRAY_SIZE_Q15);

    // riscv_abs_q15.c
    BENCH_START(riscv_abs_q15);
    riscv_abs_q15(abs_q15_input, abs_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_abs_q15);

    return;
}
