cmake_minimum_required (VERSION 3.14)

project(NMSISSVD)

file(GLOB SRC "./*_*.c")

add_library(NMSISSVD STATIC ${SRC})

### Includes
target_include_directories(NMSISSVD PUBLIC "${NN}/Include")
target_include_directories(NMSISSVD PUBLIC "${DSP}/Include")
