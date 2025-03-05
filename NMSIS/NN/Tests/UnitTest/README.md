# Unit Test

## Tested API

### Activation Functions

- [ ] riscv_nn_activation_s16
- [ ] riscv_relu6_s8
- [ ] riscv_relu_q15
- [ ] riscv_relu_q7

### Elementwise Functions

#### add

- [ ] riscv_elementwise_add_s16
- [ ] riscv_elementwise_add_s8

#### mul

- [ ] riscv_elementwise_mul_acc_s16
- [ ] riscv_elementwise_mul_s16
- [ ] riscv_elementwise_mul_s16_batch_offset
- [ ] riscv_elementwise_mul_s8

### MinimumMaximum
- [ ] riscv_maximum_s8
- [ ] riscv_minimum_s8

### Concatenation Functions

- [ ] riscv_concatenation_s8_w
- [ ] riscv_concatenation_s8_x
- [ ] riscv_concatenation_s8_y
- [ ] riscv_concatenation_s8_z

### Convolution Functions

#### normal

- [ ] riscv_convolve_1_x_n_s4_get_buffer_size
- [ ] riscv_convolve_1_x_n_s8_get_buffer_size
- [ ] riscv_convolve_1x1_s4_fast_get_buffer_size
- [ ] riscv_convolve_1x1_s8_fast_get_buffer_size
- [ ] riscv_convolve_s16_get_buffer_size
- [ ] riscv_convolve_s4_get_buffer_size
- [ ] riscv_convolve_s8_get_buffer_size
- [ ] riscv_convolve_wrapper_s16_get_buffer_size
- [ ] riscv_convolve_wrapper_s16_get_buffer_size_dsp
- [ ] riscv_convolve_wrapper_s4_get_buffer_size
- [ ] riscv_convolve_wrapper_s4_get_buffer_size_dsp
- [ ] riscv_convolve_wrapper_s8_get_buffer_size
- [ ] riscv_convolve_wrapper_s8_get_buffer_size_dsp
- [ ] riscv_convolve_1_x_n_s4
- [ ] riscv_convolve_1_x_n_s8
- [ ] riscv_convolve_1x1_s4
- [ ] riscv_convolve_1x1_s4_fast
- [ ] riscv_convolve_1x1_s8
- [ ] riscv_convolve_1x1_s8_fast
- [ ] riscv_convolve_even_s4
- [ ] riscv_convolve_s16
- [ ] riscv_convolve_s4
- [ ] riscv_convolve_s8
- [ ] riscv_convolve_wrapper_s16
- [ ] riscv_convolve_wrapper_s4
- [ ] riscv_convolve_wrapper_s8

#### depthwise

- [ ] riscv_depthwise_conv_fast_s16_get_buffer_size
- [ ] riscv_depthwise_conv_s4_opt_get_buffer_size
- [ ] riscv_depthwise_conv_s8_opt_get_buffer_size
- [ ] riscv_depthwise_conv_s8_opt_get_buffer_size_dsp
- [ ] riscv_depthwise_conv_wrapper_s16_get_buffer_size
- [ ] riscv_depthwise_conv_wrapper_s16_get_buffer_size_dsp
- [ ] riscv_depthwise_conv_wrapper_s4_get_buffer_size
- [ ] riscv_depthwise_conv_wrapper_s4_get_buffer_size_dsp
- [ ] riscv_depthwise_conv_wrapper_s8_get_buffer_size
- [ ] riscv_depthwise_conv_wrapper_s8_get_buffer_size_dsp
- [ ] riscv_depthwise_conv_3x3_s8
- [ ] riscv_depthwise_conv_fast_s16
- [ ] riscv_depthwise_conv_s16
- [ ] riscv_depthwise_conv_s4
- [ ] riscv_depthwise_conv_s4_opt
- [ ] riscv_depthwise_conv_s8
- [ ] riscv_depthwise_conv_s8_opt
- [ ] riscv_depthwise_conv_wrapper_s16
- [ ] riscv_depthwise_conv_wrapper_s4
- [ ] riscv_depthwise_conv_wrapper_s8

#### transpose conv

- [ ] riscv_transpose_conv_s8_get_buffer_size
- [ ] riscv_transpose_conv_s8_get_reverse_conv_buffer_size
- [ ] riscv_transpose_conv_s8
- [ ] riscv_transpose_conv_wrapper_s8

### Fully-connected Layer Functions

- [ ] riscv_fully_connected_s16_get_buffer_size
- [ ] riscv_fully_connected_s16_get_buffer_size_dsp
- [x] riscv_fully_connected_s8_get_buffer_size
- [ ] riscv_fully_connected_s8_get_buffer_size_dsp
- [ ] riscv_batch_matmul_s16
- [x] riscv_batch_matmul_s8
- [ ] riscv_fully_connected_per_channel_s8
- [ ] riscv_fully_connected_s16
- [ ] riscv_fully_connected_s4
- [ ] riscv_fully_connected_s8
- [ ] riscv_fully_connected_wrapper_s8
- [ ] riscv_vector_sum_s8
- [ ] riscv_vector_sum_s8_s64

### LSTM Layer Functions
- [ ] riscv_lstm_unidirectional_s16
- [ ] riscv_lstm_unidirectional_s8

### Pad Layer Functions:
- [ ] riscv_pad_s8

### Pooling Functions
- [x] riscv_avgpool_s8
- [x] riscv_avgpool_s8_get_buffer_size
- [x] riscv_avgpool_s8_get_buffer_size_dsp
- [x] riscv_avgpool_s16_get_buffer_size
- [x] riscv_avgpool_s16_get_buffer_size_dsp
- [x] riscv_avgpool_s16
- [x] riscv_max_pool_s8
- [x] riscv_max_pool_s16

### Reshape Functions
- [ ] riscv_reshape_s8

### Softmax Functions
- [ ] riscv_softmax_s16
- [ ] riscv_softmax_s8
- [ ] riscv_softmax_s8_s16
- [ ] riscv_softmax_u8

### SVDF Functions
- [ ] riscv_svdf_s8_get_buffer_size
- [ ] riscv_svdf_s8_get_buffer_size_dsp
- [ ] riscv_svdf_s8
- [ ] riscv_svdf_state_s16_s8

### Transpose Functions
- [ ] riscv_transpose_s8
