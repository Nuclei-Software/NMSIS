cmake_minimum_required(VERSION 3.14)

project(riscv_complex)

INCLUDE_DIRECTORIES(${DSP}/Include)
add_library(riscv_complex STATIC)

file(GLOB SRCS "./riscv_*.c")
#file(GLOB F16SRCS "./riscv_*f16.c")
#list(REMOVE_ITEM SRCS ${F16SRCS})

target_sources(riscv_complex PRIVATE ${SRCS})
