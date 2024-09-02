#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/dot_prod_f32/test_data.h"

BENCH_DECLARE_VAR();

void dot_prod_riscv_dot_prod_f32(void)
{
    float32_t dot_prod_f32_output;

    generate_rand_f32(dot_prod_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(dot_prod_f32_input2, ARRAY_SIZE_F32);

    // f32_dot
    BENCH_START(riscv_dot_prod_f32);
    riscv_dot_prod_f32(dot_prod_f32_input1, dot_prod_f32_input2, ARRAY_SIZE_F32, &dot_prod_f32_output);
    BENCH_END(riscv_dot_prod_f32);
}