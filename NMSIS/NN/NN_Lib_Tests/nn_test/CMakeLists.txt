cmake_minimum_required (VERSION 3.5)

SET(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
get_filename_component(ROOT "../../.." ABSOLUTE)

project(REF_NN)

option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)

add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})

add_definitions(-g -O2)

###########################
#
# NMSIS NN Reference Library
#
###########################

SET(NN ${ROOT}/NN)
SET(DSP ${ROOT}/DSP)
SET(CORE ${ROOT}/Core)
SET(REF_NN ${NN}/NN_Lib_Tests/nn_test)

INCLUDE_DIRECTORIES(${CORE}/Include)

list(APPEND CMAKE_MODULE_PATH ${REF_NN})

add_library(REF_NN INTERFACE)

add_subdirectory(Ref_Implementations)
target_link_libraries(REF_NN INTERFACE REF_NN_IMP)

### Includes
target_include_directories(REF_NN INTERFACE "${REF_ROOT}/Ref_Implementations")
