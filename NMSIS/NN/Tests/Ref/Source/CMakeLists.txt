cmake_minimum_required (VERSION 3.5)

project(REF_NN_IMP)

file(GLOB SRC "./*_*.c")
file(GLOB SRC_ActivationFunctions "ActivationFunctions/*_*.c")
file(GLOB SRC_BasicMathFunctions "BasicMathFunctions/*_*.c")
file(GLOB SRC_ConcatenationFunctions "ConcatenationFunctions/*_*.c")
file(GLOB SRC_ConvolutionFunctions "ConvolutionFunctions/*_*.c")
file(GLOB SRC_FullyConnectedFunctions "FullyConnectedFunctions/*_*.c")
file(GLOB SRC_NNSupportFunctions "NNSupportFunctions/*_*.c")
file(GLOB SRC_PoolingFunctions "PoolingFunctions/*_*.c")
file(GLOB SRC_ReshapeFunctions "ReshapeFunctions/*_*.c")
file(GLOB SRC_SoftmaxFunctions "SoftmaxFunctions/*_*.c")
file(GLOB SRC_SVDFunctions "SVDFunctions/*_*.c")

add_library(REF_NN_IMP STATIC ${SRC}
                              ${SRC_ActivationFunctions}
                              ${SRC_BasicMathFunctions}
                              ${SRC_ConcatenationFunctions}
                              ${SRC_ConvolutionFunctions}
                              ${SRC_FullyConnectedFunctions}
                              ${SRC_NNSupportFunctions}
                              ${SRC_PoolingFunctions}
                              ${SRC_ReshapeFunctions}
                              ${SRC_SoftmaxFunctions}
                              ${SRC_SVDFunctions})

### Includes
target_include_directories(REF_NN_IMP PUBLIC "${REF_NN}/Source")
target_include_directories(REF_NN_IMP PUBLIC "${NN}/Include")
target_include_directories(REF_NN_IMP PUBLIC "${DSP}/Include")
