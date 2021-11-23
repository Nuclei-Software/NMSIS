cmake_minimum_required(VERSION 3.5)

SET(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
SET(CMAKE_AR riscv-nuclei-elf-ar)

get_filename_component(ROOT "../.." ABSOLUTE)

project(RISCV_DSP)

option(LOOPUNROLL "option to enable unroll loop" ON)
option(ROUNDING "Rounding" OFF)
option(MATRIXCHECK "Matrix Checks" OFF)
option(RISCV_UNALIGN, "Unaligned Access" OFF)
option(DSP64 "option to enable dsp 64" ON)
option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)
option(RISCV_MODEL "option to select mcmodel" medany)

add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
add_definitions(-mcmodel=${RISCV_MODEL})

add_definitions(-g -O2 -ffunction-sections -fdata-sections)

if(DSP64)
    add_definitions(-DRISCV_DSP64)
endif()

if(RISCV_UNALIGN)
    add_definitions(-D__RISCV_FEATURE_UNALIGNED)
endif()

if(ROUNDING)
    add_definitions(-DRISCV_MATH_ROUNDING)
endif()

if(MATRIXCHECK)
    add_definitions(-DRISCV_MATH_MATRIX_CHECK)
endif()

add_library(RISCV_DSP INTERFACE)

INCLUDE_DIRECTORIES(${ROOT}/Core/Include)
INCLUDE_DIRECTORIES(${ROOT}/DSP/PrivateInclude)

SET(DSP ${ROOT}/DSP)

add_subdirectory(QuaternionMathFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_quaternion_math)

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

add_subdirectory(BayesFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_bayes)

add_subdirectory(InterpolationFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_interpolation)

add_subdirectory(SVMFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_svm)

add_subdirectory(DistanceFunctions)
target_link_libraries(RISCV_DSP INTERFACE riscv_distance)
