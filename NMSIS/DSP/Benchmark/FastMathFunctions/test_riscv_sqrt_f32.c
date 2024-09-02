#include "riscv_math.h"
#include <stdio.h>
#include <stdlib.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/sqrt_f32/test_data.h"

BENCH_DECLARE_VAR();

void sqrt_riscv_sqrt_f32(void)
{
    float32_t sqrt_f32_output[ARRAY_SIZE_F32];

    do_srand();
    for (int i = 0; i < ARRAY_SIZE_F32; i++) {
        sqrt_f32_input[i] = (float32_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
        sqrt_f32_input[i] = sqrt_f32_input[i] > 0.0f ? sqrt_f32_input[i] : 0.01f;
    }

    riscv_status result;
    BENCH_START(riscv_sqrt_f32);
    for (volatile int i = 0; i < ARRAY_SIZE_F32; i++) {
        result = riscv_sqrt_f32(sqrt_f32_input[i], &sqrt_f32_output[i]);
    }
    BENCH_END(riscv_sqrt_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
