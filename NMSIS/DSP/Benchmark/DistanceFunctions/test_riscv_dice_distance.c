#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "array.h"

BENCH_DECLARE_VAR();

void dice_distance_riscv_dice_distance(void)
{
    float32_t dice_distance_f32_output;

    BENCH_START(riscv_dice_distance);
    dice_distance_f32_output = riscv_dice_distance(u32_dice_a_array, u32_dice_b_array, 36);
    BENCH_END(riscv_dice_distance);

    return;
}
