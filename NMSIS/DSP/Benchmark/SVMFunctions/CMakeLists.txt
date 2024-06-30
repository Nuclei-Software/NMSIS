cmake_minimum_required(VERSION 3.14)

project(riscv_svm)

INCLUDE_DIRECTORIES(${DSP}/Include)

add_library(riscv_svm STATIC)

file(GLOB SRCS "./riscv_*.c")
#file(GLOB F16SRCS "./riscv_*f16.c")
#list(REMOVE_ITEM SRCS ${F16SRCS})

target_sources(riscv_svm PRIVATE ${SRCS})
