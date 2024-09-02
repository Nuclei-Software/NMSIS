#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void yule_distance_riscv_yule_distance(void)
{
    float32_t yule_distance_f32_output;

    BENCH_START(riscv_yule_distance);
    yule_distance_f32_output = riscv_yule_distance(u32_yule_a_array, u32_yule_b_array, 36);
    BENCH_END(riscv_yule_distance);

    return;
}