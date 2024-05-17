//
// Created by lujun on 19-6-28.
//
// This contains f32, q31 and q15 versions of matrix
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

#define M 32
#define K 64
#define N 32

q31_t q31_a_array[M * K];
q31_t q31_b_array[K * N];
q31_t q31_temp_back[K * N];
q31_t q31_c_array[M * N];
q31_t q31_d_array[M * N];
q31_t q31_output[M * N];
q31_t q31_output_ref[M * N];
q31_t q31_b_vec[K];

q31_t q31_output_1[M * N * 2];
q31_t q31_output_ref_1[M * N * 2];

int test_flag_error = 0;

BENCH_DECLARE_VAR();
int DSP_matrix_q31(void)
{
    int8_t s;
    riscv_matrix_instance_q31 q31_A;
    riscv_matrix_instance_q31 q31_B;
    riscv_matrix_instance_q31 q31_ref;
    riscv_matrix_instance_q31 q31_des;

    q31_t q31_ref_vec[M];
    q31_t q31_dst_vec[M];

    riscv_mat_init_q31(&q31_A, M, K, (q31_t *)q31_a_array);
    riscv_mat_init_q31(&q31_B, K, N, (q31_t *)q31_b_array);
    riscv_mat_init_q31(&q31_des, M, N, q31_output);
    riscv_mat_init_q31(&q31_ref, M, N, q31_output_ref);

    // In functions riscv_mat_mult_opt_q31,riscv_mat_mult_q31 and riscv_mat_mult_fast_q31,
    // The input is thus scaled down by log2(numColsA) bits to avoid overflows,
    // as a total of numColsA additions are performed internally.
    for (int i = 0; i < M * K; i++) {
        q31_a_array[i] = (q31_t)(rand() % Q31_MAX - Q31_MAX / 2) / K;
    }

    for (int i = 0; i < K * N; i++) {
        q31_b_array[i] = (q31_t)(rand() % Q31_MAX - Q31_MAX / 2) / K;
    }

    for (int i = 0; i < K; i++) {
        q31_b_vec[i] = (q31_t)(rand() % Q31_MAX - Q31_MAX / 2) / K;
    }

    // ****************   q31   *********************
    // mat_vec_mult
    BENCH_START(riscv_mat_vec_mult_q31);
    riscv_mat_vec_mult_q31(&q31_A, q31_b_vec, q31_dst_vec);
    BENCH_END(riscv_mat_vec_mult_q31);
    ref_mat_vec_mult_q31(&q31_A, q31_b_vec, q31_ref_vec);
    s = verify_results_q31(q31_ref_vec, q31_dst_vec, M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_vec_mult_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_vec_mult_q31);

    // mult
    BENCH_START(riscv_mat_mult_q31);
    riscv_mat_mult_q31(&q31_A, &q31_B, &q31_des);
    BENCH_END(riscv_mat_mult_q31);
    ref_mat_mult_q31(&q31_A, &q31_B, &q31_ref);
    s = verify_results_q31(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_q31);

    // mult_fast
    BENCH_START(riscv_mat_mult_fast_q31);
    riscv_mat_mult_fast_q31(&q31_A, &q31_B, &q31_des);
    BENCH_END(riscv_mat_mult_fast_q31);
    ref_mat_mult_fast_q31(&q31_A, &q31_B, &q31_ref);
    s = verify_results_q31_great_precision(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_fast_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_fast_q31);

    // mult_opt
    BENCH_START(riscv_mat_mult_opt_q31);
    riscv_mat_mult_opt_q31(&q31_A, &q31_B, &q31_des, q31_temp_back);
    BENCH_END(riscv_mat_mult_opt_q31);
    ref_mat_mult_opt_q31(&q31_A, &q31_B, &q31_ref);
    s = verify_results_q31(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_opt_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_opt_q31);

    riscv_mat_init_q31(&q31_A, M, N, (q31_t *)q31_c_array);
    riscv_mat_init_q31(&q31_B, M, N, (q31_t *)q31_d_array);
    riscv_mat_init_q31(&q31_des, M, N, q31_output);
    riscv_mat_init_q31(&q31_ref, M, N, q31_output_ref);
    generate_rand_q31(q31_c_array, M * N);
    generate_rand_q31(q31_d_array, M * N);
    // add
    BENCH_START(riscv_mat_add_q31);
    riscv_mat_add_q31(&q31_A, &q31_B, &q31_des);
    BENCH_END(riscv_mat_add_q31);
    ref_mat_add_q31(&q31_A, &q31_B, &q31_ref);
    s = verify_results_q31(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_add_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_add_q31);

    // sub
    BENCH_START(riscv_mat_sub_q31);
    riscv_mat_sub_q31(&q31_A, &q31_B, &q31_des);
    BENCH_END(riscv_mat_sub_q31);
    ref_mat_sub_q31(&q31_A, &q31_B, &q31_ref);
    s = verify_results_q31(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_sub_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_sub_q31);

    // trans
    BENCH_START(riscv_mat_trans_q31);
    riscv_mat_trans_q31(&q31_A, &q31_des);
    BENCH_END(riscv_mat_trans_q31);
    ref_mat_trans_q31(&q31_A, &q31_ref);
    s = verify_results_q31(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_trans_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_trans_q31);

    // scale
    #define    SCALE 64
    #define    RIGHT_SHIFT 2
    BENCH_START(riscv_mat_scale_q31);
    riscv_mat_scale_q31(&q31_A, SCALE, RIGHT_SHIFT, &q31_des);
    BENCH_END(riscv_mat_scale_q31);
    ref_mat_scale_q31(&q31_A, SCALE, RIGHT_SHIFT, &q31_ref);
    s = verify_results_q31(q31_output_ref, q31_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_scale_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_scale_q31);

    // cmplx_mult
    for (int i = 0; i < M * K; i++) {
        q31_a_array[i] = (q31_t)(rand() % (Q31_MAX / 2) - Q31_MAX / 4);
    }
    for (int i = 0; i < K * N; i++) {
        q31_b_array[i] = (q31_t)(rand() % (Q31_MAX / 2) - Q31_MAX / 4);
    }
    riscv_mat_init_q31(&q31_A, M, K / 2, (q31_t *)q31_a_array);
    riscv_mat_init_q31(&q31_B, K / 2, N, (q31_t *)q31_b_array);
    riscv_mat_init_q31(&q31_des, M, N, q31_output_1);
    riscv_mat_init_q31(&q31_ref, M, N, q31_output_ref_1);
    BENCH_START(riscv_mat_cmplx_mult_q31);
    riscv_mat_cmplx_mult_q31(&q31_A, &q31_B, &q31_des);
    BENCH_END(riscv_mat_cmplx_mult_q31);
    ref_mat_cmplx_mult_q31(&q31_A, &q31_B, &q31_ref);
    s = verify_results_q31(q31_output_ref_1, q31_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cmplx_mult_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cmplx_mult_q31);

}

int main(void)
{
    BENCH_INIT();
    DSP_matrix_q31();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
