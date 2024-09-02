#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/mult_f32/test_data.h"

BENCH_DECLARE_VAR();

void mul_riscv_mul_f32(void)
{
    float32_t mult_f32_output[ARRAY_SIZE_F32];

    generate_rand_f32(mult_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(mult_f32_input2, ARRAY_SIZE_F32);

    // f32_mult
    BENCH_START(riscv_mult_f32);
    riscv_mult_f32(mult_f32_input1, mult_f32_input2, mult_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_mult_f32);
}