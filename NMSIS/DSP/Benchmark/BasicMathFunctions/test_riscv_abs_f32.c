#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/abs_f32/test_data.h"

BENCH_DECLARE_VAR();

void abs_riscv_abs_f32(void)
{
    
    float32_t abs_f32_output[ARRAY_SIZE_F32];
    
    generate_rand_f32(abs_f32_input, ARRAY_SIZE_F32);

    // riscv_abs_f32.c
    BENCH_START(riscv_abs_f32);
    riscv_abs_f32(abs_f32_input, abs_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_abs_f32);

    return;
}
