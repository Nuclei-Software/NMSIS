file(GLOB SRC "./*_s8.c")
file(GLOB SRC_S16 "./*_s16.c")
set(SRC_Q7Q15
riscv_pool_q7_HWC.c
)
target_sources(NMSISNN PRIVATE ${SRC} ${SRC_S16} ${SRC_Q7Q15})

