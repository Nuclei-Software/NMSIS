cmake_minimum_required(VERSION 3.14)

project(riscv_window)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_window STATIC)

file(GLOB SRCS "./riscv_*.c")

target_sources(riscv_window PRIVATE ${SRCS})
