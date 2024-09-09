#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/max_no_idx_f32/test_data.h"

BENCH_DECLARE_VAR();

void maxNoIdx_riscv_max_no_idx_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(max_no_idx_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_max_no_idx_f32);
    riscv_max_no_idx_f32(max_no_idx_f32_input, ARRAY_SIZE, &f32_output);
    BENCH_END(riscv_max_no_idx_f32);
}