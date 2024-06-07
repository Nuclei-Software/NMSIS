extern void conv_1_x_n_1_riscv_convolve_s8();
extern void kernel1x1_riscv_convolve_1x1_s4_fast();
extern void kernel1x1_riscv_convolve_1x1_s8_fast();
extern void int16xint8_riscv_convolve_fast_s16();
extern void basic_riscv_convolve_s4();
extern void basic_riscv_convolve_s8();
extern void int16xint8_riscv_convolve_s16();
extern void transpose_conv_1_riscv_transpose_conv_s8();

extern void add_riscv_elementwise_add_s8();
extern void add_s16_riscv_elementwise_add_s16();
extern void mul_riscv_elementwise_mul_s8();
extern void mul_s16_riscv_elementwise_mul_s16();

extern void depthwise_kernel_3x3_riscv_depthwise_conv_3x3_s8();
extern void dw_int16xint8_fast_riscv_depthwise_conv_fast_s16();
extern void depthwise_int4_generic_riscv_depthwise_conv_s4();
extern void depthwise_int4_1_riscv_depthwise_conv_s4_opt();
extern void depthwise_2_riscv_depthwise_conv_s8();
extern void basic_riscv_depthwise_conv_s8_opt();
extern void dw_int16xint8_riscv_depthwise_conv_s16();

extern void avgpooling_riscv_avgpool_s8();
extern void avgpooling_int16_riscv_avgpool_s16();
extern void maxpooling_riscv_max_pool_s8();
extern void maxpool_int16_riscv_max_pool_s16();

extern void fully_connected_int4_riscv_fully_connected_s4();
extern void fully_connected_riscv_fully_connected_s8();
extern void fully_connected_int16_riscv_fully_connected_s16();

extern void lstm_1_riscv_lstm_unidirectional_s16_s8();

extern void softmax_riscv_softmax_s8();
extern void softmax_s8_s16_riscv_softmax_s8_s16();
extern void softmax_s16_riscv_softmax_s16();

extern void svdf_int8_riscv_svdf_s8();
extern void svdf_riscv_svdf_state_s16_s8();
