file(GLOB SRC "./*_s8*.c")
file(GLOB SRC_S16 "./*_s16*.c")

set(SRC_Q7Q15 riscv_relu_q7.c
riscv_relu_q15.c
riscv_nn_activations_q7.c
riscv_nn_activations_q15.c
)

target_sources(NMSISNN PRIVATE ${SRC} ${SRC_S16} ${SRC_Q7Q15})
