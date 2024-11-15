#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/max_q15/test_data.h"

BENCH_DECLARE_VAR();

void max_riscv_max_q15(void)
{
    q15_t q15_output;
    uint32_t index;

    generate_rand_q15(max_q15_input, ARRAY_SIZE);

    BENCH_START(riscv_max_q15);
    riscv_max_q15(max_q15_input, ARRAY_SIZE, &q15_output, &index);
    BENCH_END(riscv_max_q15);
}