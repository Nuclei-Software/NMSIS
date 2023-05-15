cmake_minimum_required (VERSION 3.14)

project(NMSISNNReshape)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNReshape STATIC ${SRC})

### Includes
target_include_directories(NMSISNNReshape PUBLIC "${NN}/Include")
