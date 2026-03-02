cmake_minimum_required (VERSION 3.14)



if (FASTBUILD)
  target_sources(NMSISDSP PRIVATE InterpolationFunctions/InterpolationFunctions.c)

  if (NOT DISABLEFLOAT16)
    target_sources(NMSISDSP PRIVATE InterpolationFunctions/InterpolationFunctionsF16.c)
  endif()

else()

target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_bilinear_interp_f32.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_bilinear_interp_q15.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_bilinear_interp_q31.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_bilinear_interp_q7.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_linear_interp_f32.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_linear_interp_q15.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_linear_interp_q31.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_linear_interp_q7.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_spline_interp_f32.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_spline_interp_init_f32.c)



if (NOT DISABLEFLOAT16)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_bilinear_interp_f16.c)
target_sources(NMSISDSP PRIVATE InterpolationFunctions/riscv_linear_interp_f16.c)
endif()

endif()
