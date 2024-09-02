#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/DistanceFunctions/correlation_distance_f32/test_data.h"

BENCH_DECLARE_VAR();

void correlation_distance_riscv_correlation_distance_f32(void)
{
    float32_t correlation_distance_f32_output;

    generate_rand_f32(correlation_distance_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(correlation_distance_f32_input2, ARRAY_SIZE_F32);

    BENCH_START(riscv_correlation_distance_f32);
    correlation_distance_f32_output = riscv_correlation_distance_f32(correlation_distance_f32_input1, correlation_distance_f32_input2, ARRAY_SIZE_F32);
    BENCH_END(riscv_correlation_distance_f32);

    return;
}