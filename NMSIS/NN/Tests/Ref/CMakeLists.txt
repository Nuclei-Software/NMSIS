cmake_minimum_required (VERSION 3.5)

SET(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
SET(CMAKE_AR riscv-nuclei-elf-ar)

get_filename_component(ROOT "../../.." ABSOLUTE)

project(REF_NN)

option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)
option(RISCV_MODEL "option to select mcmodel" medany)

add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
add_definitions(-mcmodel=${RISCV_MODEL})

add_definitions(-g)

###########################
#
# NMSIS NN Reference Library
#
###########################

SET(NN ${ROOT}/NN)
SET(DSP ${ROOT}/DSP)
SET(CORE ${ROOT}/Core)
SET(REF_NN ${NN}/Tests/Ref)

INCLUDE_DIRECTORIES(${CORE}/Include)

list(APPEND CMAKE_MODULE_PATH ${REF_NN})

add_library(REF_NN INTERFACE)

add_subdirectory(Source)
target_link_libraries(REF_NN INTERFACE REF_NN_IMP)

### Includes
target_include_directories(REF_NN INTERFACE "${REF_NN}/Source")
