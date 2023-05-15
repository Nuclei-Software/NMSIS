cmake_minimum_required (VERSION 3.14)

project(NMSISNNFullyConnected)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNFullyConnected STATIC ${SRC})

### Includes
target_include_directories(NMSISNNFullyConnected PUBLIC "${NN}/Include")
