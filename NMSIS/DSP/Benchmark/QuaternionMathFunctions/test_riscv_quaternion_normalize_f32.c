#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/QuaternionMathFunctions/quaternion_normalize_f32/test_data.h"

BENCH_DECLARE_VAR();

void quaternionNormalize_riscv_quaternion_normalize_f32(void)
{
    float32_t f32_output[4 * ARRAY_SIZE];

    generate_rand_f32(quat_normaliza_f32_input, 4 * ARRAY_SIZE);

    BENCH_START(riscv_quaternion_normalize_f32);
    riscv_quaternion_normalize_f32(quat_normaliza_f32_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_quaternion_normalize_f32);

    return;
}
