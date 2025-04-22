cmake_minimum_required(VERSION 3.14)

project(NMSISDSP)

SET(CMAKE_C_COMPILER riscv64-unknown-elf-clang)
SET(CMAKE_CXX_COMPILER riscv64-unknown-elf-clang++)
SET(CMAKE_AR llvm-ar)

# DSP Sources
get_filename_component(ROOT "../.." ABSOLUTE)
SET(DSP ${ROOT}/DSP)

option(LOOPUNROLL "Loop unrolling" ON)
option(ROUNDING "Rounding" OFF)
option(MATRIXCHECK "Matrix Checks" OFF)
option(RISCV_UNALIGN, "Unaligned Access" OFF)
option(DISABLEFLOAT16 "Disable building float16 kernels" OFF)
option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)
option(RISCV_MODEL "option to select mcmodel" medany)
option(DSP_ATTRIBUTE "option to enable attribute_weak" ON)

add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
add_definitions(-mcmodel=${RISCV_MODEL})

add_definitions(-g -O2 -ffunction-sections -fdata-sections)

if(LOOPUNROLL)
    add_definitions(-DRISCV_MATH_LOOPUNROLL)
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

if(DSP_ATTRIBUTE)
set(ATTR_WEAK "__attribute__((weak))")
add_definitions(-DRISCV_DSP_ATTRIBUTE=${ATTR_WEAK})
endif()

###########################
#
# NMSIS DSP
#
###########################

add_library(NMSISDSP STATIC)

include(BasicMathFunctions/Config.cmake)

include(ComplexMathFunctions/Config.cmake)

include(QuaternionMathFunctions/Config.cmake)

include(ControllerFunctions/Config.cmake)

include(FastMathFunctions/Config.cmake)

include(FilteringFunctions/Config.cmake)

include(MatrixFunctions/Config.cmake)

include(StatisticsFunctions/Config.cmake)

include(SupportFunctions/Config.cmake)

include(TransformFunctions/Config.cmake)

include(CommonTables/Config.cmake)

include(SVMFunctions/Config.cmake)

include(BayesFunctions/Config.cmake)

include(DistanceFunctions/Config.cmake)

include(InterpolationFunctions/Config.cmake)

include(WindowFunctions/Config.cmake)

### Includes
target_include_directories(NMSISDSP PUBLIC "${DSP}/Include")
target_include_directories(NMSISDSP PRIVATE "${DSP}/PrivateInclude")
target_include_directories(NMSISDSP PUBLIC ${ROOT}/Core/Include)
