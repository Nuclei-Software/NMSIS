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
q15_t q15_a_array[M * K];
q15_t q15_b_array[K * N];

q15_t q15_c_array[M * N];
q15_t q15_d_array[M * N];

q15_t q15_output[M * N];
q15_t q15_output_ref[M * N];
q15_t q15_output_back[K * N];
q15_t q15_b_vec[K];

q15_t q15_output_1[M * N * 2];
q15_t q15_output_ref_1[M * N * 2];

int test_flag_error = 0;

BENCH_DECLARE_VAR();
int DSP_matrix_q15(void)
{
    int8_t s;
    riscv_matrix_instance_q15 q15_A;
    riscv_matrix_instance_q15 q15_B;
    riscv_matrix_instance_q15 q15_ref;
    riscv_matrix_instance_q15 q15_des;

    q15_t q15_ref_vec[M];
    q15_t q15_dst_vec[M];

    // ****************   q15   *********************

    riscv_mat_init_q15(&q15_A, M, K, (q15_t *)q15_a_array);
    riscv_mat_init_q15(&q15_B, K, N, (q15_t *)q15_b_array);
    riscv_mat_init_q15(&q15_des, M, N, q15_output);
    riscv_mat_init_q15(&q15_ref, M, N, q15_output_ref);
    for (int i = 0; i < M * K; i++) {
        q15_a_array[i] = (q15_t)(rand() % (Q15_MAX / 2 ) - Q15_MAX / 4);
    }
    for (int i = 0; i < K * N; i++) {
        q15_b_array[i] = (q15_t)(rand() % (Q15_MAX / 2 ) - Q15_MAX / 4);
    }
    for (int i = 0; i < K; i++) {
        q15_b_vec[i] = (q15_t)(rand() % (Q15_MAX / 2 ) - Q15_MAX / 4);
    }
    // ****************   q15   *********************
    // mat_vec_mult
    BENCH_START(riscv_mat_vec_mult_q15);
    riscv_mat_vec_mult_q15(&q15_A, q15_b_vec, q15_dst_vec);
    BENCH_END(riscv_mat_vec_mult_q15);
    ref_mat_vec_mult_q15(&q15_A, q15_b_vec, q15_ref_vec);
    s = verify_results_q15(q15_ref_vec, q15_dst_vec, M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_vec_mult_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_vec_mult_q15);

    // mult
    BENCH_START(riscv_mat_mult_q15);
    riscv_mat_mult_q15(&q15_A, &q15_B, &q15_des, q15_output_back);
    BENCH_END(riscv_mat_mult_q15);
    ref_mat_mult_q15(&q15_A, &q15_B, &q15_ref);
    s = verify_results_q15(q15_output_ref, q15_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_q15);

    // mult_fast
    BENCH_START(riscv_mat_mult_fast_q15);
    riscv_mat_mult_fast_q15(&q15_A, &q15_B, &q15_des, q15_output_back);
    BENCH_END(riscv_mat_mult_fast_q15);
    ref_mat_mult_fast_q15(&q15_A, &q15_B, &q15_ref);
    s = verify_results_q15(q15_output_ref, q15_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_fast_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_fast_q15);

    riscv_mat_init_q15(&q15_A, M, N, (q15_t *)q15_c_array);
    riscv_mat_init_q15(&q15_B, M, N, (q15_t *)q15_d_array);
    riscv_mat_init_q15(&q15_des, M, N, q15_output);
    riscv_mat_init_q15(&q15_ref, M, N, q15_output_ref);
    generate_rand_q15(q15_c_array, M * N);
    generate_rand_q15(q15_d_array, M * N);
    // add
    BENCH_START(riscv_mat_add_q15);
    riscv_mat_add_q15(&q15_A, &q15_B, &q15_des);
    BENCH_END(riscv_mat_add_q15);
    ref_mat_add_q15(&q15_A, &q15_B, &q15_ref);
    s = verify_results_q15(q15_output_ref, q15_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_add_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_add_q15);

    // sub
    BENCH_START(riscv_mat_sub_q15);
    riscv_mat_sub_q15(&q15_A, &q15_B, &q15_des);
    BENCH_END(riscv_mat_sub_q15);
    ref_mat_sub_q15(&q15_A, &q15_B, &q15_ref);
    s = verify_results_q15(q15_output_ref, q15_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_sub_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_mat_sub_q15);

    // trans
    BENCH_START(riscv_mat_trans_q15);
    riscv_mat_trans_q15(&q15_A, &q15_des);
    BENCH_END(riscv_mat_trans_q15);
    ref_mat_trans_q15(&q15_A, &q15_ref);
    s = verify_results_q15(q15_output_ref, q15_output, N * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_trans_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_trans_q15);

    // scale
    #define    SCALE 32
    #define    RIGHT_SHIFT 2
    BENCH_START(riscv_mat_scale_q15);
    riscv_mat_scale_q15(&q15_A, SCALE, RIGHT_SHIFT, &q15_des);
    BENCH_END(riscv_mat_scale_q15);
    ref_mat_scale_q15(&q15_A, SCALE, RIGHT_SHIFT, &q15_ref);
    s = verify_results_q15(q15_output_ref, q15_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_scale_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_scale_q15);

    // cmplx_mult
    riscv_mat_init_q15(&q15_A, M, K / 2, (q15_t *)q15_a_array);
    riscv_mat_init_q15(&q15_B, K / 2, N, (q15_t *)q15_b_array);
    riscv_mat_init_q15(&q15_des, M, N, q15_output_1);
    riscv_mat_init_q15(&q15_ref, M, N, q15_output_ref_1);

    BENCH_START(riscv_mat_cmplx_mult_q15);
    riscv_mat_cmplx_mult_q15(&q15_A, &q15_B, &q15_des, q15_output_back);
    BENCH_END(riscv_mat_cmplx_mult_q15);
    ref_mat_cmplx_mult_q15(&q15_A, &q15_B, &q15_ref);
    s = verify_results_q15(q15_output_ref_1, q15_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cmplx_mult_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cmplx_mult_q15);
}

int main()
{
    BENCH_INIT();

    DSP_matrix_q15();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
