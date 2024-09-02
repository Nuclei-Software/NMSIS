#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/QuaternionMathFunctions/quaternion_product_single_f32/test_data.h"

BENCH_DECLARE_VAR();

void quaternionProductSingle_riscv_quaternion_product_single_f32(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_f32(quat_product_sing_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_quaternion_product_single_f32);
    riscv_quaternion_product_single_f32(quat_product_sing_f32_input, f32_output, f32_output);
    BENCH_END(riscv_quaternion_product_single_f32);
}
