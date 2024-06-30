#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void sokalsneath_distance_riscv_sokalsneath_distance(void)
{
    float32_t sokalsneath_distance_f32_output;

    BENCH_START(riscv_sokalsneath_distance);
    sokalsneath_distance_f32_output = riscv_sokalsneath_distance(u32_sokalsneath_a_array, u32_sokalsneath_b_array, 36);
    BENCH_END(riscv_sokalsneath_distance);

    return;
}