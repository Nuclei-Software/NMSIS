cmake_minimum_required(VERSION 3.5)

project(riscv_quaternion_math)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_quaternion_math STATIC)

file(GLOB SRCS "./riscv_*.c")
#file(GLOB F16SRCS "./riscv_*f16.c")
#list(REMOVE_ITEM SRCS ${F16SRCS})

target_sources(riscv_quaternion_math PRIVATE ${SRCS})
