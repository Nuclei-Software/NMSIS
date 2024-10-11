file(GLOB SRC_S4 "./*_s4*.c")
file(GLOB SRC_S8 "./*_s8*.c")
file(GLOB SRC_S16 "./*_s16*.c")
set(SRC_Q7Q15
riscv_fully_connected_mat_q7_vec_q15_opt.c
riscv_fully_connected_mat_q7_vec_q15.c
riscv_fully_connected_q7.c
riscv_fully_connected_q7_opt.c
riscv_fully_connected_q15_opt.c
riscv_fully_connected_q15.c
)

set(SRC_Q7Q15
riscv_q7_to_q15_with_offset.c
riscv_nn_copy_q7.c
riscv_nn_fill_q7.c
riscv_q7_to_q15_reordered_no_shift.c
riscv_q7_to_q7_no_shift.c
riscv_q7_to_q7_reordered_no_shift.c
riscv_q7_to_q15_no_shift.c
riscv_q7_to_q15_reordered_with_offset.c
riscv_nn_mult_q7.c
riscv_nn_accumulate_q7_to_q15.c
riscv_nn_add_q7.c
riscv_nn_copy_q15.c
riscv_nn_fill_q15.c
riscv_nn_mult_q15.c
)

target_sources(NMSISNN PRIVATE ${SRC_S4} ${SRC_S8} ${SRC_S16} ${SRC_Q7Q15} riscv_nntables.c
  riscv_nn_mat_mult_nt_t_s8_s32.c
  riscv_s8_to_s16_unordered_with_offset.c)

