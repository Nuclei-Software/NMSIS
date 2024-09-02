#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/min_no_idx_q7/test_data.h"

BENCH_DECLARE_VAR();

void minNoIdx_riscv_min_no_idx_q7(void)
{
    q7_t q7_output;

    generate_rand_q7(min_no_idx_q7_input, ARRAY_SIZE);

    BENCH_START(riscv_min_no_idx_q7);
    riscv_min_no_idx_q7(min_no_idx_q7_input, ARRAY_SIZE, &q7_output);
    BENCH_END(riscv_min_no_idx_q7);
}