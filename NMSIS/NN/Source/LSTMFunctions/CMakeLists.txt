cmake_minimum_required (VERSION 3.14)

project(NMSISNNLSTM)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNLSTM STATIC ${SRC})

### Includes
target_include_directories(NMSISNNLSTM PUBLIC "${NN}/Include")
