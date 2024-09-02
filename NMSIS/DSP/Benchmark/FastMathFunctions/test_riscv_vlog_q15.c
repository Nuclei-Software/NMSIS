#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/vlog_q15/test_data.h"

BENCH_DECLARE_VAR();

void vlog_riscv_vlog_q15(void)
{
    q15_t vlog_q15_output[ARRAY_SIZE_Q15];

    generate_rand_q15(vlog_q15_input, ARRAY_SIZE_Q15);

    BENCH_START(riscv_vlog_q15);
    riscv_vlog_q15(vlog_q15_input, vlog_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_vlog_q15);

    return;
}