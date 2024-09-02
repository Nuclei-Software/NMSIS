#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/fill_q31/test_data.h"

BENCH_DECLARE_VAR();

void fill_riscv_fill_q31(void)
{
    q31_t q31_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_q31);
    riscv_fill_q31(q31_fill_value, q31_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_q31);
}