#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/min_no_idx_f64/test_data.h"

BENCH_DECLARE_VAR();

void minNoIdx_riscv_min_no_idx_f64(void)
{
    float64_t f64_output;

    generate_rand_f64(min_no_idx_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_min_no_idx_f64);
    riscv_min_no_idx_f64(min_no_idx_f64_input, ARRAY_SIZE, &f64_output);
    BENCH_END(riscv_min_no_idx_f64);

    return;
}
