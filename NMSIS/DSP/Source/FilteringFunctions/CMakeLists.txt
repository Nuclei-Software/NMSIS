cmake_minimum_required(VERSION 3.5)

project(riscv_filtering)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_filtering STATIC)

file(GLOB SRCS "./riscv_*.c")
file(GLOB F16SRCS "./riscv_*f16.c")
list(REMOVE_ITEM SRCS ${F16SRCS})

target_sources(riscv_filtering PRIVATE ${SRCS})
