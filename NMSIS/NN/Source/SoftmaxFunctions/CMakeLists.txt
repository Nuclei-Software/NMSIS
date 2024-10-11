
file(GLOB SRC "./*_s8.c")
set(SRC_Q7Q15_U8
riscv_softmax_q7.c
riscv_softmax_with_batch_q7.c
riscv_softmax_q15.c
riscv_softmax_u8.c
)
target_sources(NMSISNN PRIVATE ${SRC} ${SRC_Q7Q15_U8} riscv_softmax_s8_s16.c
                                                   riscv_softmax_s16.c
                                                   riscv_nn_softmax_common_s8.c)

