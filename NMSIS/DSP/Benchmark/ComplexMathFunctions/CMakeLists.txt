cmake_minimum_required(VERSION 3.14)

project(riscv_complex)

INCLUDE_DIRECTORIES(${DSP}/Include)
add_library(riscv_complex STATIC)

file(GLOB SRCS "./riscv_*.c")
file(GLOB F64SRCS "./riscv_*f64.c")
list(REMOVE_ITEM SRCS ${F64SRCS})

target_sources(riscv_complex PRIVATE ${SRCS})
