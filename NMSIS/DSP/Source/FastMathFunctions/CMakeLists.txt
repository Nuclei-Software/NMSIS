cmake_minimum_required(VERSION 3.5)

project(riscv_fast_math)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_fast_math STATIC)

target_sources(riscv_fast_math PRIVATE riscv_cos_f32.c)
target_sources(riscv_fast_math PRIVATE riscv_cos_q15.c)
target_sources(riscv_fast_math PRIVATE riscv_cos_q31.c)
target_sources(riscv_fast_math PRIVATE riscv_sin_f32.c)
target_sources(riscv_fast_math PRIVATE riscv_sin_q15.c)
target_sources(riscv_fast_math PRIVATE riscv_sin_q31.c)
target_sources(riscv_fast_math PRIVATE riscv_sqrt_q15.c)
target_sources(riscv_fast_math PRIVATE riscv_sqrt_q31.c)
