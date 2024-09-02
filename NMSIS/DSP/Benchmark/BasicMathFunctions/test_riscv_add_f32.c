#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/add_f32/test_data.h"

BENCH_DECLARE_VAR();

void add_riscv_add_f32(void)
{
    float32_t add_f32_output[ARRAY_SIZE_F32];

    generate_rand_f32(add_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(add_f32_input2, ARRAY_SIZE_F32);

    // riscv_add_f32.c
    BENCH_START(riscv_add_f32);
    riscv_add_f32(add_f32_input1, add_f32_input2, add_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_add_f32);
}