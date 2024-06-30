#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void kulsinski_distance_riscv_kulsinski_distance(void)
{
    float32_t kulsinski_distance_f32_output;

    BENCH_START(riscv_kulsinski_distance);
    kulsinski_distance_f32_output = riscv_kulsinski_distance(u32_kulsinski_a_array, u32_kulsinski_b_array, 36);
    BENCH_END(riscv_kulsinski_distance);

    return;
}
