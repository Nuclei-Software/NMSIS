#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/var_q31/test_data.h"

BENCH_DECLARE_VAR();

void var_riscv_var_q31(void)
{
    q31_t q31_output;

    generate_rand_q31(var_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_var_q31);
    riscv_var_q31(var_q31_input, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_var_q31);
}