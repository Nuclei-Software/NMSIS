cmake_minimum_required(VERSION 3.5)

project(riscv_controller)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_controller STATIC)

target_sources(riscv_controller PRIVATE riscv_pid_init_f32.c)
target_sources(riscv_controller PRIVATE riscv_pid_init_q15.c)
target_sources(riscv_controller PRIVATE riscv_pid_init_q31.c)
target_sources(riscv_controller PRIVATE riscv_pid_reset_f32.c)
target_sources(riscv_controller PRIVATE riscv_pid_reset_q15.c)
target_sources(riscv_controller PRIVATE riscv_pid_reset_q31.c)
target_sources(riscv_controller PRIVATE riscv_sin_cos_f32.c)
target_sources(riscv_controller PRIVATE riscv_sin_cos_q31.c)
