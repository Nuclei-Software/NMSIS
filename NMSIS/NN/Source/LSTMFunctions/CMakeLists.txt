file(GLOB SRC_S8 "./*_s8.c")
file(GLOB SRC_S16 "./*_s16.c")
target_sources(NMSISNN PRIVATE ${SRC_S8} ${SRC_S16})

