cmake_minimum_required(VERSION 3.14)

project(riscv_distance)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_distance STATIC)

file(GLOB SRCS "./riscv_*.c")
file(GLOB F64SRCS "./riscv_*f64.c")
list(REMOVE_ITEM SRCS ${F64SRCS})

target_sources(riscv_distance PRIVATE ${SRCS})

target_include_directories(riscv_distance PRIVATE ".")
