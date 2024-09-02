#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void rogerstanimoto_distance_riscv_rogerstanimoto_distance(void)
{
    float32_t rogerstanimoto_distance_f32_output;

    BENCH_START(riscv_rogerstanimoto_distance);
    rogerstanimoto_distance_f32_output = riscv_rogerstanimoto_distance(u32_rogerstanimoto_a_array, u32_rogerstanimoto_b_array, 36);
    BENCH_END(riscv_rogerstanimoto_distance);

    return;
}