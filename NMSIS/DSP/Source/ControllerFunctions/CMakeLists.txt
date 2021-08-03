cmake_minimum_required(VERSION 3.5)

project(riscv_controller)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_controller STATIC)

file(GLOB SRCS "./riscv_*.c")

target_sources(riscv_controller PRIVATE ${SRCS})
