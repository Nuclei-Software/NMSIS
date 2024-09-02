#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/std_q31/test_data.h"

BENCH_DECLARE_VAR();

void std_riscv_std_q31(void)
{
    q31_t q31_output;

    generate_rand_q31(std_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_std_q31);
    riscv_std_q31(std_q31_input, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_std_q31);
}