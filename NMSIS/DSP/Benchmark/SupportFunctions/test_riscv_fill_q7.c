#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/fill_q7/test_data.h"

BENCH_DECLARE_VAR();

void fill_riscv_fill_q7(void)
{
    q7_t q7_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_q7);
    riscv_fill_q7(q7_fill_value, q7_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_q7);
}