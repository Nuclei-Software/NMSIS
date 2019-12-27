cmake_minimum_required (VERSION 3.5)

project(REF_NN_IMP)

file(GLOB SRC "./*_*.c")

add_library(REF_NN_IMP STATIC ${SRC})

### Includes
target_include_directories(REF_NN_IMP PUBLIC "{REF_NN}/Ref_Implementations")
target_include_directories(REF_NN_IMP PUBLIC "${NN}/Include")
target_include_directories(REF_NN_IMP PUBLIC "${DSP}/Include")
