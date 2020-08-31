cmake_minimum_required (VERSION 3.5)

SET(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
#SET(CMAKE_C_COMPILER riscv32-unknown-elf-gcc)
#SET(CMAKE_CXX_COMPILER riscv32-unknown-elf-g++)
get_filename_component(ROOT "../.." ABSOLUTE)

project(RISCV_NN)

# Select which parts of the NMSIS-DSP must be compiled.
# There are some dependencies between the parts but they are not tracked
# by this cmake. So, enabling some functions may require to enable some
# other ones.
option(FULLYCONNECTED        "Fully Connected"      ON)
option(CONVOLUTION           "Convolutions"         ON)
option(ACTIVATION            "Activations"          ON)
option(POOLING               "Pooling"              ON)
option(SOFTMAX               "Softmax"              ON)

# When OFF it is the default behavior : all tables are included.
option(NNSUPPORT             "NN Support"           ON)

option(DSP_ENABLE "option to enable dsp" ON)
option(LOOPUNROLL "option to enable unroll loop" ON)
option(DSP64 "option to enable dsp 64" ON)
option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)


add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
add_definitions(-mcmodel=medany)

add_definitions(-g -O2 -ffunction-sections -fdata-sections)
if (DSP_ENABLE)
    add_definitions(-D__RISCV_FEATURE_DSP=1)
    if(LOOPUNROLL)
        add_definitions(-DRISCV_MATH_LOOPUNROLL)
    endif()
    if(DSP64)
        add_definitions(-DRISCV_DSP64)
    endif()
endif()

# DSP and NN Sources
SET(NN ${ROOT}/NN)
SET(DSP ${ROOT}/DSP)
SET(CORE ${ROOT}/Core)

INCLUDE_DIRECTORIES(${CORE}/Include)

list(APPEND CMAKE_MODULE_PATH ${NN}/Source)

add_library(RISCV_NN INTERFACE)

if (FULLYCONNECTED)
  add_subdirectory(FullyConnectedFunctions)
  target_link_libraries(RISCV_NN INTERFACE RISCV_NNFullyConnected)
endif()

if (CONVOLUTION)
  add_subdirectory(ConvolutionFunctions)
  target_link_libraries(RISCV_NN INTERFACE RISCV_NNConvolutions)
endif()

if (ACTIVATION)
  add_subdirectory(ActivationFunctions)
  target_link_libraries(RISCV_NN INTERFACE RISCV_NNActivation)
endif()

if (POOLING)
  add_subdirectory(PoolingFunctions)
  target_link_libraries(RISCV_NN INTERFACE RISCV_NNPooling)
endif()

if (SOFTMAX)
  add_subdirectory(SoftmaxFunctions)
  target_link_libraries(RISCV_NN INTERFACE RISCV_NNSoftmax)
endif()

if (NNSUPPORT)
  add_subdirectory(NNSupportFunctions)
  target_link_libraries(RISCV_NN INTERFACE RISCV_NNSupport)
endif()

### Includes
target_include_directories(RISCV_NN INTERFACE "${NN}/Include")
target_include_directories(RISCV_NN INTERFACE "${DSP}/Include")
