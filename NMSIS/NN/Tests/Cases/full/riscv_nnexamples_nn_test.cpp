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

#include "riscv_nnexamples_nn_test.h"
#include "bench.h"

/** 
 * Here is the reference version of functions declared in riscv_nnsupportfunctions.h
 * To generate riscv_nnsupportfunctions_ref.h will incur compilation error,
 * so these functions are manually declared here.
*/
extern "C" {
riscv_nmsis_nn_status riscv_elementwise_mul_s16_batch_offset_ref(
    const int16_t *input_1_vect, const int16_t *input_2_vect, int16_t *output,
    const int32_t out_offset, const int32_t out_mult, const int32_t out_shift,
    const int32_t block_size, const int32_t batch_size,
    const int32_t batch_offset);
riscv_nmsis_nn_status riscv_elementwise_mul_acc_s16_ref(
    const int16_t *input_1_vect, const int16_t *input_2_vect,
    const int32_t input_1_offset, const int32_t input_2_offset, int16_t *output,
    const int32_t out_offset, const int32_t out_mult, const int32_t out_shift,
    const int32_t out_activation_min, const int32_t out_activation_max,
    const int32_t block_size);
}

#define TEST_Activation
#define TEST_BasicMath
#define TEST_Concatenation
#define TEST_Convolution
#define TEST_DepthwiseConvolution
#define TEST_TransposeConvolution
#define TEST_Fully_connectedLayer
#define TEST_Pooling
#define TEST_Lstm
#define TEST_Softmax
#define TEST_SVD
// #define TEST_NNDataConversion
// #define TEST_BasicMathforNN
// #define TEST_Reshape

int test_index = 0;
q7_t test_flags[100];
bool test_pass;

BENCH_DECLARE_VAR();
int main()
{
    printf("start tests\n");

    unsigned long randvar = __RV_CSR_READ(CSR_MCYCLE);
    srand(randvar);
    printf("srandvar is %d\n", randvar);

    // common pointers for testing data
    q7_t *test1;
    q15_t *test2;

    BENCH_INIT();

    for (test_index = 0; test_index < 100; test_index++) {
        test_flags[test_index] = -1;
    }
    test_index = 0;

    #define MAX_DATA_SIZE 10000
    test1 = new q7_t[MAX_DATA_SIZE];
    test2 = new q15_t[MAX_DATA_SIZE];

    q7_t *output_q7 = new q7_t[MAX_DATA_SIZE];
    q15_t *output_q15 = new q15_t[MAX_DATA_SIZE];

    for (int i = 0; i < MAX_DATA_SIZE; i++) {
        test1[i] = (rand() % 256 - 128);
        test2[i] = (rand() % 65536 - 32768);
        output_q7[i] = 0;
    }

#ifdef TEST_Activation
    /**
     * public functions:
     * riscv_nn_activation_s16
     * riscv_relu6_s8
     * riscv_relu_q15
     * riscv_relu_q7
     */
    #define Activation_SIZE 128
    uint16_t int_width = rand() % 3;
    q7_t *test1_ref = new q7_t[Activation_SIZE];
    q7_t *test1_opt = new q7_t[Activation_SIZE];
    q15_t *test2_ref = new q15_t[Activation_SIZE];
    q15_t *test2_opt = new q15_t[Activation_SIZE];
    for (int i = 0; i < Activation_SIZE; i++)
    {
        test1_ref[i] = test1[i];
        test1_opt[i] = test1[i];
        test2_ref[i] = test2[i];
        test2_opt[i] = test2[i];
    }
    printf("\r\nStart ActivationFunctions tests\r\n");
    riscv_nn_activations_direct_q7_ref(test1_ref, Activation_SIZE, int_width, RISCV_SIGMOID);
    BENCH_START(riscv_nn_activations_direct_q7);
    riscv_nn_activations_direct_q7(test1_opt, Activation_SIZE, int_width, RISCV_SIGMOID);
    BENCH_END(riscv_nn_activations_direct_q7);
    verify_results_q7(test1_ref, test1_opt, Activation_SIZE);

    riscv_nn_activations_direct_q15_ref(test2_ref, Activation_SIZE, int_width, RISCV_SIGMOID);
    BENCH_START(riscv_nn_activations_direct_q15);
    riscv_nn_activations_direct_q15(test2_opt, Activation_SIZE, int_width, RISCV_SIGMOID);
    BENCH_END(riscv_nn_activations_direct_q15);
    verify_results_q15(test2_ref, test2_opt, Activation_SIZE);

    memcpy(test1_opt, test1_ref, Activation_SIZE);
    q7_t *relu_ref_data_q7 = test1_ref;
    q7_t *relu_opt_data_q7 = test1_opt;
    q15_t *relu_ref_data_q15 = test2_ref;
    q15_t *relu_opt_data_q15 = test2_opt;

    riscv_relu_q7_ref(relu_ref_data_q7, Activation_SIZE);
    BENCH_START(riscv_relu_q7);
    riscv_relu_q7(relu_opt_data_q7, Activation_SIZE);
    BENCH_END(riscv_relu_q7);
    verify_results_q7(relu_ref_data_q7, relu_opt_data_q7, Activation_SIZE);

    riscv_relu_q15_ref(relu_ref_data_q15, Activation_SIZE);
    BENCH_START(riscv_relu_q15);
    riscv_relu_q15(relu_opt_data_q15, Activation_SIZE);
    BENCH_END(riscv_relu_q15);
    verify_results_q15(relu_ref_data_q15, relu_opt_data_q15, Activation_SIZE);

    riscv_relu6_s8_ref(relu_ref_data_q7, Activation_SIZE);
    BENCH_START(riscv_relu6_s8);
    riscv_relu6_s8(relu_opt_data_q7, Activation_SIZE);
    BENCH_END(riscv_relu6_s8);
    verify_results_q7(relu_ref_data_q7, relu_opt_data_q7, Activation_SIZE);

    riscv_nn_activation_s16_ref(test2_ref, test2_ref, Activation_SIZE, 0, RISCV_SIGMOID);
    BENCH_START(riscv_nn_activation_s16);
    riscv_nn_activation_s16(test2_opt, test2_opt, Activation_SIZE, 0, RISCV_SIGMOID);
    BENCH_END(riscv_nn_activation_s16);
    verify_results_q15(test2_ref, test2_opt, Activation_SIZE);

    riscv_nn_activation_s16_ref(test2_ref, test2_ref, Activation_SIZE, 0, RISCV_TANH);
    BENCH_START(riscv_nn_activation_s16);
    riscv_nn_activation_s16(test2_opt, test2_opt, Activation_SIZE, 0, RISCV_TANH);
    BENCH_END(riscv_nn_activation_s16);
    verify_results_q15(test2_ref, test2_opt, Activation_SIZE);

    delete[]test1_ref;
    delete[]test1_opt;
    delete[]test2_ref;
    delete[]test2_opt;
#endif

#ifdef TEST_BasicMath
    /**
     * public functions:
     * riscv_elementwise_add_s16
     * riscv_elementwise_add_s8
     * riscv_elementwise_mul_acc_s16
     * riscv_elementwise_mul_s16
     * riscv_elementwise_mul_s16_batch_offset
     * riscv_elementwise_mul_s8
     */
    #define BasicMath_SIZE 500

    #define ADD_DST_SIZE 128
    #define ADD_OUT_ACTIVATION_MIN -128
    #define ADD_OUT_ACTIVATION_MAX 127
    #define ADD_INPUT1_OFFSET 128
    #define ADD_INPUT2_OFFSET 128
    #define ADD_OUTPUT_MULT 1073741824
    #define ADD_OUTPUT_SHIFT -19
    #define ADD_OUTPUT_OFFSET -128
    #define ADD_LEFT_SHIFT 20
    #define ADD_INPUT1_SHIFT 0
    #define ADD_INPUT2_SHIFT 0
    #define ADD_INPUT1_MULT 1073741824
    #define ADD_INPUT2_MULT 1073741824

    printf("\r\nStart BasicMathFunctions tests\r\n");
    riscv_elementwise_add_s8_ref(test1, test1 + BasicMath_SIZE, ADD_INPUT1_OFFSET, ADD_INPUT1_MULT, ADD_INPUT1_SHIFT,
                                                                ADD_INPUT2_OFFSET, ADD_INPUT2_MULT, ADD_INPUT2_SHIFT, ADD_LEFT_SHIFT,
                                                                output_q7,
                                                                ADD_OUTPUT_OFFSET, ADD_OUTPUT_MULT,ADD_OUTPUT_SHIFT,
                                                                ADD_OUT_ACTIVATION_MIN, ADD_OUT_ACTIVATION_MAX, BasicMath_SIZE);

    BENCH_START(riscv_elementwise_add_s8);
    riscv_elementwise_add_s8(test1, test1 + BasicMath_SIZE, ADD_INPUT1_OFFSET, ADD_INPUT1_MULT, ADD_INPUT1_SHIFT,
                                                                ADD_INPUT2_OFFSET, ADD_INPUT2_MULT, ADD_INPUT2_SHIFT, ADD_LEFT_SHIFT,
                                                                output_q7 + BasicMath_SIZE,
                                                                ADD_OUTPUT_OFFSET, ADD_OUTPUT_MULT,ADD_OUTPUT_SHIFT,
                                                                ADD_OUT_ACTIVATION_MIN, ADD_OUT_ACTIVATION_MAX, BasicMath_SIZE);
    BENCH_END(riscv_elementwise_add_s8);

    verify_results_q7(output_q7, output_q7 + BasicMath_SIZE, BasicMath_SIZE);

    #define MUL_OUT_ACTIVATION_MIN -128
    #define MUL_OUT_ACTIVATION_MAX 127
    #define MUL_INPUT1_OFFSET 128
    #define MUL_INPUT2_OFFSET 128
    #define MUL_OUTPUT_MULT 1077952640
    #define MUL_OUTPUT_SHIFT -7
    #define MUL_OUTPUT_OFFSET -128

    riscv_elementwise_mul_s8_ref(test1, test1 + BasicMath_SIZE, MUL_INPUT1_OFFSET, MUL_INPUT2_OFFSET, output_q7,
                                                                MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
                                                                MUL_OUT_ACTIVATION_MIN, MUL_OUT_ACTIVATION_MAX, BasicMath_SIZE);

    BENCH_START(riscv_elementwise_mul_s8);
    riscv_elementwise_mul_s8(test1, test1 + BasicMath_SIZE, MUL_INPUT1_OFFSET, MUL_INPUT2_OFFSET, output_q7 + BasicMath_SIZE,
                                                                MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
                                                                MUL_OUT_ACTIVATION_MIN, MUL_OUT_ACTIVATION_MAX, BasicMath_SIZE);
    BENCH_END(riscv_elementwise_mul_s8);
    verify_results_q7(output_q7, output_q7 + BasicMath_SIZE, BasicMath_SIZE);

    memcpy(output_q15 + BasicMath_SIZE, output_q15, BasicMath_SIZE * sizeof(int16_t));
    riscv_elementwise_add_s16_ref(
        test2, test2 + BasicMath_SIZE, ADD_INPUT1_OFFSET, ADD_INPUT1_MULT,
        ADD_INPUT1_SHIFT, ADD_INPUT2_OFFSET, ADD_INPUT2_MULT, ADD_INPUT2_SHIFT,
        ADD_LEFT_SHIFT, output_q15, ADD_OUTPUT_OFFSET, ADD_OUTPUT_MULT,
        ADD_OUTPUT_SHIFT, ADD_OUT_ACTIVATION_MIN, ADD_OUT_ACTIVATION_MAX,
        BasicMath_SIZE);

    BENCH_START(riscv_elementwise_add_s16);
    riscv_elementwise_add_s16(
        test2, test2 + BasicMath_SIZE, ADD_INPUT1_OFFSET, ADD_INPUT1_MULT,
        ADD_INPUT1_SHIFT, ADD_INPUT2_OFFSET, ADD_INPUT2_MULT, ADD_INPUT2_SHIFT,
        ADD_LEFT_SHIFT, output_q15 + BasicMath_SIZE, ADD_OUTPUT_OFFSET,
        ADD_OUTPUT_MULT, ADD_OUTPUT_SHIFT, ADD_OUT_ACTIVATION_MIN,
        ADD_OUT_ACTIVATION_MAX, BasicMath_SIZE);
    BENCH_END(riscv_elementwise_add_s16);

    verify_results_q15(output_q15, output_q15 + BasicMath_SIZE, BasicMath_SIZE);

    riscv_elementwise_mul_s16_ref(
        test2, test2 + BasicMath_SIZE, MUL_INPUT1_OFFSET, MUL_INPUT2_OFFSET,
        output_q15, MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
        MUL_OUT_ACTIVATION_MIN, MUL_OUT_ACTIVATION_MAX, BasicMath_SIZE);

    BENCH_START(riscv_elementwise_mul_s16);
    riscv_elementwise_mul_s16(
        test2, test2 + BasicMath_SIZE, MUL_INPUT1_OFFSET, MUL_INPUT2_OFFSET,
        output_q15 + BasicMath_SIZE, MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
        MUL_OUT_ACTIVATION_MIN, MUL_OUT_ACTIVATION_MAX, BasicMath_SIZE);
    BENCH_END(riscv_elementwise_mul_s16);

    verify_results_q15(output_q15, output_q15 + BasicMath_SIZE, BasicMath_SIZE);

    riscv_elementwise_mul_acc_s16_ref(
        test2, test2 + BasicMath_SIZE, MUL_INPUT1_OFFSET, MUL_INPUT2_OFFSET,
        output_q15, MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
        MUL_OUT_ACTIVATION_MIN, MUL_OUT_ACTIVATION_MAX, BasicMath_SIZE);

    BENCH_START(riscv_elementwise_mul_acc_s16);
    riscv_elementwise_mul_acc_s16(
        test2, test2 + BasicMath_SIZE, MUL_INPUT1_OFFSET, MUL_INPUT2_OFFSET,
        output_q15 + BasicMath_SIZE, MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
        MUL_OUT_ACTIVATION_MIN, MUL_OUT_ACTIVATION_MAX, BasicMath_SIZE);
    BENCH_END(riscv_elementwise_mul_acc_s16);

    verify_results_q15(output_q15, output_q15 + BasicMath_SIZE, BasicMath_SIZE);

    #define BATCH_SIZE 10
    #define BLOCK_SIZE 32
    #define BATCH_OFFSET 1

    riscv_elementwise_mul_s16_batch_offset_ref(test2, test2 + BasicMath_SIZE, output_q15,
                                                                    MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
                                                                    BLOCK_SIZE, BATCH_SIZE, BATCH_OFFSET);

    BENCH_START(riscv_elementwise_mul_s16_batch_offset);
    riscv_elementwise_mul_s16_batch_offset(test2, test2 + BasicMath_SIZE, output_q15 + BasicMath_SIZE,
                                                                    MUL_OUTPUT_OFFSET, MUL_OUTPUT_MULT, MUL_OUTPUT_SHIFT,
                                                                    BLOCK_SIZE, BATCH_SIZE, BATCH_OFFSET);
    BENCH_END(riscv_elementwise_mul_s16_batch_offset);

    verify_results_q15(output_q15, output_q15 + BasicMath_SIZE, BLOCK_SIZE * BATCH_SIZE);

    /**
     * public functions:
     * riscv_maximum_s8
     * riscv_minimum_s8
     */
    {
    nmsis_nn_context ctx;
    nmsis_nn_dims in_out_dims = {.n = 2, .h = 3, .w = 4, .c = 5};
    const long inout_size = in_out_dims.n * in_out_dims.h * in_out_dims.w * in_out_dims.c;

    riscv_maximum_s8_ref(&ctx, test1, &in_out_dims, test1, &in_out_dims, output_q7, &in_out_dims);
    BENCH_START(riscv_maximum_s8);
    riscv_maximum_s8(&ctx, test1, &in_out_dims, test1, &in_out_dims, output_q7 + inout_size, &in_out_dims);
    BENCH_END(riscv_maximum_s8);
    verify_results_q7(output_q7, output_q7 + inout_size, inout_size);

    riscv_minimum_s8_ref(&ctx, test1, &in_out_dims, test1, &in_out_dims, output_q7, &in_out_dims);
    BENCH_START(riscv_minimum_s8);
    riscv_minimum_s8(&ctx, test1, &in_out_dims, test1, &in_out_dims, output_q7 + inout_size, &in_out_dims);
    BENCH_END(riscv_minimum_s8);
    verify_results_q7(output_q7, output_q7 + inout_size, inout_size);
    }

#endif

#ifdef TEST_Concatenation
    #define Concatenation_SIZE 80

    printf("\r\nStart ConcatenationFunctions tests\r\n");

    riscv_concatenation_s8_w_ref(test1, 10, 2, 2, 2, output_q7, 0);
    BENCH_START(riscv_concatenation_s8_w);
    riscv_concatenation_s8_w(test1, 10, 2, 2, 2, output_q7 + Concatenation_SIZE, 0);
    BENCH_END(riscv_concatenation_s8_w);
    verify_results_q7(output_q7, output_q7 + Concatenation_SIZE, Concatenation_SIZE);

    riscv_concatenation_s8_x_ref(test1, 10, 2, 2, 2, output_q7, 10, 0);
    BENCH_START(riscv_concatenation_s8_x);
    riscv_concatenation_s8_x(test1, 10, 2, 2, 2, output_q7 + Concatenation_SIZE, 10, 0);
    BENCH_END(riscv_concatenation_s8_x);
    verify_results_q7(output_q7, output_q7 + Concatenation_SIZE, Concatenation_SIZE);

    riscv_concatenation_s8_y_ref(test1, 10, 2, 2, 2, output_q7, 2, 0);
    BENCH_START(riscv_concatenation_s8_y);
    riscv_concatenation_s8_y(test1, 10, 2, 2, 2, output_q7 + Concatenation_SIZE, 2, 0);
    BENCH_END(riscv_concatenation_s8_y);
    verify_results_q7(output_q7, output_q7 + Concatenation_SIZE, Concatenation_SIZE);

    riscv_concatenation_s8_z_ref(test1, 10, 2, 2, 2, output_q7, 2, 0);
    BENCH_START(riscv_concatenation_s8_z);
    riscv_concatenation_s8_z(test1, 10, 2, 2, 2, output_q7 + Concatenation_SIZE, 2, 0);
    BENCH_END(riscv_concatenation_s8_z);
    verify_results_q7(output_q7, output_q7 + Concatenation_SIZE, Concatenation_SIZE);
#endif

#ifdef TEST_Convolution
    /**
     * public functions:
     * riscv_convolve_1_x_n_s4
     * riscv_convolve_1_x_n_s8
     * riscv_convolve_1x1_s4
     * riscv_convolve_1x1_s4_fast
     * riscv_convolve_1x1_s8
     * riscv_convolve_1x1_s8_fast
     * riscv_convolve_even_s4
     * riscv_convolve_s16
     * riscv_convolve_s4
     * riscv_convolve_s8
     * riscv_convolve_wrapper_s16
     * riscv_convolve_wrapper_s4
     * riscv_convolve_wrapper_s8
     */
    printf("\r\nStart ConvolutionFunctions part1 tests\r\n");
    #define Convolution_SIZE 2048
    q15_t *temp_buffer = new q15_t[Convolution_SIZE];
    int32_t multiplier[4] = {187431000, 141317000, 117516000, 151730000};
    int32_t shift[4] = {-10, -8, -7, -8};
    int32_t *bias_data = new int32_t[Convolution_SIZE];
    for (int i = 0; i < Convolution_SIZE; i++) {
        bias_data[i] = (rand() % 256 - 128);
    }

    nmsis_nn_context ctx = {temp_buffer, Convolution_SIZE};
    nmsis_nn_tile stride = {1, 1}, padding = {0, 0}, dilation = {1, 1};
    nmsis_nn_activation activation = {-12800, 12700};
    nmsis_nn_conv_params conv_params = {128, -128, stride, padding, dilation, activation};
    nmsis_nn_per_channel_quant_params quant_params = {multiplier, shift};
    nmsis_nn_dims input_dims = {1, 1, 8, 16};
    nmsis_nn_dims filter_dims = {4, 1, 2, 16};
    nmsis_nn_dims bias_dims = {4, 1, 2, 4};
    nmsis_nn_dims output_dims = {1, 8, 8, 4};

    riscv_convolve_1_x_n_s8_ref(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims, test1 + Convolution_SIZE,
                                &bias_dims, bias_data, &output_dims, output_q7);

    BENCH_START(riscv_convolve_1_x_n_s8);
    riscv_convolve_1_x_n_s8(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims, test1 + Convolution_SIZE,
                                &bias_dims, bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_1_x_n_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    riscv_convolve_1_x_n_s4_ref(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims, test1 + Convolution_SIZE,
                                &bias_dims, bias_data, &output_dims, output_q7);

    BENCH_START(riscv_convolve_1_x_n_s4);
    riscv_convolve_1_x_n_s4(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims, test1 + Convolution_SIZE,
                                &bias_dims, bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_1_x_n_s4);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    q15_t * bufferA = new q15_t[Convolution_SIZE];
    q7_t * bufferB = new q7_t[Convolution_SIZE];

    riscv_convolve_1x1_HWC_q7_fast_nonsquare_ref(test1, 1, 1, 40, test1 + Convolution_SIZE, 40, 1, 1, 0, 0, 1, 1,
                                                 test1 + Convolution_SIZE * 2, 0, 8, output_q7, 2, 2, bufferA, bufferB);

    BENCH_START(riscv_convolve_1x1_HWC_q7_fast_nonsquare);
    riscv_convolve_1x1_HWC_q7_fast_nonsquare(test1, 1, 1, 40, test1 + Convolution_SIZE, 40, 1, 1, 0, 0, 1, 1,
                                             test1 + Convolution_SIZE * 2, 0, 8, output_q7 + Convolution_SIZE, 2, 2, bufferA, bufferB);
    BENCH_END(riscv_convolve_1x1_HWC_q7_fast_nonsquare);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 40);
	nmsis_nn_dims input_dims1 = {1, 8, 8, 16};
    riscv_convolve_s8_ref(&ctx, &conv_params, &quant_params, &input_dims1, test1,
                          &filter_dims, test1 + Convolution_SIZE, &bias_dims, bias_data, NULL, &output_dims, output_q7);

    BENCH_START(riscv_convolve_s8);
    riscv_convolve_s8(&ctx, &conv_params, &quant_params, &input_dims1, test1,
                          &filter_dims, test1 + Convolution_SIZE, &bias_dims, bias_data,NULL,  &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    nmsis_nn_bias_data bias_data_s16 = {.data = bias_data,
                                        .is_int32_bias = true};
    riscv_convolve_s16_ref(&ctx, &conv_params, &quant_params, &input_dims,
                           test2, &filter_dims, test1 + Convolution_SIZE,
                           &bias_dims, &bias_data_s16, &output_dims,
                           output_q15);
    BENCH_START(riscv_convolve_s16);
    riscv_convolve_s16(&ctx, &conv_params, &quant_params, &input_dims, test2,
                       &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                       &bias_data_s16, &output_dims,
                       output_q15 + Convolution_SIZE);
    BENCH_END(riscv_convolve_s16);
    verify_results_q15(output_q15, output_q15 + Convolution_SIZE, 4 * 8 * 8);

    riscv_convolve_wrapper_s8_ref(&ctx, &conv_params, &quant_params,
                                  &input_dims, test1, &filter_dims,
                                  test1 + Convolution_SIZE, &bias_dims,
                                  bias_data, &output_dims, output_q7);
    BENCH_START(riscv_convolve_wrapper_s8);
    riscv_convolve_wrapper_s8(&ctx, &conv_params, &quant_params, &input_dims,
                              test1, &filter_dims, test1 + Convolution_SIZE,
                              &bias_dims, bias_data, &output_dims,
                              output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_wrapper_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    riscv_convolve_wrapper_s16_ref(&ctx, &conv_params, &quant_params,
                                   &input_dims, test2, &filter_dims,
                                   test1 + Convolution_SIZE, &bias_dims,
                                   &bias_data_s16, &output_dims, output_q15);
    BENCH_START(riscv_convolve_wrapper_s16);
    riscv_convolve_wrapper_s16(&ctx, &conv_params, &quant_params, &input_dims,
                               test2, &filter_dims, test1 + Convolution_SIZE,
                               &bias_dims, &bias_data_s16, &output_dims,
                               output_q15 + Convolution_SIZE);
    BENCH_END(riscv_convolve_wrapper_s16);
    verify_results_q15(output_q15, output_q15 + Convolution_SIZE, 4 * 8 * 8);

    uint8_t *test10 = new uint8_t[Convolution_SIZE];

    for (int i=0;i < Convolution_SIZE; i++) {
        test10[i] = (rand() % 256 - 128);
    }

    #define KERNEL1X1_OUT_CH 9

    int32_t kernel1x1_output_mult[KERNEL1X1_OUT_CH] = {1874310000, 1413170000, 1175160000, 1234670000, 1517300000, 1175160000, 1368550000,
                                                       1532180000, 1680930000};
    int32_t kernel1x1_output_shift[KERNEL1X1_OUT_CH] = {-10, -8, -9, -7, -7, -8, -8, -5, -7};
    const int32_t kernel1x1_biases[KERNEL1X1_OUT_CH] = {63742, -25567, 15168, -36287, 7303, 46323, 5280, 25782, 16909};

    input_dims.w = 15;
    input_dims.h = 15;
    input_dims.c = 4;
    filter_dims.w = 1;
    filter_dims.h = 1;
    output_dims.w = 15;
    output_dims.h = 15;
    output_dims.c = KERNEL1X1_OUT_CH;

    quant_params.multiplier = (int32_t *)kernel1x1_output_mult;
    quant_params.shift = (int32_t *)kernel1x1_output_shift;

    riscv_convolve_1x1_s8_fast_ref(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7);
    BENCH_START(riscv_convolve_1x1_s8_fast);
    riscv_convolve_1x1_s8_fast(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_1x1_s8_fast);

    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, KERNEL1X1_OUT_CH * 15 * 15);

    input_dims = {2, 4, 7, 9};
    filter_dims = {5, 1, 1, 9};
    output_dims = {2, 4, 3, 5};
    bias_dims.n = 1;
    bias_dims.h = 1;
    bias_dims.w = 1;
    bias_dims.c = output_dims.c;
    bias_dims = {1, 1, 1, 5};

    conv_params.padding.w = 0;
    conv_params.padding.h = 0;
    conv_params.stride.w = 3;
    conv_params.stride.h = 1;
    conv_params.dilation.w = 1;
    conv_params.dilation.h = 1;

    conv_params.input_offset = 0;
    conv_params.output_offset = 0;
    conv_params.activation.min = -126;
    conv_params.activation.max = 127;

    int32_t kernel1x1_stride_x_int4_biases[5] = {48, 116, -50, 127, -86};

    int32_t *bias_data1 = kernel1x1_stride_x_int4_biases;

    const int32_t kernel1x1_stride_x_int4_output_mult[5] = {1701096520, 1129972617, 1101170489, 1516451923, 2008360488};
    const int32_t kernel1x1_stride_x_int4_output_shift[5] = {-3, -3, -4, -3, -6};
    quant_params.multiplier = (int32_t *)kernel1x1_stride_x_int4_output_mult;
    quant_params.shift = (int32_t *)kernel1x1_stride_x_int4_output_shift;

    riscv_convolve_s4_ref(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims,
                        test1 + Convolution_SIZE, &bias_dims, bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_convolve_s4);
    riscv_convolve_s4(&ctx, &conv_params, &quant_params, &input_dims, test1, &filter_dims,
                    test1 + Convolution_SIZE, &bias_dims, bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_s4);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 120);

    riscv_convolve_1x1_s4_fast_ref(&ctx, &conv_params, &quant_params, &input_dims, test1,
                            &filter_dims, test1 + Convolution_SIZE,
                            &bias_dims, bias_data1, &output_dims, output_q7);   // kernel1x1_stride_x_int4_weights
    BENCH_START(riscv_convolve_1x1_s4_fast);
    riscv_convolve_1x1_s4_fast(&ctx, &conv_params, &quant_params, &input_dims, test1,
                            &filter_dims, test1 + Convolution_SIZE,
                            &bias_dims, bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_1x1_s4_fast);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 120);

    riscv_convolve_1x1_s4_ref(&ctx, &conv_params, &quant_params, &input_dims, test1,
                            &filter_dims, test1 + Convolution_SIZE,
                            &bias_dims, bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_convolve_1x1_s4);
    riscv_convolve_1x1_s4(&ctx, &conv_params, &quant_params, &input_dims, test1,
                        &filter_dims, test1 + Convolution_SIZE,
                        &bias_dims, bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_1x1_s4);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 120);

    riscv_convolve_wrapper_s4_ref(&ctx, &conv_params, &quant_params, &input_dims, test1,
                            &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                            bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_convolve_wrapper_s4);
    riscv_convolve_wrapper_s4(&ctx, &conv_params, &quant_params, &input_dims, test1,
                            &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                            bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_wrapper_s4);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 120);

    riscv_convolve_1x1_s8_ref(&ctx, &conv_params, &quant_params, &input_dims, test1,
        &filter_dims, test1 + Convolution_SIZE,
        &bias_dims, bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_convolve_1x1_s8);
    riscv_convolve_1x1_s8(&ctx, &conv_params, &quant_params, &input_dims, test1,
                        &filter_dims, test1 + Convolution_SIZE,
                        &bias_dims, bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_convolve_1x1_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 120);

    

    delete[] bias_data;
    delete[] temp_buffer;
    delete[] test10;
    delete[] bufferA;
    delete[] bufferB;
#endif

#ifdef TEST_DepthwiseConvolution
    {
    /**
     * public functions:
     * riscv_depthwise_conv_3x3_s8
     * riscv_depthwise_conv_fast_s16
     * riscv_depthwise_conv_s16
     * riscv_depthwise_conv_s4
     * riscv_depthwise_conv_s4_opt
     * riscv_depthwise_conv_s8
     * riscv_depthwise_conv_s8_opt
     * riscv_depthwise_conv_wrapper_s16
     * riscv_depthwise_conv_wrapper_s4
     * riscv_depthwise_conv_wrapper_s8
     */
    #define CONV_IM_DIM 8
    #define CONV_IM_CH 8
    #define CONV_KER_DIM 5
    #define CONV_OUT_CH 8
    #define CONV_OUT_DIM 8

    q7_t     *conv_weight_q7 = test1;
    q7_t     *conv_bias_q7 = test1 + CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;

    q15_t    *conv_weight_q15 = test2;
    q15_t    *conv_buf = test2 + CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;
    q15_t    *conv_bias_q15 = test2 + 2 * CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;

    q7_t     *conv_im_in_q7 = test1 + 2 * CONV_KER_DIM * CONV_KER_DIM * CONV_IM_CH * CONV_OUT_CH;
    q7_t     *conv_im_out_ref_q7 = output_q7;
    q7_t     *conv_im_out_opt_q7 = output_q7 + CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH;

    q15_t    *conv_im_in_q15 = test2;
    q15_t    *conv_im_out_ref_q15 = output_q15;
    q15_t    *conv_im_out_opt_q15 = output_q15 + CONV_IM_DIM * CONV_IM_DIM * CONV_IM_CH;

    initialize_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    printf("\r\nStart ConvolutionFunctions part2 tests\r\n");

    riscv_convolve_HWC_q7_basic_ref(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 9, conv_im_out_ref_q7,
                            CONV_OUT_DIM, conv_buf, NULL);

    BENCH_START(riscv_convolve_HWC_q7_basic);
    riscv_convolve_HWC_q7_basic(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                              CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 9, conv_im_out_opt_q7,
                              CONV_OUT_DIM, conv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q7_basic);
    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    riscv_convolve_HWC_q15_basic_ref(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 1, 9, conv_im_out_ref_q15,
                            CONV_OUT_DIM, conv_buf, NULL);
    BENCH_START(riscv_convolve_HWC_q15_basic);
    riscv_convolve_HWC_q15_basic(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                              CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 1, 9, conv_im_out_opt_q15,
                              CONV_OUT_DIM, conv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q15_basic);
    verify_results_q15(conv_im_out_ref_q15, conv_im_out_opt_q15, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    BENCH_START(riscv_convolve_HWC_q7_fast);
    riscv_convolve_HWC_q7_fast(conv_im_in_q7, CONV_IM_DIM, CONV_IM_CH, conv_weight_q7,
                             CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 9, conv_im_out_opt_q7,
                             CONV_OUT_DIM, conv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q7_fast);
    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    riscv_convolve_HWC_q15_fast_ref(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                             CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 1, 9, conv_im_out_ref_q15,
                             CONV_OUT_DIM, conv_buf, NULL);
    BENCH_START(riscv_convolve_HWC_q15_fast);
    riscv_convolve_HWC_q15_fast(conv_im_in_q15, CONV_IM_DIM, CONV_IM_CH, conv_weight_q15,
                             CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q15, 1, 9, conv_im_out_opt_q15,
                             CONV_OUT_DIM, conv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q15_fast);
    verify_results_q15(conv_im_out_ref_q15, conv_im_out_opt_q15, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);

    riscv_convolve_HWC_q7_basic_ref(conv_im_in_q7, CONV_IM_DIM, 3, conv_weight_q7,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 9, conv_im_out_ref_q7,
                            CONV_OUT_DIM, conv_buf, NULL);
    BENCH_START(riscv_convolve_HWC_q7_RGB);
    riscv_convolve_HWC_q7_RGB(conv_im_in_q7, CONV_IM_DIM, 3, conv_weight_q7,
                            CONV_OUT_CH, CONV_KER_DIM, 2, 1, conv_bias_q7, 1, 9, conv_im_out_opt_q7,
                            CONV_OUT_DIM, conv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q7_RGB);
    verify_results_q7(conv_im_out_ref_q7, conv_im_out_opt_q7, CONV_OUT_DIM * CONV_OUT_DIM * CONV_OUT_CH);
    
    #define Convolution_SIZE 2048
    q15_t *temp_buffer = new q15_t[Convolution_SIZE];
    int32_t multiplier[4] = {187431000, 141317000, 117516000, 151730000};
    int32_t shift[4] = {-10, -8, -7, -8};
    int32_t *bias_data = new int32_t[Convolution_SIZE];
    for (int i = 0; i < Convolution_SIZE; i++) {
        bias_data[i] = (rand() % 256 - 128);
    }

    nmsis_nn_context ctx = {temp_buffer, Convolution_SIZE};
    nmsis_nn_tile stride = {1, 1}, padding = {0, 0}, dilation = {1, 1};
    nmsis_nn_activation activation = {-12800, 12700};
    nmsis_nn_conv_params conv_params = {128, -128, stride, padding, dilation, activation};
    nmsis_nn_per_channel_quant_params quant_params = {multiplier, shift};
    nmsis_nn_dims input_dims = {1, 1, 8, 16};
    nmsis_nn_dims filter_dims = {4, 1, 2, 16};
    nmsis_nn_dims bias_dims = {4, 1, 2, 4};
    nmsis_nn_dims output_dims = {1, 8, 8, 4};

    nmsis_nn_dw_conv_params dw_conv_params = {0, 0, 1, stride, padding, dilation, activation};

    int32_t kernel1x1_stride_x_int4_biases[5] = {48, 116, -50, 127, -86};
    int32_t *bias_data1 = kernel1x1_stride_x_int4_biases;

    filter_dims.w = 3;
    filter_dims.h = 3;
    dw_conv_params.padding.w = 0;
    input_dims.c = 4;

    riscv_depthwise_conv_3x3_s8_ref(&ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
                                    test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7);

    BENCH_START(riscv_depthwise_conv_3x3_s8);
    riscv_depthwise_conv_3x3_s8(&ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
                                    test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_3x3_s8);

    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    dw_conv_params.ch_mult = 3;

    riscv_depthwise_conv_s8_ref(&ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
                                test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7);

    BENCH_START(riscv_depthwise_conv_s8);
    riscv_depthwise_conv_s8(&ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
                            test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_s8);

    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    dw_conv_params.ch_mult = 1;
    input_dims.n = 1;
    input_dims.w = 4;
    input_dims.h = 5;
    input_dims.c = 6;
    filter_dims.w = 2;
    filter_dims.h = 3;
    output_dims.w = 4;
    output_dims.h = 5;
    output_dims.c = 6;

    riscv_depthwise_conv_s8_opt_ref(&ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
                                    test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7);

    BENCH_START(riscv_depthwise_conv_s8_opt);
    riscv_depthwise_conv_s8_opt(&ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
                                    test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_s8_opt);

    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    riscv_depthwise_conv_wrapper_s8_ref(&ctx, &dw_conv_params, &quant_params,
                                        &input_dims, test1, &filter_dims,
                                        test1 + Convolution_SIZE, &bias_dims,
                                        bias_data, &output_dims, output_q7);
    BENCH_START(riscv_depthwise_conv_wrapper_s8);
    riscv_depthwise_conv_wrapper_s8(
        &ctx, &dw_conv_params, &quant_params, &input_dims, test1, &filter_dims,
        test1 + Convolution_SIZE, &bias_dims, bias_data, &output_dims,
        output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_wrapper_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 4 * 8 * 8);

    input_dims = {1, 16, 16, 2};
    filter_dims.w = 8;
    filter_dims.h = 8;
    output_dims.w = 5;
    output_dims.h = 5;
    output_dims.c = 8;
    dw_conv_params.ch_mult = 1;
    dw_conv_params.activation = {-127, 127};
    dw_conv_params = {0, 0, 1, stride, padding, dilation, activation};

    const int32_t depthwise_int4_generic_output_mult[8] =
    {1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358};
    const int32_t depthwise_int4_generic_output_shift[8] = {-3, -3, -3, -3, -3, -3, -3, -3};

    quant_params.shift = (int32_t *)depthwise_int4_generic_output_shift;
    quant_params.multiplier = (int32_t *)depthwise_int4_generic_output_mult;

    riscv_depthwise_conv_s4_ref(&ctx, &dw_conv_params, &quant_params, &input_dims,
                          test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                          bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_depthwise_conv_s4);
    riscv_depthwise_conv_s4(&ctx, &dw_conv_params, &quant_params, &input_dims,
                          test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                          bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_s4);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 200);

    input_dims = {1, 23, 1, 22};
    filter_dims.w = 1;
    filter_dims.h = 3;
    output_dims.w = 1;
    output_dims.h = 21;
    output_dims.c = 22;

    const int32_t depthwise_int4_1_output_mult[22] = {
    1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358,
    1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358,
    1533286358, 1533286358, 1533286358, 1533286358, 1533286358, 1533286358};

    const int32_t depthwise_int4_1_output_shift[22] = {-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
                                                      -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
    quant_params.multiplier = (int32_t *)depthwise_int4_1_output_mult;
    quant_params.shift = (int32_t *)depthwise_int4_1_output_shift;

    riscv_depthwise_conv_s4_opt_ref(&ctx, &dw_conv_params, &quant_params, &input_dims,
                              test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                              bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_depthwise_conv_s4_opt);
    riscv_depthwise_conv_s4_opt(&ctx, &dw_conv_params, &quant_params, &input_dims,
                              test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                              bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_s4_opt);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 462);

    riscv_depthwise_conv_wrapper_s4_ref(&ctx, &dw_conv_params, &quant_params, &input_dims,
                                  test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                                  bias_data1, &output_dims, output_q7);
    BENCH_START(riscv_depthwise_conv_wrapper_s4);
    riscv_depthwise_conv_wrapper_s4(&ctx, &dw_conv_params, &quant_params, &input_dims,
                                  test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                                  bias_data1, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_wrapper_s4);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 462);

    int64_t bias_int64[5] = {48, 116, -50, 127, -86};

    riscv_depthwise_conv_fast_s16_ref(&ctx, &dw_conv_params, &quant_params,
                                      &input_dims, test2, &filter_dims,
                                      test1 + Convolution_SIZE, &bias_dims,
                                      bias_int64, &output_dims, output_q15);
    BENCH_START(riscv_depthwise_conv_fast_s16);
    riscv_depthwise_conv_fast_s16(
        &ctx, &dw_conv_params, &quant_params, &input_dims, test2, &filter_dims,
        test1 + Convolution_SIZE, &bias_dims, bias_int64, &output_dims,
        output_q15 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_fast_s16);
    verify_results_q15(output_q15, output_q15 + Convolution_SIZE, 4 * 8 * 8);

    riscv_depthwise_conv_s16_ref(&ctx, &dw_conv_params, &quant_params,
        &input_dims, test2, &filter_dims,
        test1 + Convolution_SIZE, &bias_dims,
        bias_int64, &output_dims, output_q15);
    BENCH_START(riscv_depthwise_conv_s16);
    riscv_depthwise_conv_s16(
        &ctx, &dw_conv_params, &quant_params, &input_dims, test2, &filter_dims,
        test1 + Convolution_SIZE, &bias_dims, bias_int64, &output_dims,
        output_q15 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_s16);
    verify_results_q15(output_q15, output_q15 + Convolution_SIZE, 4 * 8 * 8);

    riscv_depthwise_conv_wrapper_s16_ref(&ctx, &dw_conv_params, &quant_params,
        &input_dims, test2, &filter_dims,
        test1 + Convolution_SIZE, &bias_dims,
        bias_int64, &output_dims, output_q15);
    BENCH_START(riscv_depthwise_conv_wrapper_s16);
    riscv_depthwise_conv_wrapper_s16(
        &ctx, &dw_conv_params, &quant_params, &input_dims, test2, &filter_dims,
        test1 + Convolution_SIZE, &bias_dims, bias_int64, &output_dims,
        output_q15 + Convolution_SIZE);
    BENCH_END(riscv_depthwise_conv_wrapper_s16);
    verify_results_q15(output_q15, output_q15 + Convolution_SIZE, 4 * 8 * 8);

    delete[] temp_buffer;
    delete[] bias_data;

    }
#endif

#ifdef TEST_TransposeConvolution
    /**
     * public functions:
     * riscv_transpose_conv_s8
     * riscv_transpose_conv_wrapper_s8
     */
    #define RCONV_IM_DIM_X 8
    #define RCONV_IM_DIM_Y 8
    #define RCONV_IM_CH 4
    #define RCONV_KER_DIM_X 5
    #define RCONV_KER_DIM_Y 3
    #define RCONV_STRIDE_X 1
    #define RCONV_STRIDE_Y 1
    #define RCONV_PADDING_X 2
    #define RCONV_PADDING_Y 1
    #define RCONV_OUT_CH 4
    #define RCONV_OUT_DIM_X 4
    #define RCONV_OUT_DIM_Y 4

    #define CONV_BIAS_SHIFT 1
    #define CONV_OUT_SHIFT 9

    q7_t *rconv_weight_q7 = test1;
    q7_t *rconv_bias_q7 = test1 + RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH;

    q15_t *rconv_buf = new q15_t[2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH
         + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH + 2 * RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH];

    q7_t *rconv_im_in_q7 = test1 + 2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH;
    q7_t *rconv_im_out_ref_q7 = output_q7;
    q7_t *rconv_im_out_opt_q7 = output_q7 + RCONV_IM_DIM_Y * RCONV_IM_DIM_X * RCONV_IM_CH;

    initialize_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("\r\nStart ConvolutionFunctions part3 tests\r\n");
    riscv_convolve_HWC_q7_fast_nonsquare_ref(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                      RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                      RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_ref_q7,
                                      RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);

    BENCH_START(riscv_convolve_HWC_q7_fast_nonsquare);
    riscv_convolve_HWC_q7_fast_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                       RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                       RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_opt_q7,
                                       RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q7_fast_nonsquare);
    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    BENCH_START(riscv_convolve_HWC_q7_basic_nonsquare);
    riscv_convolve_HWC_q7_basic_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                       RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                       RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_opt_q7,
                                       RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q7_basic_nonsquare);
    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    initialize_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    printf("start 1x1 conv q7 nonsquare fast implementation\n");
    BENCH_START(riscv_convolve_HWC_q7_fast_nonsquare);
    riscv_convolve_HWC_q7_fast_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                       RCONV_OUT_CH, 1, 1, 0, 0, RCONV_STRIDE_X,
                                       RCONV_STRIDE_Y, rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_ref_q7, RCONV_OUT_DIM_X,
                                       RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q7_fast_nonsquare);
    printf("start 1x1 conv q7 nonsquare dedicated function implementation\n");
    BENCH_START(riscv_convolve_1x1_HWC_q7_fast_nonsquare);
    riscv_convolve_1x1_HWC_q7_fast_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q7,
                                           RCONV_OUT_CH, 1, 1, 0, 0, RCONV_STRIDE_X,
                                           RCONV_STRIDE_Y, rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_opt_q7, RCONV_OUT_DIM_X,
                                           RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_END(riscv_convolve_1x1_HWC_q7_fast_nonsquare);

    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    riscv_depthwise_separable_conv_HWC_q7_nonsquare_ref(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH,
                                                      rconv_weight_q7, RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y,
                                                      RCONV_PADDING_X, RCONV_PADDING_Y, RCONV_STRIDE_X, RCONV_STRIDE_Y,
                                                      rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_ref_q7, RCONV_OUT_DIM_X,
                                                      RCONV_OUT_DIM_Y, rconv_buf, NULL);

    BENCH_START(riscv_depthwise_separable_conv_HWC_q7_nonsquare);
    riscv_depthwise_separable_conv_HWC_q7_nonsquare(rconv_im_in_q7, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH,
                                                  rconv_weight_q7, RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y,
                                                  RCONV_PADDING_X, RCONV_PADDING_Y, RCONV_STRIDE_X, RCONV_STRIDE_Y,
                                                  rconv_bias_q7, CONV_BIAS_SHIFT, CONV_OUT_SHIFT, rconv_im_out_opt_q7, RCONV_OUT_DIM_X,
                                                  RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_END(riscv_depthwise_separable_conv_HWC_q7_nonsquare);
    verify_results_q7(rconv_im_out_ref_q7, rconv_im_out_opt_q7, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    q15_t *rconv_weight_q15 = test2;
    q15_t *rconv_bias_q15 = test2 + RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH;

    q15_t *rconv_im_in_q15 = test2 + 2 * RCONV_KER_DIM_Y * RCONV_KER_DIM_X * RCONV_IM_CH * RCONV_OUT_CH;
    q15_t *rconv_im_out_ref_q15 = output_q15;
    q15_t *rconv_im_out_opt_q15 = output_q15 + RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH;

    initialize_results_q15(rconv_im_out_ref_q15, rconv_im_out_opt_q15, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);

    riscv_convolve_HWC_q15_fast_nonsquare_ref(rconv_im_in_q15, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q15,
                                      RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                      RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q15, CONV_BIAS_SHIFT, 22, rconv_im_out_ref_q15,
                                      RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_START(riscv_convolve_HWC_q15_fast_nonsquare);
    riscv_convolve_HWC_q15_fast_nonsquare(rconv_im_in_q15, RCONV_IM_DIM_X, RCONV_IM_DIM_Y, RCONV_IM_CH, rconv_weight_q15,
                                       RCONV_OUT_CH, RCONV_KER_DIM_X, RCONV_KER_DIM_Y, RCONV_PADDING_X, RCONV_PADDING_Y,
                                       RCONV_STRIDE_X, RCONV_STRIDE_Y, rconv_bias_q15, CONV_BIAS_SHIFT, 22, rconv_im_out_opt_q15,
                                       RCONV_OUT_DIM_X, RCONV_OUT_DIM_Y, rconv_buf, NULL);
    BENCH_END(riscv_convolve_HWC_q15_fast_nonsquare);

    verify_results_q15(rconv_im_out_ref_q15, rconv_im_out_opt_q15, RCONV_OUT_DIM_Y * RCONV_OUT_DIM_X * RCONV_OUT_CH);
    {
    #define Convolution_SIZE 2048
    q15_t *temp_buffer = new q15_t[Convolution_SIZE];
    int32_t multiplier[4] = {187431000, 141317000, 117516000, 151730000};
    int32_t shift[4] = {-10, -8, -7, -8};
    int32_t *bias_data = new int32_t[Convolution_SIZE];
    for (int i = 0; i < Convolution_SIZE; i++) {
        bias_data[i] = (rand() % 256 - 128);
    }

    nmsis_nn_context ctx = {temp_buffer, Convolution_SIZE};
    nmsis_nn_tile stride = {1, 1}, padding = {0, 0}, dilation = {1, 1};
    nmsis_nn_activation activation = {-12800, 12700};
    nmsis_nn_conv_params conv_params = {128, -128, stride, padding, dilation, activation};
    nmsis_nn_per_channel_quant_params quant_params = {multiplier, shift};
    nmsis_nn_dims input_dims = {1, 3, 3, 16};
    nmsis_nn_dims filter_dims = {4, 3, 3, 16};
    nmsis_nn_dims bias_dims = {4, 1, 1, 1};
    nmsis_nn_dims output_dims = {1, 5, 5, 4};

    output_dims.c = 3;
    nmsis_nn_context output_ctx = {temp_buffer, Convolution_SIZE};
    padding = {2, 2};
    nmsis_nn_tile padding_offsets = {2, 2};
    stride = {2, 2};
    dilation = {1, 1};
    activation = {-128, 127};
    nmsis_nn_transpose_conv_params transpose_conv_params = {128, -13, stride, padding, padding_offsets, dilation, activation};
    const int32_t transpose_conv_1_output_mult[3] = {2007829174, 2008826774, 2011503377};
    const int32_t transpose_conv_1_output_shift[3] = {-12, -12, -12};
    quant_params.multiplier = (int32_t *)transpose_conv_1_output_mult;
    quant_params.shift = (int32_t *)transpose_conv_1_output_shift;

    riscv_transpose_conv_s8_ref(&ctx, &output_ctx, &transpose_conv_params, &quant_params, &input_dims,
                          test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                          bias_data, &output_dims, output_q7);
    BENCH_START(riscv_transpose_conv_s8);
    riscv_transpose_conv_s8(&ctx, &output_ctx, &transpose_conv_params, &quant_params, &input_dims,
                          test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
                          bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_transpose_conv_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 1 * 5 * 5 * 4);

    riscv_transpose_conv_wrapper_s8_ref(&ctx, &output_ctx, &transpose_conv_params, &quant_params, &input_dims,
        test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
        bias_data, &output_dims, output_q7);
    BENCH_START(riscv_transpose_conv_wrapper_s8);
    riscv_transpose_conv_wrapper_s8(&ctx, &output_ctx, &transpose_conv_params, &quant_params, &input_dims,
        test1, &filter_dims, test1 + Convolution_SIZE, &bias_dims,
        bias_data, &output_dims, output_q7 + Convolution_SIZE);
    BENCH_END(riscv_transpose_conv_wrapper_s8);
    verify_results_q7(output_q7, output_q7 + Convolution_SIZE, 1 * 5 * 5 * 4);

    delete[] temp_buffer;
    delete[] bias_data;
    }

#endif

#ifdef TEST_Fully_connectedLayer
    /**
     * public functions:
     * riscv_batch_matmul_s16
     * riscv_batch_matmul_s8
     * riscv_fully_connected_per_channel_s8
     * riscv_fully_connected_s16
     * riscv_fully_connected_s4
     * riscv_fully_connected_s8
     * riscv_fully_connected_wrapper_s8
     * riscv_vector_sum_s8
     * riscv_vector_sum_s8_s64
     * 
     */
    #define IP_ROW_DIM 64
    #define IP_COL_DIM 64

    {
    q15_t *vec_buffer = new q15_t[256];
    q7_t  *ip_bias_q7 = test1 + IP_COL_DIM;

    q7_t *ip_out_q7_ref = output_q7;
    q7_t *ip_out_q7_opt = output_q7 + IP_ROW_DIM;
    q7_t *ip_out_q7_opt_fast = output_q7 + 2 * IP_ROW_DIM;
    q15_t *ip_out_q15_ref = output_q15 + IP_ROW_DIM;
    q15_t *ip_out_q15_opt = output_q15 + 2 * IP_ROW_DIM;

    #define CON_BIAS_SHIFT_Q7 1
    #define CON_OUT_SHIFT_Q7 9

    printf("\r\nStart FullyConnectedFunctions tests\r\n");

    riscv_fully_connected_q7_ref(test1, test1 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q7, CON_OUT_SHIFT_Q7, ip_bias_q7, ip_out_q7_ref, vec_buffer);
    BENCH_START(riscv_fully_connected_q7);
    riscv_fully_connected_q7(test1, test1 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q7, CON_OUT_SHIFT_Q7, ip_bias_q7, ip_out_q7_opt, vec_buffer);
    BENCH_END(riscv_fully_connected_q7);
    verify_results_q7(ip_out_q7_ref, ip_out_q7_opt, IP_ROW_DIM);

    riscv_fully_connected_q7_opt_ref(test1, test1 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q7, CON_OUT_SHIFT_Q7, ip_bias_q7,
                                   ip_out_q7_ref, vec_buffer);

    BENCH_START(riscv_fully_connected_q7_opt);
    riscv_fully_connected_q7_opt(test1, test1 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q7, CON_OUT_SHIFT_Q7, ip_bias_q7, ip_out_q7_opt_fast,
                               vec_buffer);
    BENCH_END(riscv_fully_connected_q7_opt);
    verify_results_q7(ip_out_q7_ref, ip_out_q7_opt_fast, IP_ROW_DIM);

    initialize_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);
    #define CON_BIAS_SHIFT_Q15 1
    #define CON_OUT_SHIFT_Q15 13

    riscv_fully_connected_q15_ref(test2, test2 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, test2 + IP_ROW_DIM * IP_COL_DIM, ip_out_q15_ref, vec_buffer);
    BENCH_START(riscv_fully_connected_q15);
    riscv_fully_connected_q15(test2, test2 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, test2 + IP_ROW_DIM * IP_COL_DIM, ip_out_q15_opt, vec_buffer);
    BENCH_END(riscv_fully_connected_q15);
    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    riscv_fully_connected_q15_opt_ref(test2, test2 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, test2 + IP_ROW_DIM * IP_COL_DIM, ip_out_q15_ref,
                                    NULL);
    BENCH_START(riscv_fully_connected_q15_opt);
    riscv_fully_connected_q15_opt(test2, test2 + IP_COL_DIM, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, test2 + IP_ROW_DIM * IP_COL_DIM, ip_out_q15_opt, NULL);
    BENCH_END(riscv_fully_connected_q15_opt);
    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    initialize_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);


    riscv_fully_connected_mat_q7_vec_q15_ref(test2, test1, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, ip_bias_q7, ip_out_q15_ref,
                                           vec_buffer);
    BENCH_START(riscv_fully_connected_mat_q7_vec_q15);
    riscv_fully_connected_mat_q7_vec_q15(test2, test1, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, ip_bias_q7, ip_out_q15_opt,
                                       vec_buffer);
    BENCH_END(riscv_fully_connected_mat_q7_vec_q15);
    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    riscv_fully_connected_mat_q7_vec_q15_opt_ref(test2, test1, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, ip_bias_q7,
                                               ip_out_q15_ref, vec_buffer);

    BENCH_START(riscv_fully_connected_mat_q7_vec_q15_opt);
    riscv_fully_connected_mat_q7_vec_q15_opt(test2, test1, IP_COL_DIM, IP_ROW_DIM, CON_BIAS_SHIFT_Q15, CON_OUT_SHIFT_Q15, ip_bias_q7,
                                           ip_out_q15_opt, vec_buffer);
    BENCH_END(riscv_fully_connected_mat_q7_vec_q15_opt);
    verify_results_q15(ip_out_q15_ref, ip_out_q15_opt, IP_ROW_DIM);

    int32_t fc_multiplier = 0x800000;
    int32_t fc_shift = 1;

    q7_t *fc_temp_buffer = new q7_t[320];
    int32_t *fc_bias_data = new int32_t[320];
    for (int i = 0; i < 320; i++) {
        fc_bias_data[i] = rand() % 10;
    }

    nmsis_nn_context fc_ctx = {fc_temp_buffer, 320};
    nmsis_nn_tile fc_stride = {2, 2};
    nmsis_nn_tile fc_padding = {2, 2};
    nmsis_nn_tile fc_dilation = {2, 2};
    nmsis_nn_activation fc_activation = {-1280, 1270};
    nmsis_nn_conv_params fc_conv_params = {1, 1, fc_stride, fc_padding, fc_dilation, fc_activation};
    nmsis_nn_per_tensor_quant_params  fc_quant_params = {fc_multiplier, fc_shift};
    nmsis_nn_dims fc_input_dims = {3, 1, 2, 10};
    nmsis_nn_dims fc_filter_dims = {20, 1, 2, 32};
    nmsis_nn_dims fc_bias_dims = {20, 1, 2, 32};
    nmsis_nn_dims fc_output_dims = {3, 1, 2, 32};
    nmsis_nn_fc_params fc_fc_params = {0, 0, 0, fc_activation};

    riscv_fully_connected_s8_ref(&fc_ctx, &fc_fc_params, &fc_quant_params, &fc_input_dims, test1, &fc_filter_dims, test1 + 320, &fc_bias_dims, fc_bias_data, &fc_output_dims, output_q7);
    BENCH_START(riscv_fully_connected_s8);
    riscv_fully_connected_s8(&fc_ctx, &fc_fc_params, &fc_quant_params, &fc_input_dims, test1, &fc_filter_dims, test1 + 320, &fc_bias_dims, fc_bias_data, &fc_output_dims, output_q7 + 320);
    BENCH_END(riscv_fully_connected_s8);
    verify_results_q7(output_q7, output_q7 + 320, 96);

    int32_t multi[10] = {0x800000, 0x800000, 0x800000, 0x800000, 0x800000,
                         0x800000, 0x800000, 0x800000, 0x800000, 0x800000};
    int32_t shift[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    nmsis_nn_per_channel_quant_params per_channel_quant = {multi, shift};
    riscv_fully_connected_per_channel_s8_ref(
        &fc_ctx, &fc_fc_params, &per_channel_quant, &fc_input_dims, test1,
        &fc_filter_dims, test1 + 320, &fc_bias_dims, fc_bias_data,
        &fc_output_dims, output_q7);
    BENCH_START(riscv_fully_connected_s8);
    riscv_fully_connected_per_channel_s8(
        &fc_ctx, &fc_fc_params, &per_channel_quant, &fc_input_dims, test1,
        &fc_filter_dims, test1 + 320, &fc_bias_dims, fc_bias_data,
        &fc_output_dims, output_q7 + 320);
    BENCH_END(riscv_fully_connected_s8);
    verify_results_q7(output_q7, output_q7 + 320, 96);

    nmsis_nn_quant_params quant_params = {&fc_multiplier, &fc_shift, 1};
    riscv_fully_connected_wrapper_s8_ref(
        &fc_ctx, &fc_fc_params, &quant_params, &fc_input_dims, test1,
        &fc_filter_dims, test1 + 320, &fc_bias_dims, fc_bias_data,
        &fc_output_dims, output_q7);
    BENCH_START(riscv_fully_connected_wrapper_s8);
    riscv_fully_connected_wrapper_s8(&fc_ctx, &fc_fc_params, &quant_params,
                                     &fc_input_dims, test1, &fc_filter_dims,
                                     test1 + 320, &fc_bias_dims, fc_bias_data,
                                     &fc_output_dims, output_q7 + 320);
    BENCH_END(riscv_fully_connected_wrapper_s8);
    verify_results_q7(output_q7, output_q7 + 320, 96);

    nmsis_nn_context ctx;
    riscv_fully_connected_s4_ref(&ctx, &fc_fc_params, &fc_quant_params, &fc_input_dims, test1, &fc_filter_dims,
                           test1 + 320, &fc_bias_dims, fc_bias_data, &fc_output_dims, output_q7);
    BENCH_START(riscv_fully_connected_s4);
    riscv_fully_connected_s4(&ctx, &fc_fc_params, &fc_quant_params, &fc_input_dims, test1, &fc_filter_dims,
                           test1 + 320, &fc_bias_dims, fc_bias_data, &fc_output_dims, output_q7 + 320);
    BENCH_END(riscv_fully_connected_s4);
    verify_results_q7(output_q7, output_q7 + 320, 96);

    nmsis_nn_bmm_params bmm_params = {false, false, fc_fc_params};
    nmsis_nn_dims fc_input2_dims = {3, 1, 3, 10};
    fc_output_dims.h = 1;
    fc_output_dims.w = 2;
    fc_output_dims.c = 3;
    riscv_batch_matmul_s8_ref(&fc_ctx, &bmm_params, &fc_quant_params,
                              &fc_input_dims, test1, &fc_input2_dims,
                              test1 + 320, &fc_output_dims, output_q7);
    BENCH_START(riscv_batch_matmul_s8);
    riscv_batch_matmul_s8(&fc_ctx, &bmm_params, &fc_quant_params,
                          &fc_input_dims, test1, &fc_input2_dims, test1 + 320,
                          &fc_output_dims, output_q7 + 320);
    BENCH_END(riscv_batch_matmul_s8)
    verify_results_q7(output_q7, output_q7 + 320, 18);

    riscv_batch_matmul_s16_ref(&fc_ctx, &bmm_params, &fc_quant_params,
                               &fc_input_dims, test2, &fc_input2_dims,
                               test2 + 320, &fc_output_dims, output_q15);
    BENCH_START(riscv_batch_matmul_s16);
    riscv_batch_matmul_s16(&fc_ctx, &bmm_params, &fc_quant_params,
                           &fc_input_dims, test2, &fc_input2_dims, test2 + 320,
                           &fc_output_dims, output_q15 + 320);
    BENCH_END(riscv_batch_matmul_s16)
    verify_results_q15(output_q15, output_q15 + 320, 18);
    
    #define VEC_ROWS 16
    #define VEC_COLS 32
    int64_t bias64[VEC_ROWS];

    for (int i = 0; i < VEC_ROWS; i++)
    {
        bias64[i] = (int64_t)(rand() % 0x7fffffffL - 0x7fffffffL / 2);
    }
    int64_t vector_sum_buf[VEC_ROWS];
    int64_t vector_sum_buf_ref[VEC_ROWS];
    const int32_t vector_cols = VEC_COLS;
    const int32_t vector_rows = VEC_ROWS;
    const int32_t lhs_offset = 4;

    riscv_vector_sum_s8_s64_ref(vector_sum_buf_ref, vector_cols, vector_rows, test1, lhs_offset, bias64);
    BENCH_START(riscv_vector_sum_s8_s64);
    riscv_vector_sum_s8_s64(vector_sum_buf, vector_cols, vector_rows, test1, lhs_offset, bias64);
    BENCH_END(riscv_vector_sum_s8_s64);

    verify_results_int64(vector_sum_buf_ref, vector_sum_buf, VEC_ROWS);

    delete[] fc_temp_buffer;
    delete[] fc_bias_data;
    delete[] vec_buffer;
    }

#endif

#ifdef TEST_Pooling
    #define Pooling_SIZE 600
    q7_t *pool_temp_buffer = new q7_t[Pooling_SIZE];
    int32_t pool_multiplier = 1;
    int32_t pool_shift = 0;

    nmsis_nn_context pool_ctx = {pool_temp_buffer, 10};
    nmsis_nn_tile pool_stride = {1, 1};
    nmsis_nn_tile pool_padding = {0, 0};
    nmsis_nn_tile pool_dilation = {2, 2};
    nmsis_nn_activation pool_activation = {-128, 127};
    nmsis_nn_conv_params pool_conv_params = {1, 1, pool_stride, pool_padding, pool_dilation, pool_activation};
    nmsis_nn_per_channel_quant_params pool_quant_params = {&pool_multiplier, &pool_shift};
    nmsis_nn_dims pool_input_dims = {2, 8, 8, 32};
    nmsis_nn_dims pool_filter_dims = {32, 4, 4, 32};
    nmsis_nn_dims pool_bias_dims = {2, 3, 3, 32};
    nmsis_nn_dims pool_output_dims = {2, 3, 3, 32};
    nmsis_nn_dw_conv_params pool_dw_conv_params = {0, 0, 1, pool_stride, pool_padding, pool_dilation, pool_activation};
    nmsis_nn_pool_params pool_pool_params = {pool_stride, pool_padding, pool_activation};

    printf("\r\nStart PoolingFunctions tests\r\n");

    riscv_max_pool_s8_ref(&pool_ctx, &pool_pool_params, &pool_input_dims, test1, &pool_filter_dims, &pool_output_dims, output_q7);
    BENCH_START(riscv_max_pool_s8);
    riscv_max_pool_s8(&pool_ctx, &pool_pool_params, &pool_input_dims, test1, &pool_filter_dims, &pool_output_dims, output_q7 + Pooling_SIZE);
    BENCH_END(riscv_max_pool_s8);
    verify_results_q7(output_q7, output_q7 + Pooling_SIZE, 288);

    riscv_avgpool_s8_ref(&pool_ctx, &pool_pool_params, &pool_input_dims, test1, &pool_filter_dims, &pool_output_dims, output_q7);
    BENCH_START(riscv_avgpool_s8);
    riscv_avgpool_s8(&pool_ctx,&pool_pool_params, &pool_input_dims, test1, &pool_filter_dims, &pool_output_dims, output_q7 + Pooling_SIZE);
    BENCH_END(riscv_avgpool_s8);
    verify_results_q7(output_q7, output_q7 + Pooling_SIZE, 288);

    #define POOL_IM_DIM 32
    #define POOL_IM_CH 8
    #define DIM_IM_OUT 16

    q7_t *pool_out_ref = output_q7;
    q7_t *pool_out_opt = output_q7 + DIM_IM_OUT * DIM_IM_OUT * POOL_IM_CH;

    riscv_maxpool_q7_HWC_ref(test1, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, DIM_IM_OUT, (q7_t *) pool_temp_buffer, pool_out_ref);
    BENCH_START(riscv_maxpool_q7_HWC);
    riscv_maxpool_q7_HWC(test1, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, DIM_IM_OUT, (q7_t *) pool_temp_buffer, pool_out_opt);
    BENCH_END(riscv_maxpool_q7_HWC);
    verify_results_q7(pool_out_ref, pool_out_opt, DIM_IM_OUT * DIM_IM_OUT * POOL_IM_CH);

    riscv_avepool_q7_HWC_ref(test1, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, DIM_IM_OUT, (q7_t *) pool_temp_buffer, pool_out_ref);
    BENCH_START(riscv_avepool_q7_HWC);
    riscv_avepool_q7_HWC(test1, POOL_IM_DIM, POOL_IM_CH, 3, 0, 2, DIM_IM_OUT, (q7_t *) pool_temp_buffer, pool_out_opt);
    BENCH_END(riscv_avepool_q7_HWC);
    verify_results_q7(pool_out_ref, pool_out_opt, DIM_IM_OUT * DIM_IM_OUT * POOL_IM_CH);

    delete[] pool_temp_buffer;

#endif

#ifdef TEST_Lstm
    #define LSTM_1_BUFFER_SIZE 11
    #define LSTM_1_INPUT_BATCHES 1
    #define LSTM_1_DST_SIZE 110
    #define LSTM_1_TIME_STEPS 10
    #define LSTM_1_NUMBER_UNITS 11
    #define LSTM_1_NUMBER_INPUTS 22
    #define LSTM_1_TIME_MAJOR 1
    #define LSTM_1_IN_ACTIVATION_MIN -3
    #define LSTM_1_IN_ACTIVATION_MAX 32767
    #define LSTM_1_IN_TO_INPUT_MULTIPLIER 1075906048
    #define LSTM_1_IN_TO_INPUT_SHIFT -2
    #define LSTM_1_IN_TO_FORGET_MULTIPLIER 1085883136
    #define LSTM_1_IN_TO_FORGET_SHIFT -2
    #define LSTM_1_IN_TO_CELL_MULTIPLIER 1084231552
    #define LSTM_1_IN_TO_CELL_SHIFT -2
    #define LSTM_1_IN_TO_OUTPUT_MULTIPLIER 1085274240
    #define LSTM_1_IN_TO_OUTPUT_SHIFT -2
    #define LSTM_1_RECURRENT_TO_INPUT_MULTIPLIER 1523696256
    #define LSTM_1_RECURRENT_TO_INPUT_SHIFT -2
    #define LSTM_1_RECURRENT_TO_FORGET_MULTIPLIER 1511291392
    #define LSTM_1_RECURRENT_TO_FORGET_SHIFT -2
    #define LSTM_1_RECURRENT_TO_CELL_MULTIPLIER 1523716992
    #define LSTM_1_RECURRENT_TO_CELL_SHIFT -2
    #define LSTM_1_RECURRENT_TO_OUTPUT_MULTIPLIER 1525092864
    #define LSTM_1_RECURRENT_TO_OUTPUT_SHIFT -2
    #define LSTM_1_FORGET_MULTIPLIER 1073741824
    #define LSTM_1_FORGET_SHIFT -14
    #define LSTM_1_INPUT_MULTIPLIER 1073741824
    #define LSTM_1_INPUT_SHIFT -17
    #define LSTM_1_HIDDEN_MULTIPLIER 1522160019
    #define LSTM_1_HIDDEN_SHIFT -22
    #define LSTM_1_HIDDEN_OFFSET -23
    #define LSTM_1_DATA_OFFSET 128
    #define LSTM_1_OUTPUT_STATE_OFFSET -23
    #define LSTM_1_CELL_STATE_SHIFT -12

    int8_t lstm_output[LSTM_1_DST_SIZE] = {0};
    int8_t lstm_output_ref[LSTM_1_DST_SIZE] = {0};

    // Calculate kernel sums if using MVE-extension
    int32_t input_data_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t forget_data_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t cell_data_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t output_data_kernel_sum[LSTM_1_NUMBER_UNITS];

    int32_t input_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t forget_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t cell_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];
    int32_t output_hidden_kernel_sum[LSTM_1_NUMBER_UNITS];

    int32_t size_data = LSTM_1_NUMBER_INPUTS;
    int32_t size_hidden = LSTM_1_NUMBER_UNITS;

    int8_t lstm_1_input[LSTM_1_TIME_STEPS * LSTM_1_NUMBER_INPUTS];
    for (int i = 0; i < LSTM_1_TIME_STEPS * LSTM_1_NUMBER_INPUTS; i++)
    {
        lstm_1_input[i] = (int8_t)(rand() % 0x7f - 0x7f / 2);
    }

    int8_t lstm_1_input_to_input_w[LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_recurrent_input_to_input_w[LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_input_to_forget_w[LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_recurrent_input_to_forget_w[LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_input_to_cell_w[LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_recurrent_input_to_cell_w[LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_input_to_output_w[LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
    int8_t lstm_1_recurrent_input_to_output_w[LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];

    #define LARGEST_BUFFER_SIZE LSTM_1_BUFFER_SIZE

    int16_t buffer1[LARGEST_BUFFER_SIZE];
    int16_t buffer2[LARGEST_BUFFER_SIZE];
    int16_t buffer3[LARGEST_BUFFER_SIZE];

    for (int i = 0; i < LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS; i++)
    {
        lstm_1_input_to_input_w[i] = test1[i];
        lstm_1_input_to_forget_w[i] = test1[i + LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
        lstm_1_input_to_cell_w[i] = test1[i + 2 * LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
        lstm_1_input_to_output_w[i] = test1[i + 3 * LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS];
    }
    int tmp_num = 4 * LSTM_1_NUMBER_INPUTS * LSTM_1_NUMBER_UNITS;
    for (int i = 0; i < LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS; i++)
    {
        lstm_1_recurrent_input_to_input_w[i] = test1[i + tmp_num];
        lstm_1_recurrent_input_to_forget_w[i] = test1[i + tmp_num + LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];
        lstm_1_recurrent_input_to_cell_w[i] = test1[i + tmp_num + LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];
        lstm_1_recurrent_input_to_output_w[i] = test1[i + tmp_num + LSTM_1_BUFFER_SIZE * LSTM_1_NUMBER_UNITS];
    }

    const int32_t lstm_1_input_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {-32410, -104, 21567, -21097, 12535, 259, 8401, 10604, 24974, 30367, -9986};
    const int32_t lstm_1_forget_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {-23170, -13466, -6110, 22504, -22652, 25549, -26211, -32267, 15774, -29318, 6943};
    const int32_t lstm_1_cell_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {-16190, 6797, 24062, 29971, -22780, 17656, 14698, 1849, 4054, 14590, -20709};
    const int32_t lstm_1_output_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {22266, 16773, 25291, -17527, -11578, 16072, 21528, 3001, -28336, 29661, 30876};


    riscv_vector_sum_s8(&input_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_input_w[0],
                      LSTM_1_DATA_OFFSET,
                      0,
                      &lstm_1_input_gate_bias[0]);
    riscv_vector_sum_s8(&forget_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_forget_w[0],
                      LSTM_1_DATA_OFFSET,
                      0,
                      &lstm_1_forget_gate_bias[0]);
    riscv_vector_sum_s8(&cell_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_cell_w[0],
                      LSTM_1_DATA_OFFSET,
                      0,
                      &lstm_1_cell_gate_bias[0]);
    riscv_vector_sum_s8(&output_data_kernel_sum[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_output_w[0],
                      LSTM_1_DATA_OFFSET,
                      0,
                      &lstm_1_output_gate_bias[0]);

    riscv_vector_sum_s8(&input_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_input_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      0,
                      NULL);
    riscv_vector_sum_s8(&forget_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_forget_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      0,
                      NULL);
    riscv_vector_sum_s8(&cell_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_cell_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      0,
                      NULL);
    riscv_vector_sum_s8(&output_hidden_kernel_sum[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_output_w[0],
                      -LSTM_1_HIDDEN_OFFSET,
                      0,
                      NULL);

    // INPUT GATE
    nmsis_nn_lstm_gate gate_input = {LSTM_1_IN_TO_INPUT_MULTIPLIER,
                                           LSTM_1_IN_TO_INPUT_SHIFT,
                                           &lstm_1_input_to_input_w[0],
                                           &input_data_kernel_sum[0],
                                           LSTM_1_RECURRENT_TO_INPUT_MULTIPLIER,
                                           LSTM_1_RECURRENT_TO_INPUT_SHIFT,
                                           &lstm_1_recurrent_input_to_input_w[0],
                                           &input_hidden_kernel_sum[0],
                                           &lstm_1_input_gate_bias[0],
                                           RISCV_SIGMOID};

    // FORGET GATE
    nmsis_nn_lstm_gate gate_forget = {LSTM_1_IN_TO_FORGET_MULTIPLIER,
                                            LSTM_1_IN_TO_FORGET_SHIFT,
                                            &lstm_1_input_to_forget_w[0],
                                            &forget_data_kernel_sum[0],
                                            LSTM_1_RECURRENT_TO_FORGET_MULTIPLIER,
                                            LSTM_1_RECURRENT_TO_FORGET_SHIFT,
                                            &lstm_1_recurrent_input_to_forget_w[0],
                                            &forget_hidden_kernel_sum[0],
                                            &lstm_1_forget_gate_bias[0],
                                            RISCV_SIGMOID};

    // CELL GATE
    nmsis_nn_lstm_gate gate_cell = {LSTM_1_IN_TO_CELL_MULTIPLIER,
                                          LSTM_1_IN_TO_CELL_SHIFT,
                                          &lstm_1_input_to_cell_w[0],
                                          &cell_data_kernel_sum[0],
                                          LSTM_1_RECURRENT_TO_CELL_MULTIPLIER,
                                          LSTM_1_RECURRENT_TO_CELL_SHIFT,
                                          &lstm_1_recurrent_input_to_cell_w[0],
                                          &cell_hidden_kernel_sum[0],
                                          &lstm_1_cell_gate_bias[0],
                                          RISCV_TANH};

    // OUTPUT GATE
    nmsis_nn_lstm_gate gate_output = {LSTM_1_IN_TO_OUTPUT_MULTIPLIER,
                                            LSTM_1_IN_TO_OUTPUT_SHIFT,
                                            &lstm_1_input_to_output_w[0],
                                            &output_data_kernel_sum[0],
                                            LSTM_1_RECURRENT_TO_OUTPUT_MULTIPLIER,
                                            LSTM_1_RECURRENT_TO_OUTPUT_SHIFT,
                                            &lstm_1_recurrent_input_to_output_w[0],
                                            &output_hidden_kernel_sum[0],
                                            &lstm_1_output_gate_bias[0],
                                            RISCV_SIGMOID};

    // LSTM DATA
    nmsis_nn_lstm_params params = {LSTM_1_TIME_MAJOR,
                                         LSTM_1_INPUT_BATCHES,
                                         LSTM_1_TIME_STEPS,
                                         LSTM_1_NUMBER_INPUTS,
                                         LSTM_1_NUMBER_UNITS,
                                         LSTM_1_DATA_OFFSET,
                                         LSTM_1_FORGET_MULTIPLIER,
                                         LSTM_1_FORGET_SHIFT,
                                         LSTM_1_INPUT_MULTIPLIER,
                                         LSTM_1_INPUT_SHIFT,
                                         LSTM_1_IN_ACTIVATION_MAX,
                                         LSTM_1_CELL_STATE_SHIFT,
                                         LSTM_1_HIDDEN_MULTIPLIER,
                                         LSTM_1_HIDDEN_SHIFT,
                                         LSTM_1_HIDDEN_OFFSET,
                                         gate_forget,
                                         gate_input,
                                         gate_cell,
                                         gate_output};

    // BUFFERS
    nmsis_nn_lstm_context buffers;
    buffers.temp1 = buffer1;
    buffers.temp2 = buffer2;
    buffers.cell_state = buffer3;

    riscv_lstm_unidirectional_s8_ref(lstm_1_input, lstm_output_ref, &params, &buffers);

    BENCH_START(riscv_lstm_unidirectional_s8);
    riscv_lstm_unidirectional_s8(lstm_1_input, lstm_output, &params, &buffers);
    BENCH_END(riscv_lstm_unidirectional_s8);

    verify_results_q7(lstm_output_ref, lstm_output, LSTM_1_DST_SIZE);

    #define LARGEST_BUFFER_SIZE2 (LSTM_1_NUMBER_UNITS * LSTM_1_INPUT_BATCHES * LSTM_1_TIME_STEPS)
    int16_t buffer11[LARGEST_BUFFER_SIZE2];
    int16_t buffer22[LARGEST_BUFFER_SIZE2];
    int16_t buffer33[LARGEST_BUFFER_SIZE2];

    int64_t input_data_kernel_sum_s64[LSTM_1_NUMBER_UNITS];
    int64_t forget_data_kernel_sum_s64[LSTM_1_NUMBER_UNITS];
    int64_t cell_data_kernel_sum_s64[LSTM_1_NUMBER_UNITS];
    int64_t output_data_kernel_sum_s64[LSTM_1_NUMBER_UNITS];

    int64_t input_hidden_kernel_sum_s64[LSTM_1_NUMBER_UNITS];
    int64_t forget_hidden_kernel_sum_s64[LSTM_1_NUMBER_UNITS];
    int64_t cell_hidden_kernel_sum_s64[LSTM_1_NUMBER_UNITS];
    int64_t output_hidden_kernel_sum_s64[LSTM_1_NUMBER_UNITS];

    int16_t lstm_input_s16[LSTM_1_TIME_STEPS * LSTM_1_NUMBER_INPUTS];
    for (int i = 0; i < LSTM_1_TIME_STEPS * LSTM_1_NUMBER_INPUTS; i++)
    {
        lstm_input_s16[i] = (int16_t)(rand() % 0x7fff - 0x7fff / 2);
    }
    int16_t lstm_output_s16[LSTM_1_DST_SIZE] = {0};
    int16_t lstm_output_s16_ref[LSTM_1_DST_SIZE] = {0};

    const int64_t lstm_1_s16_input_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {13543, 1638, 25244, 3970, 6981, 29954, 4375, 16836, 30316, 6762, 4189};
    const int64_t lstm_1_s16_forget_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {8691, 24230, 5069, 6375, 13615, 17184, 3326, 27937, 5850, 1018, 15410};
    const int64_t lstm_1_s16_cell_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {20142, 23563, 15741, 25098, 29041, 25327, 12730, 19511, 21749, 13563, 31032};
    const int64_t lstm_1_s16_output_gate_bias[LSTM_1_NUMBER_UNITS] =
                  {8300, 1579, 11812, 11224, 18423, 11748, 9932, 26717, 23831, 11737, 10113};


    riscv_vector_sum_s8_s64(&input_data_kernel_sum_s64[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_input_w[0],
                      0,
                      &lstm_1_s16_input_gate_bias[0]);
    riscv_vector_sum_s8_s64(&forget_data_kernel_sum_s64[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_forget_w[0],
                      0,
                      &lstm_1_s16_forget_gate_bias[0]);
    riscv_vector_sum_s8_s64(&cell_data_kernel_sum_s64[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_cell_w[0],
                      0,
                      &lstm_1_s16_cell_gate_bias[0]);
    riscv_vector_sum_s8_s64(&output_data_kernel_sum_s64[0],
                      size_data,
                      size_hidden,
                      &lstm_1_input_to_output_w[0],
                      0,
                      &lstm_1_s16_output_gate_bias[0]);

    riscv_vector_sum_s8_s64(&input_hidden_kernel_sum_s64[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_input_w[0],
                      0,
                      NULL);
    riscv_vector_sum_s8_s64(&forget_hidden_kernel_sum_s64[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_forget_w[0],
                      0,
                      NULL);
    riscv_vector_sum_s8_s64(&cell_hidden_kernel_sum_s64[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_cell_w[0],
                      0,
                      NULL);
    riscv_vector_sum_s8_s64(&output_hidden_kernel_sum_s64[0],
                      size_hidden,
                      size_hidden,
                      &lstm_1_recurrent_input_to_output_w[0],
                      0,
                      NULL);

    // INPUT GATE
    gate_input = {LSTM_1_IN_TO_INPUT_MULTIPLIER,
                                           LSTM_1_IN_TO_INPUT_SHIFT,
                                           &lstm_1_input_to_input_w[0],
                                           &input_data_kernel_sum_s64[0],
                                           LSTM_1_RECURRENT_TO_INPUT_MULTIPLIER,
                                           LSTM_1_RECURRENT_TO_INPUT_SHIFT,
                                           &lstm_1_recurrent_input_to_input_w[0],
                                           &input_hidden_kernel_sum_s64[0],
                                           &lstm_1_input_gate_bias[0],
                                           RISCV_SIGMOID};

    // FORGET GATE
    gate_forget = {LSTM_1_IN_TO_FORGET_MULTIPLIER,
                                            LSTM_1_IN_TO_FORGET_SHIFT,
                                            &lstm_1_input_to_forget_w[0],
                                            &forget_data_kernel_sum_s64[0],
                                            LSTM_1_RECURRENT_TO_FORGET_MULTIPLIER,
                                            LSTM_1_RECURRENT_TO_FORGET_SHIFT,
                                            &lstm_1_recurrent_input_to_forget_w[0],
                                            &forget_hidden_kernel_sum_s64[0],
                                            &lstm_1_forget_gate_bias[0],
                                            RISCV_SIGMOID};

    // CELL GATE
    gate_cell = {LSTM_1_IN_TO_CELL_MULTIPLIER,
                                          LSTM_1_IN_TO_CELL_SHIFT,
                                          &lstm_1_input_to_cell_w[0],
                                          &cell_data_kernel_sum_s64[0],
                                          LSTM_1_RECURRENT_TO_CELL_MULTIPLIER,
                                          LSTM_1_RECURRENT_TO_CELL_SHIFT,
                                          &lstm_1_recurrent_input_to_cell_w[0],
                                          &cell_hidden_kernel_sum_s64[0],
                                          &lstm_1_cell_gate_bias[0],
                                          RISCV_TANH};

    // OUTPUT GATE
    gate_output = {LSTM_1_IN_TO_OUTPUT_MULTIPLIER,
                                            LSTM_1_IN_TO_OUTPUT_SHIFT,
                                            &lstm_1_input_to_output_w[0],
                                            &output_data_kernel_sum_s64[0],
                                            LSTM_1_RECURRENT_TO_OUTPUT_MULTIPLIER,
                                            LSTM_1_RECURRENT_TO_OUTPUT_SHIFT,
                                            &lstm_1_recurrent_input_to_output_w[0],
                                            &output_hidden_kernel_sum_s64[0],
                                            &lstm_1_output_gate_bias[0],
                                            RISCV_SIGMOID};

    params = {LSTM_1_TIME_MAJOR,
              LSTM_1_INPUT_BATCHES,
              LSTM_1_TIME_STEPS,
              LSTM_1_NUMBER_INPUTS,
              LSTM_1_NUMBER_UNITS,
              LSTM_1_DATA_OFFSET,
              LSTM_1_FORGET_MULTIPLIER,
              LSTM_1_FORGET_SHIFT,
              LSTM_1_INPUT_MULTIPLIER,
              LSTM_1_INPUT_SHIFT,
              LSTM_1_IN_ACTIVATION_MAX,
              LSTM_1_CELL_STATE_SHIFT,
              LSTM_1_HIDDEN_MULTIPLIER,
              LSTM_1_HIDDEN_SHIFT,
              LSTM_1_HIDDEN_OFFSET,
              gate_forget,
              gate_input,
              gate_cell,
              gate_output};

    buffers.temp1 = buffer11;
    buffers.temp2 = buffer22;
    buffers.cell_state = buffer33;

    riscv_lstm_unidirectional_s16_ref(lstm_input_s16, lstm_output_s16_ref, &params, &buffers);

    BENCH_START(riscv_lstm_unidirectional_s16);
    riscv_lstm_unidirectional_s16(lstm_input_s16, lstm_output_s16, &params, &buffers);
    BENCH_END(riscv_lstm_unidirectional_s16);

    verify_results_q15(lstm_output_s16_ref, lstm_output_s16, LSTM_1_DST_SIZE);
#endif

#ifdef TEST_Softmax
    #define Softmax_SIZE 100
    uint8_t *test5 = new uint8_t[Softmax_SIZE];
    uint8_t *test6 = new uint8_t[Softmax_SIZE * 2];

    for (int i = 0; i < Softmax_SIZE; i++) {
        test5[i] = (rand() % 256);
    }

    printf("\r\nStart SoftmaxFunctions tests\r\n");

    riscv_softmax_q15_ref(test2, Softmax_SIZE, output_q15);
    BENCH_START(riscv_softmax_q15);
    riscv_softmax_q15(test2, Softmax_SIZE, output_q15 + Softmax_SIZE);
    BENCH_END(riscv_softmax_q15);
    verify_results_q15(output_q15, output_q15 + Softmax_SIZE, Softmax_SIZE);

    riscv_softmax_q7_ref(test1, Softmax_SIZE, output_q7);
    BENCH_START(riscv_softmax_q7);
    riscv_softmax_q7(test1, Softmax_SIZE, output_q7 + Softmax_SIZE);
    BENCH_END(riscv_softmax_q7);
    verify_results_q7(output_q7, output_q7 + Softmax_SIZE, Softmax_SIZE);

    riscv_softmax_s8_ref(test1, 3, 30, 1077952640, 19, -3968, (int8_t *)output_q7);
    BENCH_START(riscv_softmax_s8);
    riscv_softmax_s8(test1, 3, 30, 1077952640, 19, -3968, (int8_t *)output_q7 + Softmax_SIZE);
    BENCH_END(riscv_softmax_s8);
    verify_results_q7(output_q7, output_q7 + Softmax_SIZE, Softmax_SIZE);

    riscv_softmax_s8_s16_ref(test1, 3, 30, 1077952640, 19, -3968, output_q15);
    BENCH_START(riscv_softmax_s8_s16);
    riscv_softmax_s8_s16(test1, 3, 30, 1077952640, 19, -3968, output_q15 + Softmax_SIZE);
    BENCH_END(riscv_softmax_s8_s16);
    verify_results_q15(output_q15, output_q15 + Softmax_SIZE, Softmax_SIZE);

    riscv_softmax_u8_ref(test5, 3, 30, 1077952640, 19, -3968, test6);
    BENCH_START(riscv_softmax_u8);
    riscv_softmax_u8(test5, 3, 30, 1077952640, 19, -3968, test6 + Softmax_SIZE);
    BENCH_END(riscv_softmax_u8);
    verify_results_q7((q7_t *)test6, (q7_t *)(test6 + Softmax_SIZE), 3 * 30);

    riscv_softmax_with_batch_q7_ref(test1, 2, 20, output_q7);
    BENCH_START(riscv_softmax_with_batch_q7);
    riscv_softmax_with_batch_q7(test1, 2, 20, output_q7 + Softmax_SIZE);
    BENCH_END(riscv_softmax_with_batch_q7);
    verify_results_q7(output_q7, output_q7 + Softmax_SIZE, 40);

    delete[] test5;
    delete[] test6;

#endif

#ifdef TEST_SVD
    {
    #define SVD_SIZE 2048 * 2
    q7_t *scratch_buf = new q7_t[SVD_SIZE * 2];
    q7_t *svdf_state_ref = new q7_t[SVD_SIZE];
    q7_t *svdf_state_opt = new q7_t[SVD_SIZE];

    for (int i = 0; i < SVD_SIZE; i++) {
        svdf_state_ref[i] = (rand() % 256 - 128);
        svdf_state_opt[i] = svdf_state_ref[i];
    }

    #define SVDF_MULTIPLIER_IN 1717987072
    #define SVDF_MULTIPLIER_OUT 1099511552
    #define SVDF_SHIFT_1 -3
    #define SVDF_SHIFT_2 -11

    #define SVDF_DST_SIZE 6

    nmsis_nn_context input_ctx = {scratch_buf, SVD_SIZE};
    nmsis_nn_context output_ctx = {scratch_buf + SVD_SIZE, SVD_SIZE};
    nmsis_nn_svdf_params svdf_params;
    nmsis_nn_dims svdf_input_dims;
    nmsis_nn_dims weights_feature_dims;
    nmsis_nn_dims weights_time_dims;
    nmsis_nn_dims state_dims = {.n = 0,  .h = 0, .w = 0, .c = 0};
    nmsis_nn_dims svdf_output_dims = {.n = 0,  .h = 0, .w = 0, .c = 0};
    nmsis_nn_dims svdf_bias_dims = {.n = 0,  .h = 0, .w = 0, .c = 0};
    nmsis_nn_per_tensor_quant_params input_quant_params;
    nmsis_nn_per_tensor_quant_params output_quant_params ;

    svdf_input_dims.n = 20;
    svdf_input_dims.h = 20;
    weights_feature_dims.n = 24;
    weights_time_dims.h = 8;

    input_quant_params.multiplier = SVDF_MULTIPLIER_IN;
    input_quant_params.shift = SVDF_SHIFT_1;
    output_quant_params.multiplier = SVDF_MULTIPLIER_OUT;
    output_quant_params.shift = SVDF_SHIFT_2;

    svdf_params.input_activation.min = -32768;
    svdf_params.input_activation.max = 32767;
    svdf_params.output_activation.min = -128;
    svdf_params.output_activation.max = 127;
    svdf_params.input_offset = 0;
    svdf_params.output_offset = 0;
    svdf_params.rank = 1;

    printf("\r\nStart SVDFunctions tests\r\n");
    int8_t *buffer = new int8_t[SVD_SIZE];
    nmsis_nn_context ctx = {buffer, SVD_SIZE};
    riscv_svdf_s8_ref(&ctx, &input_ctx, &output_ctx, &svdf_params, &input_quant_params, &output_quant_params,
                        &svdf_input_dims, test1, &state_dims,
                       svdf_state_ref, &weights_feature_dims, test1 + SVD_SIZE, &weights_time_dims,
                       test1 + 2 * SVD_SIZE, &svdf_bias_dims, NULL, &svdf_output_dims, output_q7);
    BENCH_START(riscv_svdf_s8);
    riscv_svdf_s8(&ctx, &input_ctx, &output_ctx, &svdf_params, &input_quant_params, &output_quant_params,
                        &svdf_input_dims, test1, &state_dims,
                       svdf_state_opt, &weights_feature_dims, test1 + SVD_SIZE, &weights_time_dims,
                       test1 + 2 * SVD_SIZE, &svdf_bias_dims, NULL, &svdf_output_dims, output_q7 + SVD_SIZE);
    BENCH_END(riscv_svdf_s8);
    verify_results_q7(output_q7, output_q7 + SVD_SIZE, 8 * 8);

    delete[] svdf_state_ref;
    delete[] svdf_state_opt;

    q15_t *state_ref = new q15_t[SVD_SIZE];
    q15_t *state_opt = new q15_t[SVD_SIZE];
    for (int i = 0; i < SVD_SIZE; i++) {
        state_ref[i] = (rand() % 65536 - 32768);
        state_opt[i] = state_ref[i];
    }

    memset(output_q7, 0, sizeof(q7_t) * 64);
    memset(output_q7 + SVD_SIZE, 0, sizeof(q7_t) * 64);
    riscv_svdf_state_s16_s8_ref(
        &input_ctx, &output_ctx, &svdf_params, &input_quant_params,
        &output_quant_params, &svdf_input_dims, test1, &state_dims, state_ref,
        &weights_feature_dims, test1 + SVD_SIZE, &weights_time_dims, test2,
        &svdf_bias_dims, NULL, &svdf_output_dims, output_q7);
    BENCH_START(riscv_svdf_state_s16_s8)
    riscv_svdf_state_s16_s8(
        &input_ctx, &output_ctx, &svdf_params, &input_quant_params,
        &output_quant_params, &svdf_input_dims, test1, &state_dims, state_opt,
        &weights_feature_dims, test1 + SVD_SIZE, &weights_time_dims, test2,
        &svdf_bias_dims, NULL, &svdf_output_dims, output_q7 + SVD_SIZE);
    BENCH_END(riscv_svdf_state_s16_s8)
    verify_results_q7(output_q7, output_q7 + SVD_SIZE, 8 * 8);

    delete[] state_ref;
    delete[] state_opt;

    delete[] buffer;
    delete[] scratch_buf;
    }
#endif

#ifdef TEST_NNDataConversion
    #define NNDataConversion_SIZE 100

    printf("\r\nStart TEST_NNDataConversion tests\r\n");

    riscv_q7_to_q15_no_shift_ref(test1, output_q15, NNDataConversion_SIZE);
    BENCH_START(riscv_q7_to_q15_no_shift);
    riscv_q7_to_q15_no_shift(test1, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE);
    BENCH_END(riscv_q7_to_q15_no_shift);
    verify_results_q15(output_q15, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE);

// Only available for DSP_ON
#if defined(RISCV_MATH_DSP)
    riscv_q7_to_q15_reordered_no_shift_ref(test1, output_q15, NNDataConversion_SIZE);
    BENCH_START(riscv_q7_to_q15_reordered_no_shift);
    riscv_q7_to_q15_reordered_no_shift(test1, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE);
    BENCH_END(riscv_q7_to_q15_reordered_no_shift);
    verify_results_q15(output_q15, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE);

    riscv_q7_to_q15_reordered_with_offset_ref(test1, output_q15, NNDataConversion_SIZE,0);
    BENCH_START(riscv_q7_to_q15_reordered_with_offset);
    riscv_q7_to_q15_reordered_with_offset(test1, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE,0);
    BENCH_END(riscv_q7_to_q15_reordered_with_offset);

    //verify_results_q15(output_q15, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE);
#endif
    q15_t offset_q15 = (rand() % 65536 - 32768);

    riscv_q7_to_q15_with_offset_ref(test1, output_q15, NNDataConversion_SIZE, offset_q15);
    BENCH_START(riscv_q7_to_q15_with_offset);
    riscv_q7_to_q15_with_offset(test1, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE, offset_q15);
    BENCH_END(riscv_q7_to_q15_with_offset);
    verify_results_q15(output_q15, output_q15 + NNDataConversion_SIZE, NNDataConversion_SIZE);
#endif

#ifdef TEST_BasicMathforNN
    #define BasicMathforNN_SIZE 1000
    printf("\r\nStart BasicMath tests\r\n");
    // Test q7
    riscv_nn_mult_q7_ref(test1, test1 + BasicMathforNN_SIZE, output_q7, 5, BasicMathforNN_SIZE);
    BENCH_START(riscv_nn_mult_q7);
    riscv_nn_mult_q7(test1, test1 + BasicMathforNN_SIZE, output_q7 + BasicMathforNN_SIZE, 5, BasicMathforNN_SIZE);
    BENCH_END(riscv_nn_mult_q7);
    verify_results_q7(output_q7, output_q7 + BasicMathforNN_SIZE, BasicMathforNN_SIZE);

    // Test q15
    riscv_nn_mult_q15_ref(test2, test2 + BasicMathforNN_SIZE, output_q15 + BasicMathforNN_SIZE, 13, BasicMathforNN_SIZE);
    BENCH_START(riscv_nn_mult_q15);
    riscv_nn_mult_q15(test2, test2 + BasicMathforNN_SIZE, output_q15, 13, BasicMathforNN_SIZE);
    BENCH_END(riscv_nn_mult_q15);
    verify_results_q15(output_q15, output_q15 + BasicMathforNN_SIZE, BasicMathforNN_SIZE);

#if defined(RISCV_MATH_DSP) || defined(RISCV_MATH_VECTOR)
    memset(output_q15, 0, 2 * BasicMathforNN_SIZE * sizeof(q15_t));
    riscv_nn_accumulate_q7_to_q15_ref(output_q15, test1, BasicMathforNN_SIZE);
    BENCH_START(riscv_nn_accumulate_q7_to_q15);
    riscv_nn_accumulate_q7_to_q15(output_q15 + BasicMathforNN_SIZE, test1, BasicMathforNN_SIZE);
    BENCH_END(riscv_nn_accumulate_q7_to_q15);
    verify_results_q15(output_q15, output_q15 + BasicMathforNN_SIZE, BasicMathforNN_SIZE);
#endif

    q31_t output_opt = 0, output_ref = 0;
    riscv_nn_add_q7_ref(test1, &output_ref, BasicMathforNN_SIZE);
    BENCH_START(riscv_nn_add_q7);
    riscv_nn_add_q7(test1, &output_opt, BasicMathforNN_SIZE);
    BENCH_END(riscv_nn_add_q7);
    verify_results_int32(&output_ref, &output_opt, 1);

//Only available for RISCV_MATH_MVEI
    // printf("Start ref q7 add implementation\n");

    // BENCH_START(riscv_nn_depthwise_conv_nt_t_padded_s8_ref);
    // riscv_nn_depthwise_conv_nt_t_padded_s8_ref(test1, &output_ref, BasicMathforNN_SIZE);
    // BENCH_END(riscv_nn_depthwise_conv_nt_t_padded_s8_ref);

    // printf("Start opt q7 add implementation\n");

    // BENCH_START(riscv_nn_depthwise_conv_nt_t_padded_s8);
    // riscv_nn_depthwise_conv_nt_t_padded_s8(test1, &output_opt, BasicMathforNN_SIZE);
    // BENCH_END(riscv_nn_depthwise_conv_nt_t_padded_s8);

    // verify_results_int32(&output_ref, &output_opt, 1);

    // printf("Start ref q7 add implementation\n");

    // BENCH_START(riscv_nn_depthwise_conv_nt_t_s8_ref);
    // riscv_nn_depthwise_conv_nt_t_s8_ref(test1, &output_ref, BasicMathforNN_SIZE);
    // BENCH_END(riscv_nn_depthwise_conv_nt_t_s8_ref);

    // printf("Start opt q7 add implementation\n");

    // BENCH_START(riscv_nn_depthwise_conv_nt_t_s8);
    // riscv_nn_depthwise_conv_nt_t_s8(test1, &output_opt, BasicMathforNN_SIZE);
    // BENCH_END(riscv_nn_depthwise_conv_nt_t_s8);

    // verify_results_int32(&output_ref, &output_opt, 1);

#if 0
    int32_t sum_col_opt = 0, output_opt_s8[4] = {0};
    int32_t sum_col_ref = 0, output_ref_s8[4] = {0};

    riscv_nn_mat_mul_core_1x_s8_ref(BasicMathforNN_SIZE, test1, test1 + BasicMathforNN_SIZE, &sum_col_ref, &output_ref_s8[0]);
    BENCH_START(riscv_nn_mat_mul_core_1x_s8);
    riscv_nn_mat_mul_core_1x_s8(BasicMathforNN_SIZE, test1, test1 + BasicMathforNN_SIZE, &sum_col_opt, &output_opt_s8[0]);
    BENCH_END(riscv_nn_mat_mul_core_1x_s8);
    verify_results_int32(&sum_col_ref, &sum_col_opt, 1);
    verify_results_int32(output_ref_s8, output_opt_s8, 1);
#endif

//Only available for RISCV_MATH_MVEI
    // printf("Start ref q7 add implementation\n");

    // BENCH_START(riscv_nn_mat_mul_core_4x_s8_ref);
    // riscv_nn_mat_mul_core_4x_s8_ref(BasicMathforNN_SIZE, 0, test1, test1 + BasicMathforNN_SIZE, &sum_col_ref, &output_ref_s8[0]);
    // //riscv_nn_mat_mul_core_4x_s8_ref(BasicMathforNN_SIZE, 0, test1, test1+BasicMathforNN_SIZE, (int32_t *const)&sum_col_ref,&output_ref_s8[0]);
    // BENCH_END(riscv_nn_mat_mul_core_4x_s8_ref);

    // printf("Start opt q7 add implementation\n");

    // BENCH_START(riscv_nn_mat_mul_core_4x_s8);
    // riscv_nn_mat_mul_core_4x_s8(BasicMathforNN_SIZE, 0, test1, test1+BasicMathforNN_SIZE, (int32_t *const)&sum_col_opt,&output_opt_s8[0]);
    // BENCH_END(riscv_nn_mat_mul_core_4x_s8);

    // verify_results_int32(&sum_col_opt, &sum_col_ref, 1);
    // verify_results_int32(output_opt_s8, output_ref_s8, 1);

    #define LHS_ROWS 30
    #define RHS_ROWS 30
    #define RHS_COLS 30
    #define LHS_OFFSET 0
    #define DST_OFFSET 0
    #define ACTIVATION_MIN -1000
    #define ACTIVATION_MAX 1000

    int32_t *bias_q31 = new int32_t[RHS_ROWS];
    int32_t *dst_multipliers = new int32_t[RHS_ROWS];
    int32_t *dst_shifts = new int32_t[RHS_ROWS];

    for (int i = 0; i < RHS_ROWS; i++) {
        bias_q31[i] = (rand() % 65536 - 32768);
        dst_multipliers[i] = rand() % 65536 + 0x400000;
        dst_shifts[i] = (rand() % 1);
    }

    riscv_nn_mat_mult_nt_t_s8_ref(test1, test1 + BasicMathforNN_SIZE, bias_q31, output_q7, dst_multipliers, dst_shifts,
                                  LHS_ROWS, RHS_ROWS, RHS_COLS, LHS_OFFSET, DST_OFFSET, ACTIVATION_MIN, ACTIVATION_MAX, 0, 4);

    BENCH_START(riscv_nn_mat_mult_nt_t_s8);
    riscv_nn_mat_mult_nt_t_s8(test1, test1 + BasicMathforNN_SIZE, bias_q31, output_q7 + LHS_ROWS * RHS_ROWS, dst_multipliers, dst_shifts,
                              LHS_ROWS, RHS_ROWS, RHS_COLS, LHS_OFFSET, DST_OFFSET, ACTIVATION_MIN, ACTIVATION_MAX, 0, 4);
    BENCH_END(riscv_nn_mat_mult_nt_t_s8);
    verify_results_q7(output_q7, output_q7 + LHS_ROWS * RHS_ROWS, LHS_ROWS * RHS_ROWS);

    initialize_results_q7(output_q7, output_q7 + BasicMathforNN_SIZE, BasicMathforNN_SIZE);

    #define ADDRESS_OFFSET 1
    #define RHS_OFFSET 1
    riscv_nn_vec_mat_mult_t_s8_ref(test1, test1 + BasicMathforNN_SIZE, NULL, bias_q31, output_q7, 0, 0,
                                   dst_multipliers[0], dst_shifts[0], RHS_COLS, RHS_ROWS, ACTIVATION_MIN, ACTIVATION_MAX, ADDRESS_OFFSET, RHS_OFFSET);

    BENCH_START(riscv_nn_vec_mat_mult_t_s8);
    riscv_nn_vec_mat_mult_t_s8(test1, test1 + BasicMathforNN_SIZE, NULL, bias_q31, output_q7 + RHS_ROWS * ADDRESS_OFFSET, 0, 0,
                                dst_multipliers[0], dst_shifts[0], RHS_COLS, RHS_ROWS, ACTIVATION_MIN, ACTIVATION_MAX, ADDRESS_OFFSET, RHS_OFFSET);
    BENCH_END(riscv_nn_vec_mat_mult_t_s8);

    verify_results_q7(output_q7, output_q7 + RHS_ROWS * ADDRESS_OFFSET, RHS_ROWS * ADDRESS_OFFSET);

    riscv_nn_vec_mat_mult_t_s4_ref(test1, test1 + BasicMathforNN_SIZE, bias_q31, output_q7, 0, 0,
                                dst_multipliers[0], dst_shifts[0], RHS_COLS, RHS_ROWS, ACTIVATION_MIN, ACTIVATION_MAX);
    BENCH_START(riscv_nn_vec_mat_mult_t_s4);
    riscv_nn_vec_mat_mult_t_s4(test1, test1 + BasicMathforNN_SIZE, bias_q31, output_q7 + RHS_ROWS * ADDRESS_OFFSET, 0, 0,
                                dst_multipliers[0], dst_shifts[0], RHS_COLS, RHS_ROWS, ACTIVATION_MIN, ACTIVATION_MAX);
    BENCH_END(riscv_nn_vec_mat_mult_t_s4);

    verify_results_q7(output_q7, output_q7 + RHS_ROWS * ADDRESS_OFFSET, RHS_ROWS * ADDRESS_OFFSET);

    #define BATCH_SIZE2 1
    #define BATCH_OFFSET2 10

    int64_t bias_q63[RHS_ROWS];

    for (int i = 0; i < RHS_ROWS; i++)
    {
        bias_q63[i] = (int64_t)(rand() % 0x7fffffffL - 0x7fffffffL / 2);
    }

    riscv_nn_vec_mat_mul_result_acc_s16_ref(test2, test1, bias_q63, output_q15 + BasicMathforNN_SIZE, dst_multipliers[0],
                                 dst_shifts[0], RHS_COLS, RHS_ROWS, BATCH_SIZE2, BATCH_OFFSET2);

    BENCH_START(riscv_nn_vec_mat_mul_result_acc_s16);
    riscv_nn_vec_mat_mul_result_acc_s16(test2, test1, bias_q63, output_q15, dst_multipliers[0],
                                 dst_shifts[0], RHS_COLS, RHS_ROWS, BATCH_SIZE2, BATCH_OFFSET2);
    BENCH_END(riscv_nn_vec_mat_mul_result_acc_s16);

    verify_results_q15(output_q15, output_q15 + BasicMathforNN_SIZE, RHS_ROWS * BATCH_SIZE2);

    delete[] bias_q31;
    delete[] dst_multipliers;
    delete[] dst_shifts;

#endif
    delete[] test1;
    delete[] test2;
    delete[] output_q7;
    delete[] output_q15;

    test_pass = true;
    test_index = 0;
    while (test_flags[test_index] != -1) {
        if (test_flags[test_index]) {
             test_pass = false;
        }
        test_index ++;
    }
    if (test_pass) {
        printf("All tests passed\n");
    } else {
        printf("Test failed\n");
    }

    if (test_pass) {
        return 0;
    } else {
        return 1;
    }
}
