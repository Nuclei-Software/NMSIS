#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/clip_f16/test_data.h"

BENCH_DECLARE_VAR();

void clip_riscv_clip_f16(void)
{
#if defined(RISCV_FLOAT16_SUPPORTED)
    float16_t low_f16 = LOWER_BOUND_F16;
    float16_t high_f16 = HIGHER_BOUND_F16;
    float16_t clip_f16_output[ARRAY_SIZE_F16];

    generate_rand_f16(clip_f16_input, ARRAY_SIZE_F16);

    BENCH_START(riscv_clip_f16);
    riscv_clip_f16(clip_f16_input, clip_f16_output, low_f16, high_f16, ARRAY_SIZE_F16);
    BENCH_END(riscv_clip_f16);
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
}