#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/scale_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void scale_riscv_scale_f16(void)
{
    float16_t scale_f16_output[ARRAY_SIZE_F16];
    float16_t scale_f16;
    generate_rand_f16(scale_f16_input, ARRAY_SIZE_F16);
    generate_rand_f16(&scale_f16, 1);
    BENCH_START(riscv_scale_f16);
    riscv_scale_f16(scale_f16_input, scale_f16, scale_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_scale_f16);
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */