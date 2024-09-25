cmake_minimum_required (VERSION 3.14)
project(RISCV_NN)

SET(CMAKE_C_COMPILER riscv64-unknown-elf-clang)
SET(CMAKE_CXX_COMPILER riscv64-unknown-elf-clang++)
SET(CMAKE_AR llvm-ar)

# NN Sources
get_filename_component(ROOT "../.." ABSOLUTE)
SET(NN ${ROOT}/NN)


# Select which parts of the NMSIS-NN must be compiled.
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
option(LSTM                 "LSTM"                  ON)
option(PAD                  "Pad"                   ON)
option(TRANSPOSE            "Transpose"             ON)

# Always needed if any other module above is on.
option(NNSUPPORT            "NN Support"            ON)

option(LOOPUNROLL "Loop unrolling" ON)
option(RISCV_UNALIGN, "Unaligned Access" OFF)
option(RISCV_ARCH "option to select riscv arch" rv32imafdc)
option(RISCV_ABI "option to select riscv abi" ilp32d)
option(RISCV_MODEL "option to select mcmodel" medany)


add_definitions(-march=${RISCV_ARCH} -mabi=${RISCV_ABI})
add_definitions(-mcmodel=${RISCV_MODEL})

add_definitions(-g -O2 -ffunction-sections -fdata-sections)
add_definitions(-fno-vectorize -fno-slp-vectorize)

if(LOOPUNROLL)
    add_definitions(-DRISCV_MATH_LOOPUNROLL)
endif()

if(RISCV_UNALIGN)
    add_definitions(-D__RISCV_FEATURE_UNALIGNED)
endif()

###########################
#
# NMSIS NN
#
###########################

add_library(NMSISNN STATIC)

if (BASICMATHSNN)
  add_subdirectory(BasicMathFunctions)
endif()

if (CONCATENATION)
  add_subdirectory(ConcatenationFunctions)
endif()

if (FULLYCONNECTED)
  add_subdirectory(FullyConnectedFunctions)
endif()

if (CONVOLUTION)
  add_subdirectory(ConvolutionFunctions)
endif()

if (ACTIVATION)
  add_subdirectory(ActivationFunctions)
endif()

if (POOLING)
  add_subdirectory(PoolingFunctions)
endif()

if (SOFTMAX)
  add_subdirectory(SoftmaxFunctions)
endif()

if (SVDF)
  add_subdirectory(SVDFunctions)
endif()

if (LSTM)
  add_subdirectory(LSTMFunctions)
endif()

if (RESHAPE)
  add_subdirectory(ReshapeFunctions)
endif()

if (PAD)
  add_subdirectory(PadFunctions)
endif()

if (TRANSPOSE)
  add_subdirectory(TransposeFunctions)
endif()

# Keep NNSUPPORT at the end
if (NNSUPPORT)
  add_subdirectory(NNSupportFunctions)
endif()

### Includes
target_include_directories(NMSISNN PUBLIC "${NN}/Include")
target_include_directories(NMSISNN PUBLIC ${ROOT}/Core/Include)
