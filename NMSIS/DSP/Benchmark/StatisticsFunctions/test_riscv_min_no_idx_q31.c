#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/min_no_idx_q31/test_data.h"

BENCH_DECLARE_VAR();

void minNoIdx_riscv_min_no_idx_q31(void)
{
    q31_t q31_output;

    generate_rand_q31(min_no_idx_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_min_no_idx_q31);
    riscv_min_no_idx_q31(min_no_idx_q31_input, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_min_no_idx_q31);

    return;
}