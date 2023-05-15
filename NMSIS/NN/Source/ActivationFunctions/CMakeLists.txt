cmake_minimum_required (VERSION 3.14)

project(NMSISNNActivation)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNActivation STATIC ${SRC})

### Includes
target_include_directories(NMSISNNActivation PUBLIC "${NN}/Include")
