file(GLOB SRC_S4 "./*_s4*.c")
file(GLOB SRC_S8 "./*_s8*.c")
file(GLOB SRC_S16 "./*_s16*.c")

set(SRC_Q7Q15
riscv_convolve_HWC_q7_RGB.c
riscv_convolve_HWC_q7_basic.c
riscv_convolve_HWC_q7_basic_nonsquare.c
riscv_convolve_HWC_q7_fast.c
riscv_convolve_HWC_q7_fast_nonsquare.c
riscv_nn_mat_mult_kernel_q7_q15_reordered.c
riscv_depthwise_separable_conv_HWC_q7.c
riscv_depthwise_separable_conv_HWC_q7_nonsquare.c
riscv_nn_mat_mult_kernel_q7.c
riscv_nn_mat_mult_kernel_q7_q15.c
riscv_nn_mat_mult_kernel_q7_reordered.c
riscv_convolve_1x1_HWC_q7_fast_nonsquare.c
riscv_convolve_HWC_q15_basic.c
riscv_convolve_HWC_q15_fast.c
riscv_convolve_HWC_q15_fast_nonsquare.c
)
target_sources(NMSISNN PRIVATE ${SRC_S4} ${SRC_S8} ${SRC_S16} ${SRC_Q7Q15})

