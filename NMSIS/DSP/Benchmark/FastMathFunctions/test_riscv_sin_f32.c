#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/sin_f32/test_data.h"

BENCH_DECLARE_VAR();

void sin_riscv_sin_f32(void)
{
    float32_t sin_f32_output[ARRAY_SIZE_F32];
    
    generate_rand_f32(sin_f32_input, ARRAY_SIZE_F32);

    BENCH_START(riscv_sin_f32);
    for (int i = 0; i < ARRAY_SIZE_F32; i++) {
        sin_f32_output[i] = riscv_sin_f32(sin_f32_input[i]);
    }
    BENCH_END(riscv_sin_f32);

    return;
}