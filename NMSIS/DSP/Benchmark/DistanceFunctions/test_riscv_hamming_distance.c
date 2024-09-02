#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void hamming_distance_riscv_hamming_distance(void)
{
    float32_t hamming_distance_f32_output;

    BENCH_START(riscv_hamming_distance);
    hamming_distance_f32_output = riscv_hamming_distance(u32_hamming_a_array, u32_hamming_b_array, 36);
    BENCH_END(riscv_hamming_distance);

    return;
}
