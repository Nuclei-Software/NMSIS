cmake_minimum_required (VERSION 3.5)

project(NMSISNNBasicMaths)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNBasicMaths STATIC ${SRC})

### Includes
target_include_directories(NMSISNNBasicMaths PUBLIC "${NN}/Include")
target_include_directories(NMSISNNBasicMaths PUBLIC "${DSP}/Include")
