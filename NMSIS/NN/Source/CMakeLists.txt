cmake_minimum_required (VERSION 3.5)

SET(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
SET(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
SET(CMAKE_AR riscv-nuclei-elf-ar)

get_filename_component(ROOT "../.." ABSOLUTE)

project(RISCV_NN)

# Select which parts of the NMSIS-DSP must be compiled.
# There are some dependencies between the parts but they are not tracked
# by this cmake. So, enabling some functions may require to enable some
# other ones.
option(CONCATENATION        "Concatenation"         ON)
option(FULLYCONNECTED       "Fully Connected"       ON)
option(CONVOLUTION          "Convolutions"          ON)
option(ACTIVATION           "Activations"           ON)
option(POOLING              "Pooling"               ON)
option(SOFTMAX              "Softmax"               ON)
option(BASICMATHSNN         "Basic Maths for NN"    ON)
option(RESHAPE              "Reshape"               ON)
option(SVDF                 "SVDF"                  ON)

# When OFF it is the default behavior : all tables are included.
option(NNSUPPORT             "NN Support"           ON)

option(DSP_ENABLE "option to enable dsp" ON)
option(VECTOR_ENABLE "option to enable vector" ON)
option(LOOPUNROLL "option to enable unroll loop" ON)
option(DSP64 "option to enable dsp 64" ON)
option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)
option(RISCV_UNALIGN, "Unaligned Access" OFF)

if(VECTOR_ENABLE)
add_definitions(-march=${RISCV_ARCH}v -mabi=${RISCV_ABI})
else()
add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
endif()
add_definitions(-mcmodel=medany)

add_definitions(-g -O2 -ffunction-sections -fdata-sections)
if (DSP_ENABLE)
  if(VECTOR_ENABLE)
    add_definitions(-D__RISCV_FEATURE_DSP=1 -D__RISCV_FEATURE_VECTOR=1 -D__RISCV_FEATURE_VECTOR=1)
    if(LOOPUNROLL)
        add_definitions(-DRISCV_MATH_LOOPUNROLL)
    endif()
    if(DSP64)
        add_definitions(-DRISCV_DSP64)
    endif()
  else()
    add_definitions(-D__RISCV_FEATURE_DSP=1)
    if(LOOPUNROLL)
        add_definitions(-DRISCV_MATH_LOOPUNROLL)
    endif()
    if(DSP64)
        add_definitions(-DRISCV_DSP64)
    endif()
  endif()
elseif(VECTOR_ENABLE)
    add_definitions(-D__RISCV_FEATURE_VECTOR=1 -D__RISCV_FEATURE_VECTOR=1)
    if(DSP64)
        add_definitions(-DRISCV_DSP64)
    endif()
else()
    if(RISCV_UNALIGN)
    add_definitions(-D__RISCV_FEATURE_UNALIGNED)
    endif()
endif()

# DSP and NN Sources
SET(NN ${ROOT}/NN)
SET(DSP ${ROOT}/DSP)
SET(CORE ${ROOT}/Core)

INCLUDE_DIRECTORIES(${CORE}/Include)

list(APPEND CMAKE_MODULE_PATH ${NN}/Source)

add_library(RISCV_NN INTERFACE)

if (BASICMATHSNN)
  add_subdirectory(BasicMathFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNBasicMaths)
endif()

if (CONCATENATION)
  add_subdirectory(ConcatenationFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNConcatenation)
endif()

if (FULLYCONNECTED)
  add_subdirectory(FullyConnectedFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNFullyConnected)
endif()

if (CONVOLUTION)
  add_subdirectory(ConvolutionFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNConvolutions)
endif()

if (ACTIVATION)
  add_subdirectory(ActivationFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNActivation)
endif()

if (POOLING)
  add_subdirectory(PoolingFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNPooling)
endif()

if (SOFTMAX)
  add_subdirectory(SoftmaxFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNSoftmax)
endif()

if (SVDF)
  add_subdirectory(SVDFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNSVDF)
endif()

if (RESHAPE)
  add_subdirectory(ReshapeFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNReshape)
endif()

# Keep NNSUPPORT at the end
if (NNSUPPORT)
  add_subdirectory(NNSupportFunctions)
  target_link_libraries(RISCV_NN INTERFACE NMSISNNSupport)
endif()

### Includes
target_include_directories(RISCV_NN INTERFACE "${NN}/Include")
target_include_directories(RISCV_NN INTERFACE "${DSP}/Include")
