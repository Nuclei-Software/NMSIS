cmake_minimum_required (VERSION 3.5)

project(NMSISNNConvolutions)

file(GLOB SRC "./*_*.c")

add_library(NMSISNNConvolutions STATIC ${SRC})

### Includes
target_include_directories(NMSISNNConvolutions PUBLIC "${NN}/Include")
target_include_directories(NMSISNNConvolutions PUBLIC "${DSP}/Include")
