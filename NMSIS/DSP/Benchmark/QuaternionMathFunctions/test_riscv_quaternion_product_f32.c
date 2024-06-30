#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/QuaternionMathFunctions/quaternion_product_f32/test_data.h"

BENCH_DECLARE_VAR();

void quaternionProduct_riscv_quaternion_product_f32(void)
{
    float32_t f32_output[4 * ARRAY_SIZE];

    generate_rand_f32(quat_product_f32_input1, 4 * ARRAY_SIZE);
    generate_rand_f32(quat_product_f32_input2, 4 * ARRAY_SIZE);

    BENCH_START(riscv_quaternion_product_f32);
    riscv_quaternion_product_f32(quat_product_f32_input1, quat_product_f32_input2, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_quaternion_product_f32);

    return;
}
