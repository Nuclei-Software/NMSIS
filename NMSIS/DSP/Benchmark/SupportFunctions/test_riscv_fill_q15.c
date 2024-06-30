#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/fill_q15/test_data.h"

BENCH_DECLARE_VAR();

void fill_riscv_fill_q15(void)
{
    q15_t q15_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_q15);
    riscv_fill_q15(q15_fill_value, q15_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_q15);

    return;
}