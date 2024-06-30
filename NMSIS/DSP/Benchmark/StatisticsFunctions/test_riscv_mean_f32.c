#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/mean_f32/test_data.h"

BENCH_DECLARE_VAR();

void mean_riscv_mean_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(mean_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_mean_f32);
    riscv_mean_f32(mean_f32_input, ARRAY_SIZE, &f32_output);
    BENCH_END(riscv_mean_f32);

    return;
}