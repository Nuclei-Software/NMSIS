#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/std_q15/test_data.h"

BENCH_DECLARE_VAR();

void std_riscv_std_q15(void)
{
    q15_t q15_output;

    generate_rand_q15(std_q15_input, ARRAY_SIZE);

    BENCH_START(riscv_std_q15);
    riscv_std_q15(std_q15_input, ARRAY_SIZE, &q15_output);
    BENCH_END(riscv_std_q15);

    return;
}