#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/max_no_idx_q7/test_data.h"

BENCH_DECLARE_VAR();

void maxNoIdx_riscv_max_no_idx_q7(void)
{
    q7_t q7_output;

    generate_rand_q7(max_no_idx_q7_input, ARRAY_SIZE);

    BENCH_START(riscv_max_no_idx_q7);
    riscv_max_no_idx_q7(max_no_idx_q7_input, ARRAY_SIZE, &q7_output);
    BENCH_END(riscv_max_no_idx_q7);
}