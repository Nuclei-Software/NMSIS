cmake_minimum_required (VERSION 3.14)

project(NMSISNNSoftmax)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNSoftmax STATIC ${SRC})

### Includes
target_include_directories(NMSISNNSoftmax PUBLIC "${NN}/Include")
target_include_directories(NMSISNNSoftmax PUBLIC "${DSP}/Includ")
~
