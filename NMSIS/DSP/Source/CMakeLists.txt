cmake_minimum_required(VERSION 3.5)

SET(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
#SET(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
#SET(CMAKE_CXX_COMPILER riscv32-unknown-elf-g++)
get_filename_component(ROOT "../.." ABSOLUTE)

project(RISCV_DSP)

option(DSP_ENABLE "option to enable dsp" ON)
option(LOOPUNROLL "option to enable unroll loop" ON)
option(ROUNDING "Rounding" OFF)
option(MATRIXCHECK "Matrix Checks" OFF)
option(RISCV_ALIGN, "Aligned Access" OFF)
option(DSP64 "option to enable dsp 64" ON)
option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)

add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})

add_definitions(-mcmodel=medany)

add_definitions(-g -O2)
if (DSP_ENABLE)
    add_definitions(-D__RISCV_FEATURE_DSP=1)
    if(LOOPUNROLL)
        add_definitions(-DRISCV_MATH_LOOPUNROLL)
    endif()
    if(DSP64)
        add_definitions(-DRISCV_DSP64)
    endif()
else()
    if(RISCV_ALIGN)
    add_definitions(-DRISCV_ALIGN_ACCESS)
    endif()
endif()

if(ROUNDING)
    add_definitions(-DRISCV_MATH_ROUNDING)
endif()

if(MATRIXCHECK)
    add_definitions(-DRISCV_MATH_MATRIX_CHECK)
endif()

add_library(RISCV_DSP INTERFACE)

INCLUDE_DIRECTORIES(${ROOT}/Core/Include)

SET(DSP ${ROOT}/DSP)

add_subdirectory(FastMathFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_fast_math)

add_subdirectory(CommonTables)
target_link_libraries(RISCV_DSP INTERFACE riscv_common_table)

add_subdirectory(BasicMathFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_base_math)

add_subdirectory(MatrixFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_matrix)

add_subdirectory(FilteringFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_filtering)

add_subdirectory(SupportFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_support)

add_subdirectory(StatisticsFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_statistics)

add_subdirectory(ComplexMathFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_complex)

add_subdirectory(ControllerFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_controller)

add_subdirectory(TransformFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_transform)
