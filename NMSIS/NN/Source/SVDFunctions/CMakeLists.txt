cmake_minimum_required (VERSION 3.14)

project(NMSISNNSVDF)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNSVDF STATIC ${SRC})

### Includes
target_include_directories(NMSISNNSVDF PUBLIC "${NN}/Include")
