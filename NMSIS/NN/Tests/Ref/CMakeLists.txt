cmake_minimum_required (VERSION 3.14)

SET(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)
SET(CMAKE_AR riscv64-unknown-elf-ar)

project(REF_NN)

get_filename_component(ROOT "../../.." ABSOLUTE)
SET(REF ${ROOT}/NN/Tests/Ref)

option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)
option(RISCV_MODEL "option to select mcmodel" medany)

add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
add_definitions(-mcmodel=${RISCV_MODEL})

add_definitions(-g -O2)

###########################
#
# NMSIS NN Reference Library
#
###########################

add_library(REF_NN INTERFACE)

add_subdirectory(Source)
target_link_libraries(REF_NN INTERFACE REF_NN_IMP)

### Includes
target_include_directories(REF_NN INTERFACE "${ROOT}/Core/Include")
target_include_directories(REF_NN INTERFACE "${REF}/Source")
