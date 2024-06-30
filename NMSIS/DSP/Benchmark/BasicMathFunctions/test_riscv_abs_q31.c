#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/abs_q31/test_data.h"

BENCH_DECLARE_VAR();

void abs_riscv_abs_q31(void)
{
    
    q31_t abs_q31_output[ARRAY_SIZE_Q31];
    
    generate_rand_q31(abs_q31_input, ARRAY_SIZE_Q31);

    // riscv_abs_q31.c
    BENCH_START(riscv_abs_q31);
    riscv_abs_q31(abs_q31_input, abs_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_abs_q31);

    return;
}
