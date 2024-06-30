#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void sokalmichener_distance_riscv_sokalmichener_distance(void)
{
    float32_t sokalmichener_distance_f32_output;

    BENCH_START(riscv_sokalmichener_distance);
    sokalmichener_distance_f32_output = riscv_sokalmichener_distance(u32_sokalmichener_a_array, u32_sokalmichener_b_array, 36);
    BENCH_END(riscv_sokalmichener_distance);

    return;
}