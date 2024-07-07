#include <stdio.h>
#include "riscv_math.h"
#include "funcs_def.h"

int main()
{
    printf("Start QuaternionMathFunctions benchmark test:\n");

    quaternionConjugate_riscv_quaternion_conjugate_f32();
    quaternionInverse_riscv_quaternion_inverse_f32();
    quaternionNorm_riscv_quaternion_norm_f32();
    quaternionNormalize_riscv_quaternion_normalize_f32();
    quaternionProduct_riscv_quaternion_product_f32();
    quaternionProductSingle_riscv_quaternion_product_single_f32();
    quaternion2Rotation_riscv_quaternion2rotation_f32();
    rotation2quaternion_riscv_rotation2quaternion_f32();

    printf("All tests are passed.\n");
    printf("test for QuaternionMathFunctions benchmark finished.\n");
}
