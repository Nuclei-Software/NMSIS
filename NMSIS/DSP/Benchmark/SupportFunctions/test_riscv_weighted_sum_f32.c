#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/weighted_sum_f32/test_data.h"

BENCH_DECLARE_VAR();

void weightedSum_riscv_weighted_sum_f32(void)
{
    float32_t f32_weighted_output;

    generate_rand_f32(f32_weighted_sum_array, WEIGHT_NUM);
    for (int i = 0; i < WEIGHT_NUM; i++) {
        f32_weighted_array[i] = (float32_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_weighted_sum_f32);
    f32_weighted_output = riscv_weighted_sum_f32(f32_weighted_sum_array, f32_weighted_array, WEIGHT_NUM);
    BENCH_END(riscv_weighted_sum_f32);
}
