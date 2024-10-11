file(GLOB SRC_S4 "./*_s4.c")
file(GLOB SRC_S8 "./*_s8.c")
file(GLOB SRC_S16 "./*_s16*.c")
file(GLOB SRC_S64 "./*_s64.c")

set(SRC_Q7Q15
riscv_fully_connected_mat_q7_vec_q15_opt.c
riscv_fully_connected_mat_q7_vec_q15.c
riscv_fully_connected_q7.c
riscv_fully_connected_q7_opt.c
riscv_fully_connected_q15_opt.c
riscv_fully_connected_q15.c
)

target_sources(NMSISNN PRIVATE ${SRC_S4} ${SRC_S8} ${SRC_S16} ${SRC_S64} ${SRC_Q7Q15})

