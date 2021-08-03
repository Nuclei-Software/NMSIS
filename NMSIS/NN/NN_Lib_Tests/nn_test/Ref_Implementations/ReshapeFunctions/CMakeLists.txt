cmake_minimum_required (VERSION 3.5)

project(NMSISNNReshape)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNReshape STATIC ${SRC})

### Includes
target_include_directories(NMSISNNReshape PUBLIC "${NN}/Include")
target_include_directories(NMSISNNReshape PUBLIC "${DSP}/Include")
