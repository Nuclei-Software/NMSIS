#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/DistanceFunctions/jensenshannon_distance_f32/test_data.h"

BENCH_DECLARE_VAR();

void jensenshannon_distance_riscv_jensenshannon_distance_f32(void)
{
    float32_t jensenshannon_distance_f32_output;

    generate_rand_f32(jensenshannon_distance_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(jensenshannon_distance_f32_input2, ARRAY_SIZE_F32);

    BENCH_START(riscv_jensenshannon_distance_f32);
    jensenshannon_distance_f32_output = riscv_jensenshannon_distance_f32(jensenshannon_distance_f32_input1, jensenshannon_distance_f32_input2, ARRAY_SIZE_F32);
    BENCH_END(riscv_jensenshannon_distance_f32);

    return;
}