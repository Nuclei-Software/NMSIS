cmake_minimum_required (VERSION 3.14)

project(NMSISNNConcatenation)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNConcatenation STATIC ${SRC})

### Includes
target_include_directories(NMSISNNConcatenation PUBLIC "${NN}/Include")
target_include_directories(NMSISNNConcatenation PUBLIC "${DSP}/Include")
