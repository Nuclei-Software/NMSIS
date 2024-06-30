#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/QuaternionMathFunctions/quaternion2rotation_f32/test_data.h"

BENCH_DECLARE_VAR();

void quaternion2Rotation_riscv_quaternion2rotation_f32(void)
{
    float32_t f32_output[9 * ARRAY_SIZE];

    generate_rand_f32(quaternion2rotation_f32_input, 9 * ARRAY_SIZE);

    BENCH_START(riscv_quaternion2rotation_f32);
    riscv_quaternion2rotation_f32(quaternion2rotation_f32_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_quaternion2rotation_f32);

    return;
}
