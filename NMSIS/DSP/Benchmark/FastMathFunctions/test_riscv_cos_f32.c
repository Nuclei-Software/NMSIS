#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/cos_f32/test_data.h"

BENCH_DECLARE_VAR();

void cos_riscv_cos_f32(void)
{
    float32_t cos_f32_output[ARRAY_SIZE_F32];
    
    generate_rand_f32(cos_f32_input, ARRAY_SIZE_F32);

    BENCH_START(riscv_cos_f32);
    for (int i = 0; i < ARRAY_SIZE_F32; i++) {
        cos_f32_output[i] = riscv_cos_f32(cos_f32_input[i]);
    }
    BENCH_END(riscv_cos_f32);

    return;
}