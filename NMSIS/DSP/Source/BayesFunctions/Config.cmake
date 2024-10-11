cmake_minimum_required (VERSION 3.14)


target_sources(NMSISDSP PRIVATE BayesFunctions/riscv_gaussian_naive_bayes_predict_f32.c)

if (NOT DISABLEFLOAT16)
target_sources(NMSISDSP PRIVATE BayesFunctions/riscv_gaussian_naive_bayes_predict_f16.c)
endif()
