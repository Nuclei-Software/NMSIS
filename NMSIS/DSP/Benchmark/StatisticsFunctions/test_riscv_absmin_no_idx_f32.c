#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmin_no_idx_f32/test_data.h"

BENCH_DECLARE_VAR();

void absminNoIdx_riscv_absmin_no_idx_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(absmin_no_idx_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_absmin_no_idx_f32);
    riscv_absmin_no_idx_f32(absmin_no_idx_f32_input, ARRAY_SIZE, &f32_output);
    BENCH_END(riscv_absmin_no_idx_f32);
}