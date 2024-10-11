cmake_minimum_required (VERSION 3.14)



target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_cos_f32.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_cos_q15.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_cos_q31.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_sin_f32.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_sin_q15.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_sin_q31.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_sqrt_q31.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_sqrt_q15.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vlog_f32.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vlog_f64.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vexp_f32.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vexp_f64.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vlog_q31.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vlog_q15.c)

if (NOT DISABLEFLOAT16)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vlog_f16.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vexp_f16.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_vinverse_f16.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_atan2_f16.c)
endif()

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_divide_q15.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_divide_q31.c)

target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_atan2_f32.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_atan2_q31.c)
target_sources(NMSISDSP PRIVATE FastMathFunctions/riscv_atan2_q15.c)
