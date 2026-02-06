cmake_minimum_required (VERSION 3.14)



if (FASTBUILD)
  target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/QuaternionMathFunctions.c)


else()


target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion_norm_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion_inverse_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion_conjugate_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion_normalize_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion_product_single_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion_product_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_quaternion2rotation_f32.c)
target_sources(NMSISDSP PRIVATE QuaternionMathFunctions/riscv_rotation2quaternion_f32.c)


if (NOT DISABLEFLOAT16)
endif()
endif()




