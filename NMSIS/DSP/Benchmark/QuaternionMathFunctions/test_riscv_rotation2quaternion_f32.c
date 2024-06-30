#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/QuaternionMathFunctions/rotation2quaternion_f32/test_data.h"

BENCH_DECLARE_VAR();

void rotation2quaternion_riscv_rotation2quaternion_f32(void)
{
    float32_t f32_output[9 * ARRAY_SIZE];

    generate_rand_f32(rotation2quaternion_f32_input, 9 * ARRAY_SIZE);

    BENCH_START(riscv_rotation2quaternion_f32);
    riscv_rotation2quaternion_f32(rotation2quaternion_f32_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_rotation2quaternion_f32);

    return;
}
