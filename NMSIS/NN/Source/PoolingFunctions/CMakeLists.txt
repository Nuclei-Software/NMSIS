cmake_minimum_required (VERSION 3.14)

project(NMSISNNPooling)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNPooling STATIC ${SRC})

### Includes
target_include_directories(NMSISNNPooling PUBLIC "${NN}/Include")
