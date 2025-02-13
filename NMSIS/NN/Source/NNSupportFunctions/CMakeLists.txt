file(GLOB SRC_S4 "./*_s4*.c")
file(GLOB SRC_S8 "./*_s8*.c")
file(GLOB SRC_S16 "./*_s16*.c")
file(GLOB SRC_S32 "./*_s32*.c")

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

target_sources(NMSISNN PRIVATE ${SRC_S4} ${SRC_S8} ${SRC_S16} ${SRC_S32} ${SRC_Q7Q15} riscv_nntables.c
  riscv_s8_to_s16_unordered_with_offset.c)
