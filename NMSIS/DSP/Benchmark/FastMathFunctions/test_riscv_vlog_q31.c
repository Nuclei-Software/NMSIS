#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/vlog_q31/test_data.h"

BENCH_DECLARE_VAR();

void vlog_riscv_vlog_q31(void)
{
    
    q31_t vlog_q31_output[ARRAY_SIZE_Q31];
    
    generate_rand_q31(vlog_q31_input, ARRAY_SIZE_Q31);

    BENCH_START(riscv_vlog_q31);
    riscv_vlog_q31(vlog_q31_input, vlog_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_vlog_q31);

    return;
}