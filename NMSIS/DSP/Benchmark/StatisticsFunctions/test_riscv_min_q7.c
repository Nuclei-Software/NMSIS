#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/min_q7/test_data.h"

BENCH_DECLARE_VAR();

void min_riscv_min_q7(void)
{
    q7_t q7_output;
    uint32_t index;

    generate_rand_q7(min_q7_input, ARRAY_SIZE);

    BENCH_START(riscv_min_q7);
    riscv_min_q7(min_q7_input, ARRAY_SIZE, &q7_output, &index);
    BENCH_END(riscv_min_q7);

    return;
}