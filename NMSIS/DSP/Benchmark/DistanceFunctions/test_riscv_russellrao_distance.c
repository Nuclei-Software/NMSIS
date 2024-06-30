#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void russellrao_distance_riscv_russellrao_distance(void)
{
    float32_t russellrao_distance_f32_output;

    BENCH_START(riscv_russellrao_distance);
    russellrao_distance_f32_output = riscv_russellrao_distance(u32_russellrao_a_array, u32_russellrao_b_array, 36);
    BENCH_END(riscv_russellrao_distance);

    return;
}