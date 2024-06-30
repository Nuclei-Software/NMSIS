#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/DistanceFunctions/cityblock_distance_f32/test_data.h"

BENCH_DECLARE_VAR();

void cityblock_distance_riscv_cityblock_distance_f32(void)
{
    float32_t cityblock_distance_f32_output;

    generate_rand_f32(cityblock_distance_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(cityblock_distance_f32_input2, ARRAY_SIZE_F32);

    BENCH_START(riscv_cityblock_distance_f32);
    cityblock_distance_f32_output = riscv_cityblock_distance_f32(cityblock_distance_f32_input1, cityblock_distance_f32_input2, ARRAY_SIZE_F32);
    BENCH_END(riscv_cityblock_distance_f32);

    return;
}