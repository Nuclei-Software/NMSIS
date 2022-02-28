cmake_minimum_required (VERSION 3.5)

project(NMSISNNSupport)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNSupport STATIC ${SRC})

### Includes
target_include_directories(NMSISNNSupport PUBLIC "${NN}/Include")
target_include_directories(NMSISNNSupport PUBLIC "${DSP}/Include")
