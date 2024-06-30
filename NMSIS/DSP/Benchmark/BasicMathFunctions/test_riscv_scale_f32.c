#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/scale_f32/test_data.h"

BENCH_DECLARE_VAR();

void scale_riscv_scale_f32(void)
{
    float32_t scale_f32_output[ARRAY_SIZE_F32];
    float32_t scale_f32;
    generate_rand_f32(scale_f32_input, ARRAY_SIZE_F32);
    generate_rand_f32(&scale_f32, 1);
    BENCH_START(riscv_scale_f32);
    riscv_scale_f32(scale_f32_input, scale_f32, scale_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_scale_f32);
}