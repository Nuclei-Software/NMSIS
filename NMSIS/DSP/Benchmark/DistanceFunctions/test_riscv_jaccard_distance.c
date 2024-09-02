#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void jaccard_distance_riscv_jaccard_distance(void)
{
    float32_t jaccard_distance_f32_output;

    BENCH_START(riscv_jaccard_distance);
    jaccard_distance_f32_output = riscv_jaccard_distance(u32_jaccard_a_array, u32_jaccard_b_array, 36);
    BENCH_END(riscv_jaccard_distance);

    return;
}
