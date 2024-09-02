#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/clip_f32/test_data.h"

BENCH_DECLARE_VAR();

void clip_riscv_clip_f32(void)
{
    float32_t low_f32 = LOWER_BOUND_F32;
    float32_t high_f32 = HIGHER_BOUND_F32;
    float32_t clip_f32_output[ARRAY_SIZE_F32];

    generate_rand_f32(clip_f32_input, ARRAY_SIZE_F32);

    BENCH_START(riscv_clip_f32);
    riscv_clip_f32(clip_f32_input, clip_f32_output, low_f32, high_f32, ARRAY_SIZE_F32);
    BENCH_END(riscv_clip_f32);
}