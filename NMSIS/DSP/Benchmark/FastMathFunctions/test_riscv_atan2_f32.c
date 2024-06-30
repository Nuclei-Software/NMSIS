#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/atan2_f32/test_data.h"

BENCH_DECLARE_VAR();

void cos_riscv_atan2_f32(void)
{

    float32_t atan2_f32_output[ARRAY_SIZE_F32];

    generate_rand_f32(atan2_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(atan2_f32_input2, ARRAY_SIZE_F32);

    riscv_status result;
    BENCH_START(riscv_atan2_f32);
    for (int i = 0; i < ARRAY_SIZE_F32; i++) {
        result = riscv_atan2_f32(atan2_f32_input1[i], atan2_f32_input2[i], &atan2_f32_output[i]);
    }
    BENCH_END(riscv_atan2_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
