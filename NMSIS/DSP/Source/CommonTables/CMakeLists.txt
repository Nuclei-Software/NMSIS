cmake_minimum_required(VERSION 3.5)

project(riscv_common_table)

INCLUDE_DIRECTORIES(${DSP}/Include)
add_library(riscv_common_table STATIC)

target_sources(riscv_common_table PRIVATE riscv_common_tables.c)
target_sources(riscv_common_table PRIVATE riscv_realcoef_tables.c)
target_sources(riscv_common_table PRIVATE riscv_twiddlecoef_rfft_tables.c)
target_sources(riscv_common_table PRIVATE riscv_twiddlecoef_tables.c)
target_sources(riscv_common_table PRIVATE riscv_dct4_f32_8192_tables.c)
target_sources(riscv_common_table PRIVATE riscv_dct4_q15_8192_tables.c)
target_sources(riscv_common_table PRIVATE riscv_dct4_q31_8192_tables.c)
target_sources(riscv_common_table PRIVATE riscv_const_structs.c)

