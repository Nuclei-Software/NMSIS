//
// Created by lujun on 19-6-28.
//
// This contains f16, q31 and q15 versions of matrix
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include <stdint.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#define M 32
#define K 64
#define N 32

int test_flag_error = 0;

#if defined (RISCV_FLOAT16_SUPPORTED)
float16_t f16_a_array[M * K];
float16_t f16_b_array[K * N];
float16_t f16_output[M * N];
float16_t f16_output_ref[M * N];
float16_t f16_B_vec[K];
float16_t f16_ref_vec[M];
float16_t f16_dst_vec[M];

float16_t f16_c_array[M * N];
float16_t f16_d_array[M * N];

float16_t f16_e_array[M * M];
float16_t f16_f_array[M * N];
float16_t f16_output_1[M * N * 2];
float16_t f16_output_ref_1[M * N * 2];

float16_t f16_output_2[M * M];
float16_t f16_output_ref_2[M * M];
float16_t f16_posi_array[M * M];
float16_t f16_dot_array[M * M];
float16_t f16_tmp_array[M * M] = {0};

float16_t f16_g_array[M * M];


BENCH_DECLARE_VAR();

int DSP_matrix_f16(void)
{
    int8_t s;
    int8_t s1, s2, s3;
    riscv_matrix_instance_f16 f16_A;
    riscv_matrix_instance_f16 f16_B;
    riscv_matrix_instance_f16 f16_ref;
    riscv_matrix_instance_f16 f16_des;
    riscv_matrix_instance_f16 f16_back;

    /* mat_vec_mult */
    riscv_mat_init_f16(&f16_A, M, K, (float16_t *)f16_a_array);
    riscv_mat_init_f16(&f16_B, K, N, (float16_t *)f16_b_array);
    riscv_mat_init_f16(&f16_des, M, N, f16_output);
    riscv_mat_init_f16(&f16_ref, M, N, f16_output_ref);
    generate_rand_f16(f16_a_array, M * K);
    generate_rand_f16(f16_b_array, K * N);
    generate_rand_f16(f16_B_vec, K);

    BENCH_START(riscv_mat_vec_mult_f16);
    riscv_mat_vec_mult_f16(&f16_A, f16_B_vec, f16_dst_vec);
    BENCH_END(riscv_mat_vec_mult_f16);
    ref_mat_vec_mult_f16(&f16_A, f16_B_vec, f16_ref_vec);
    s = verify_results_f16(f16_ref_vec, f16_dst_vec, M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_vec_mult_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_vec_mult_f16);

    // mult
    BENCH_START(riscv_mat_mult_f16);
    riscv_mat_mult_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_mult_f16);
    ref_mat_mult_f16(&f16_A, &f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref, f16_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_f16);

    riscv_mat_init_f16(&f16_A, M, N, (float16_t *)f16_c_array);
    riscv_mat_init_f16(&f16_B, M, N, (float16_t *)f16_d_array);
    riscv_mat_init_f16(&f16_des, M, N, f16_output);
    riscv_mat_init_f16(&f16_ref, M, N, f16_output_ref);
    generate_rand_f16(f16_c_array, M * N);
    generate_rand_f16(f16_d_array, M * N);
    // add
    BENCH_START(riscv_mat_add_f16);
    riscv_mat_add_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_add_f16);
    ref_mat_add_f16(&f16_A, &f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref, f16_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_add_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_add_f16);
    // sub
    BENCH_START(riscv_mat_sub_f16);
    riscv_mat_sub_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_sub_f16);
    ref_mat_sub_f16(&f16_A, &f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref, f16_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_sub_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_sub_f16);

    // trans
    BENCH_START(riscv_mat_trans_f16);
    riscv_mat_trans_f16(&f16_A, &f16_des);
    BENCH_END(riscv_mat_trans_f16);
    ref_mat_trans_f16(&f16_A, &f16_ref);
    s = verify_results_f16(f16_output_ref, f16_output, N * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_trans_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_trans_f16);
    // scale
    #define    SCALE 80.0f16
    BENCH_START(riscv_mat_scale_f16);
    riscv_mat_scale_f16(&f16_A, SCALE, &f16_des);
    BENCH_END(riscv_mat_scale_f16);
    ref_mat_scale_f16(&f16_A, SCALE, &f16_ref);
    s = verify_results_f16(f16_output_ref, f16_output, N * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_scale_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_scale_f16);

    // cmplx_mult
    riscv_mat_init_f16(&f16_A, M, K / 2, (float16_t *)f16_a_array);
    riscv_mat_init_f16(&f16_B, K / 2, N, (float16_t *)f16_b_array);
    riscv_mat_init_f16(&f16_des, M, N, f16_output_1);
    riscv_mat_init_f16(&f16_ref, M, N, f16_output_ref_1);
    BENCH_START(riscv_mat_cmplx_mult_f16);
    riscv_mat_cmplx_mult_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_cmplx_mult_f16);
    ref_mat_cmplx_mult_f16(&f16_A, &f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref_1, f16_output_1, M * N * 2);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cmplx_mult_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cmplx_mult_f16);

    riscv_mat_init_f16(&f16_A, M, M, (float16_t *)f16_e_array);
    riscv_mat_init_f16(&f16_B, M, N, (float16_t *)f16_f_array);

    riscv_mat_init_f16(&f16_des, M, N, f16_output_1);
    riscv_mat_init_f16(&f16_ref, M, N, f16_output_ref_1);
    // solve upper triangular matrix
    // initialize upper triangular matrix by setting 0 to lower elements in matrix
    for (int i = 0; i < M * N; i++) {
        f16_e_array[i] = (float16_t)rand();
    }
    for (int i = 0; i < M; i++) {
        for(int j = 0; j < i; j++) {
            f16_e_array[i * M + j] = 0;
        }
    }
    for (int i = 0; i < M * N; i++) {
        f16_f_array[i] = (float16_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
    memset(f16_output_1, 0, sizeof(float16_t) * M * N);
    // ensure f16_output_ref_1[i] = f16_output_1[i], i = 0, 1, 2, ...
    memcpy(f16_output_1, f16_output_ref_1, sizeof(float16_t) * M * N);
    BENCH_START(riscv_mat_solve_upper_triangular_f16);
    riscv_mat_solve_upper_triangular_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_solve_upper_triangular_f16);
    ref_mat_solve_upper_triangular_f16(&f16_A, &f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref_1, f16_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_solve_upper_triangular_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_solve_upper_triangular_f16);

    // solve lower triangular matrix
    // initialize lower triangular matrix by setting 0 to upper elements in matrix
    for (int i=0; i < M; i++) {
        for (int j = M - 1; j > i; j--) {
            f16_e_array[i * M + j] = 0;
        }
        for (int j = 0; j <= i; j++)
            f16_e_array[i * M + j] = (float16_t)rand();
    }
    // ensure f16_output_ref_1[i] = f16_output_1[i], i = 0, 1, 2, ...
    BENCH_START(riscv_mat_solve_lower_triangular_f16);
    riscv_mat_solve_lower_triangular_f16(&f16_A, &f16_B, &f16_des);
    BENCH_END(riscv_mat_solve_lower_triangular_f16);
    ref_mat_solve_lower_triangular_f16(&f16_A, &f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref_1, f16_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_solve_lower_triangular_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_solve_lower_triangular_f16);

    // cholesky
    float16_t tmp = (float16_t)(rand() % Q31_MAX) / Q31_MAX;
    riscv_matrix_instance_f16 f16_posi;
    riscv_matrix_instance_f16 f16_dot;
    riscv_matrix_instance_f16 f16_tmp;
    riscv_mat_init_f16(&f16_des, M, M, f16_output_2);
    riscv_mat_init_f16(&f16_ref, M, M, f16_output_ref_2);
    riscv_mat_init_f16(&f16_posi, M, M, f16_posi_array);
    riscv_mat_init_f16(&f16_dot, M, M, f16_dot_array);
    riscv_mat_init_f16(&f16_tmp, M, M, f16_tmp_array);
    generate_rand_f16(f16_e_array, M * M);
    generate_rand_f16(f16_output_2, M * M);
    // ensure f16_output_ref_1[i] = f16_output_1[i], i = 0, 1, 2, ...
    memcpy(f16_output_2, f16_output_ref_2, sizeof(float16_t) * M * M);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (i == j) {
                f16_tmp_array[i * M + j] = tmp;
            }
        }
    }
    generate_posi_def_symme_f16(&f16_A, &f16_tmp, &f16_dot, &f16_posi);
    BENCH_START(riscv_mat_cholesky_f16);
    riscv_mat_cholesky_f16(&f16_posi, &f16_des);
    BENCH_END(riscv_mat_cholesky_f16);
    ref_mat_cholesky_f16(&f16_posi, &f16_ref);
    s = 0;
    /* result is lower triangular matrix */
    for (int i = 0 ; i < M ; i++) {
      for (int j = i ; j < M ; j++) {
         if (fabs(f16_output_ref_2[j * M + i] - f16_output_2[j * M + i]) > DELTAF64 ) {
           printf("i%u-j%u mismatch %f vs %f\n", i, j, f16_output_ref_2[j * M + i], f16_output_2[j * M + i]);
           s = 1; /* fail */
         }
      }
    }
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cholesky_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cholesky_f16);

    // inverse
    riscv_mat_init_f16(&f16_B, M, M, (float16_t *)f16_g_array);
    memcpy(f16_g_array, f16_e_array, sizeof(f16_e_array));
    memcpy(f16_output_ref_2, f16_output_2, sizeof(f16_output_2));
    BENCH_START(riscv_mat_inverse_f16);
    riscv_mat_inverse_f16(&f16_A, &f16_des);
    BENCH_END(riscv_mat_inverse_f16);
    ref_mat_inverse_f16(&f16_B, &f16_ref);
    s = verify_results_f16(f16_output_ref_2, f16_output_2, M * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_inverse_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_inverse_f16);
}

#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

int main(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    BENCH_INIT();
    DSP_matrix_f16();
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
