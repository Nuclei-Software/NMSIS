/* ----------------------------------------------------------------------
* Copyright (C) 2010-2018 Arm Limited. All rights reserved.
* Copyright (c) 2019 Nuclei Limited. All rights reserved.
*
*
* Project:       NMSIS NN Library
* Title:         riscv_nnexamples_nn_test.cpp
*
* Description:   Example code for NN kernel testing.
*
* Target Processor: RISC-V Cores
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of ARM LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */
#include <stdio.h>

#define Activation
#define BasicMath
#define Concatenation
#define Convolution
#define DepthwiseConvolution
#define Fully_connectedLayer
#define Pooling
#define Lstm
#define Softmax
#define SVD
#define NNDataConversion
#define BasicMathforNN

int main()
{
    printf("Start nn benchmark\n");

#ifdef Convolution
    conv_1_x_n_1_riscv_convolve_s8();
    kernel1x1_riscv_convolve_1x1_s4_fast();
    kernel1x1_riscv_convolve_1x1_s8_fast();
    int16xint8_riscv_convolve_fast_s16();
    basic_riscv_convolve_s4();
    basic_riscv_convolve_s8();
    int16xint8_riscv_convolve_s16();
    transpose_conv_1_riscv_transpose_conv_s8();
#endif

#ifdef BasicMath
    add_riscv_elementwise_add_s8();
    add_s16_riscv_elementwise_add_s16();
    mul_riscv_elementwise_mul_s8();
    mul_s16_riscv_elementwise_mul_s16();
#endif

#ifdef DepthwiseConvolution
    depthwise_kernel_3x3_riscv_depthwise_conv_3x3_s8();
    dw_int16xint8_fast_riscv_depthwise_conv_fast_s16();
    depthwise_int4_generic_riscv_depthwise_conv_s4();
    depthwise_int4_1_riscv_depthwise_conv_s4_opt();
    depthwise_2_riscv_depthwise_conv_s8();
    basic_riscv_depthwise_conv_s8_opt();
    dw_int16xint8_riscv_depthwise_conv_s16();
#endif

#ifdef Pooling
    avgpooling_riscv_avgpool_s8();
    avgpooling_int16_riscv_avgpool_s16();
    maxpooling_riscv_max_pool_s8();
    maxpool_int16_riscv_max_pool_s16();
#endif

#ifdef Fully_connectedLayer
    fully_connected_int4_riscv_fully_connected_s4();
    fully_connected_riscv_fully_connected_s8();
    fully_connected_int16_riscv_fully_connected_s16();
#endif

#ifdef Lstm
    lstm_1_riscv_lstm_unidirectional_s16_s8();
#endif

#ifdef Softmax
    softmax_riscv_softmax_s8();
    softmax_s8_s16_riscv_softmax_s8_s16();
    softmax_s16_riscv_softmax_s16();
#endif

#ifdef SVD
    svdf_int8_riscv_svdf_s8();
    svdf_riscv_svdf_state_s16_s8();
#endif
    
    printf("Finish nn benchmark\n");
}
