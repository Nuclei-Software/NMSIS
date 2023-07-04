//
// Created by lujun on 19-6-28.
//
// This contains f32, q31 and q15 versions of matrix
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
BENCH_DECLARE_VAR();

int DSP_matrix_f32(void)
{
    int8_t s;
    int8_t s1, s2, s3;
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_B;
    riscv_matrix_instance_f32 f32_ref;
    riscv_matrix_instance_f32 f32_des;
    riscv_matrix_instance_f32 f32_back;

    /* mat_vec_mult */
    float32_t f32_a_array[M * K];
    float32_t f32_b_array[K * N];
    float32_t f32_output[M * N];
    float32_t f32_output_ref[M * N];
    float32_t f32_B_vec[K];
    float32_t f32_ref_vec[M];
    float32_t f32_dst_vec[M];

    riscv_mat_init_f32(&f32_A, M, K, (float32_t *)f32_a_array);
    riscv_mat_init_f32(&f32_B, K, N, (float32_t *)f32_b_array);
    riscv_mat_init_f32(&f32_des, M, N, f32_output);
    riscv_mat_init_f32(&f32_ref, M, N, f32_output_ref);
    generate_rand_f32(f32_a_array, M * K);
    generate_rand_f32(f32_b_array, K * N);
    generate_rand_f32(f32_B_vec, K);

    BENCH_START(riscv_mat_vec_mult_f32);
    riscv_mat_vec_mult_f32(&f32_A, f32_B_vec, f32_dst_vec);
    BENCH_END(riscv_mat_vec_mult_f32);
    ref_mat_vec_mult_f32(&f32_A, f32_B_vec, f32_ref_vec);
    s = verify_results_f32(f32_ref_vec, f32_dst_vec, M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_vec_mult_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_vec_mult_f32);

    // mult
    BENCH_START(riscv_mat_mult_f32);
    riscv_mat_mult_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_mult_f32);
    ref_mat_mult_f32(&f32_A, &f32_B, &f32_ref);
    s = verify_results_f32(f32_output_ref, f32_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_f32);

    float32_t f32_c_array[M * N];
    float32_t f32_d_array[M * N];
    riscv_mat_init_f32(&f32_A, M, N, (float32_t *)f32_c_array);
    riscv_mat_init_f32(&f32_B, M, N, (float32_t *)f32_d_array);
    riscv_mat_init_f32(&f32_des, M, N, f32_output);
    riscv_mat_init_f32(&f32_ref, M, N, f32_output_ref);
    generate_rand_f32(f32_c_array, M * N);
    generate_rand_f32(f32_d_array, M * N);
    // add
    BENCH_START(riscv_mat_add_f32);
    riscv_mat_add_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_add_f32);
    ref_mat_add_f32(&f32_A, &f32_B, &f32_ref);
    s = verify_results_f32(f32_output_ref, f32_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_add_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_add_f32);
    // sub
    BENCH_START(riscv_mat_sub_f32);
    riscv_mat_sub_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_sub_f32);
    ref_mat_sub_f32(&f32_A, &f32_B, &f32_ref);
    s = verify_results_f32(f32_output_ref, f32_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_sub_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_sub_f32);

    // trans
    BENCH_START(riscv_mat_trans_f32);
    riscv_mat_trans_f32(&f32_A, &f32_des);
    BENCH_END(riscv_mat_trans_f32);
    ref_mat_trans_f32(&f32_A, &f32_ref);
    s = verify_results_f32(f32_output_ref, f32_output, N * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_trans_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_trans_f32);
    // scale
    #define    SCALE 80.73f
    BENCH_START(riscv_mat_scale_f32);
    riscv_mat_scale_f32(&f32_A, SCALE, &f32_des);
    BENCH_END(riscv_mat_scale_f32);
    ref_mat_scale_f32(&f32_A, SCALE, &f32_ref);
    s = verify_results_f32(f32_output_ref, f32_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_scale_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_scale_f32);

    // cmplx_mult
    riscv_mat_init_f32(&f32_A, M, K / 2, (float32_t *)f32_a_array);
    riscv_mat_init_f32(&f32_B, K / 2, N / 2, (float32_t *)f32_b_array);
    riscv_mat_init_f32(&f32_des, M, N, f32_output);
    riscv_mat_init_f32(&f32_ref, M, N, f32_output_ref);
    BENCH_START(riscv_mat_cmplx_mult_f32);
    riscv_mat_cmplx_mult_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_cmplx_mult_f32);
    ref_mat_cmplx_mult_f32(&f32_A, &f32_B, &f32_ref);
    s = verify_results_f32(f32_output_ref, f32_output, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cmplx_mult_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cmplx_mult_f32);

    float32_t f32_e_array[M * M];
    float32_t f32_f_array[M * N];
    float32_t f32_output_1[M * N];
    float32_t f32_output_ref_1[M * N];

    riscv_mat_init_f32(&f32_A, M, M, (float32_t *)f32_e_array);
    riscv_mat_init_f32(&f32_B, M, N, (float32_t *)f32_f_array);

    riscv_mat_init_f32(&f32_des, M, N, f32_output_1);
    riscv_mat_init_f32(&f32_ref, M, N, f32_output_ref_1);
    // solve upper triangular matrix
    // initialize upper triangular matrix by setting 0 to lower elements in matrix
    for (int i = 0; i < M * N; i++) {
        f32_e_array[i] = (float32_t)rand();
    }
    for (int i = 0; i < M; i++) {
        for(int j = 0; j < i; j++) {
            f32_e_array[i * M + j] = 0;
        }
    }
    for (int i = 0; i < M * N; i++) {
        f32_f_array[i] = (float32_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
    memset(f32_output_1, 0, sizeof(float32_t) * M * N);
    // ensure f32_output_ref_1[i] = f32_output_1[i], i = 0, 1, 2, ...
    memcpy(f32_output_1, f32_output_ref_1, sizeof(float32_t) * M * N);
    BENCH_START(riscv_mat_solve_upper_triangular_f32);
    riscv_mat_solve_upper_triangular_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_solve_upper_triangular_f32);
    ref_mat_solve_upper_triangular_f32(&f32_A, &f32_B, &f32_ref);
    s = verify_results_f32_low_precision(f32_output_ref_1, f32_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_solve_upper_triangular_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_solve_upper_triangular_f32);

    // solve lower triangular matrix
    // initialize lower triangular matrix by setting 0 to upper elements in matrix
    for (int i=0; i < M; i++) {
        for (int j = M - 1; j > i; j--) {
            f32_e_array[i * M + j] = 0;
        }
        for (int j = 0; j <= i; j++)
            f32_e_array[i * M + j] = (float32_t)rand();
    }
    // ensure f32_output_ref_1[i] = f32_output_1[i], i = 0, 1, 2, ...
    BENCH_START(riscv_mat_solve_lower_triangular_f32);
    riscv_mat_solve_lower_triangular_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_solve_lower_triangular_f32);
    ref_mat_solve_lower_triangular_f32(&f32_A, &f32_B, &f32_ref);
    s = verify_results_f32_low_precision(f32_output_ref_1, f32_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_solve_lower_triangular_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_solve_lower_triangular_f32);

    // cholesky
    float32_t f32_output_2[M * M];
    float32_t f32_output_ref_2[M * M];
    float32_t f32_rand_array[M * M];
    float32_t f32_posi_array[M * M];
    float32_t f32_dot_array[M * M];
    float32_t f32_tmp_array[M * M] = {0};
    float32_t tmp = (float32_t)(rand() % Q31_MAX) / Q31_MAX;
    riscv_matrix_instance_f32 f32_rand;
    riscv_matrix_instance_f32 f32_posi;
    riscv_matrix_instance_f32 f32_dot;
    riscv_matrix_instance_f32 f32_tmp;
    riscv_mat_init_f32(&f32_des, M, M, f32_output_2);
    riscv_mat_init_f32(&f32_ref, M, M, f32_output_ref_2);
    riscv_mat_init_f32(&f32_rand, M, M, f32_rand_array);
    riscv_mat_init_f32(&f32_posi, M, M, f32_posi_array);
    riscv_mat_init_f32(&f32_dot, M, M, f32_dot_array);
    riscv_mat_init_f32(&f32_tmp, M, M, f32_tmp_array);
    generate_rand_f32(f32_e_array, M * M);
    generate_rand_f32(f32_output_2, M * M);
    // ensure f32_output_ref_1[i] = f32_output_1[i], i = 0, 1, 2, ...
    memcpy(f32_output_2, f32_output_ref_2, sizeof(float32_t) * M * M);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (i == j) {
                f32_tmp_array[i * M + j] = tmp;
            }
        }
    }
    generate_posi_def_symme_f32(&f32_A, &f32_tmp, &f32_dot, &f32_posi);
    BENCH_START(riscv_mat_cholesky_f32);
    riscv_mat_cholesky_f32(&f32_posi, &f32_des);
    BENCH_END(riscv_mat_cholesky_f32);
    ref_mat_cholesky_f32(&f32_posi, &f32_ref);
    s = 0;
    /* result is lower triangular matrix */
    for (int i = 0 ; i < M ; i++) {
      for (int j = i ; j < M ; j++) {
         if (fabs(f32_output_ref_2[j * M + i] - f32_output_2[j * M + i]) > DELTAF64 ) {
           printf("i%u-j%u mismatch %f vs %f\n", i, j, f32_output_ref_2[j * M + i], f32_output_2[j * M + i]);
           s = 1; /* fail */
         }
      }
    }
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cholesky_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cholesky_f32);

    // inverse
    float32_t f32_g_array[M * M];
    riscv_mat_init_f32(&f32_B, M, M, (float32_t *)f32_g_array);
    memcpy(f32_g_array, f32_e_array, sizeof(f32_e_array));
    memcpy(f32_output_ref_2, f32_output_2, sizeof(f32_output_2));
    BENCH_START(riscv_mat_inverse_f32);
    riscv_mat_inverse_f32(&f32_A, &f32_des);
    BENCH_END(riscv_mat_inverse_f32);
    ref_mat_inverse_f32(&f32_B, &f32_ref);
    s = verify_results_f32(f32_output_ref_2, f32_output_2, M * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_inverse_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_inverse_f32);

    // Initialize Symmetric Matrices
    uint16_t pp[M], pp_ref[M];
    float32_t f32_output_3[M * M];
    float32_t f32_output_ref_3[M * M];

    for(int i = 0; i < M; i++){
        for(int j = M - 1; j > i; j--){
            if (i == j) break;
            f32_e_array[i * M + j] = f32_e_array[j * M + i];
        }
    }
    riscv_mat_init_f32(&f32_B, M, M, (float32_t *)f32_output_3);
    riscv_mat_init_f32(&f32_back, M, M, f32_output_ref_3);
    BENCH_START(riscv_mat_ldlt_f32);
    riscv_mat_ldlt_f32(&f32_posi, &f32_B, &f32_des, pp);
    BENCH_END(riscv_mat_ldlt_f32);
    ref_mat_ldlt_f32(&f32_posi, &f32_back, &f32_ref, pp_ref);
    s1 = verify_results_f32_low_precision(f32_output_ref_2, f32_output_2, M * M);
    s2 = verify_results_f32_low_precision(f32_output_ref_3, f32_output_3, M * M);
    s3 = verify_results_q15((int16_t *)pp_ref, (int16_t *)pp, M);
    if (s1 != 0 || s2 != 0 || s3 != 0) {
        BENCH_ERROR(riscv_mat_ldlt_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_ldlt_f32);
}

void riscv_mat_init_f64(
  riscv_matrix_instance_f64 * S,
  uint16_t nRows,
  uint16_t nColumns,
  float64_t * pData)
{
  /* Assign Number of Rows */
  S->numRows = nRows;

  /* Assign Number of Columns */
  S->numCols = nColumns;

  /* Assign Data pointer */
  S->pData = pData;
}

int DSP_matrix_f64(void)
{
    int8_t s;
    int8_t s1, s2, s3;

    float64_t f64_a_array[M * M];
    float64_t f64_output[M * M];
    float64_t f64_output_ref[M * M];
    riscv_matrix_instance_f64 f64_A;
    riscv_matrix_instance_f64 f64_ref;
    riscv_matrix_instance_f64 f64_des;
    riscv_mat_init_f64(&f64_A, M, M, (float64_t *)f64_a_array);
    riscv_mat_init_f64(&f64_des, M, M, f64_output);
    riscv_mat_init_f64(&f64_ref, M, M, f64_output_ref);

    // float32_t type element used to generate positive definite symmetric matrix
    float32_t f32_rand_array[M * M];
    float32_t f32_posi_array[M * M];
    float32_t f32_dot_array[M * M];
    float32_t f32_tmp_array[M * M] = {0};
    float32_t tmp = (float32_t)(rand() % Q31_MAX) / Q31_MAX;
    riscv_matrix_instance_f32 f32_rand;
    riscv_matrix_instance_f32 f32_posi;
    riscv_matrix_instance_f32 f32_dot;
    riscv_matrix_instance_f32 f32_tmp;
    riscv_mat_init_f32(&f32_rand, M, M, f32_rand_array);
    riscv_mat_init_f32(&f32_posi, M, M, f32_posi_array);
    riscv_mat_init_f32(&f32_dot, M, M, f32_dot_array);
    riscv_mat_init_f32(&f32_tmp, M, M, f32_tmp_array);
    generate_rand_f32(f32_rand_array, M * M);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (i == j) {
                f32_tmp_array[i * M + j] = tmp;
            }
        }
    }
    generate_posi_def_symme_f32(&f32_rand, &f32_tmp, &f32_dot, &f32_posi);
    // f32_rand_array is a positive definite symmetric matrix
    // turn the element to float64_t type and assign it to the test input f64_a_array
    for (int i = 0; i < M * M; i++) {
        f64_a_array[i] = (float64_t)(f32_posi_array[i]);
        // init f64_output and f64_output_ref and ensure f64_output[i] = f64_output_ref[i], i = 0, 1, 2, ...
        f64_output[i] = f64_a_array[i];
        f64_output_ref[i] = f64_a_array[i];
    }
    BENCH_START(riscv_mat_cholesky_f64);
    riscv_mat_cholesky_f64(&f64_A, &f64_des);
    BENCH_END(riscv_mat_cholesky_f64);
    ref_mat_cholesky_f64(&f64_A, &f64_ref);
    s = verify_results_f64(f64_output_ref, f64_output, M * M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_cholesky_f64);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_cholesky_f64);

    // solve upper triangular matrix
    // initialize upper triangular matrix by setting 0 to lower elements in matrix
    for(int i = 0; i < M; i++){
        for(int j = 0; j < i; j++){
            f64_a_array[i * M + j] = 0;
        }
    }
    riscv_matrix_instance_f64 f64_B;
    float64_t f64_b_array[M * N];
    riscv_mat_init_f64(&f64_B, M, N, (float64_t *)f64_b_array);
    float64_t f64_output_1[M * N];
    float64_t f64_output_ref_1[M * N];
    riscv_mat_init_f64(&f64_des, M, N, f64_output_1);
    riscv_mat_init_f64(&f64_ref, M, N, f64_output_ref_1);
    for (int i = 0; i < M * N; i++) {
        f64_b_array[i] = (float64_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
	f64_output_1[i] = 0.0;
	f64_output_ref_1[i] = 0.0;
    }
    BENCH_START(riscv_mat_solve_upper_triangular_f64);
    riscv_mat_solve_upper_triangular_f64(&f64_A, &f64_B, &f64_des);
    BENCH_END(riscv_mat_solve_upper_triangular_f64);
    ref_mat_solve_upper_triangular_f64(&f64_A, &f64_B, &f64_ref);
    s = verify_results_f64(f64_output_ref_1, f64_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_solve_upper_triangular_f64);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_solve_upper_triangular_f64);

    for (int i = 0; i < M * M; i++) {
        f64_a_array[i] = (float64_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
    for (int i = 0; i < M * N; i++) {
        f64_output_1[i] = 0.0;
        f64_output_ref_1[i] = 0.0;
    }
    // solve lower triangular matrix
    // initialize lower triangular matrix by setting 0 to upper elements in matrix
    for (int i = 0; i < M; i++){
        for (int j = M - 1; j > i; j--) {
            f64_a_array[i * M + j] = 0;
        }
    }
    BENCH_START(riscv_mat_solve_lower_triangular_f64);
    riscv_mat_solve_lower_triangular_f64(&f64_A, &f64_B, &f64_des);
    BENCH_END(riscv_mat_solve_lower_triangular_f64);
    ref_mat_solve_lower_triangular_f64(&f64_A, &f64_B, &f64_ref);
    s = verify_results_f64(f64_output_ref_1, f64_output_1, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_solve_lower_triangular_f64);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_solve_lower_triangular_f64);

    // Initialize Symmetric Matrices
    for (int i = 0; i < M * M; i++) {
        f64_a_array[i] = (float64_t)((rand() % Q31_MAX - Q31_MAX / 2) * 1.0 / Q31_MAX);
    }
    for (int i = 0; i < M; i++){
        for (int j = M - 1; j > i; j--){
            if (i == j) break;
            f64_a_array[i * M + j] = f64_a_array[j * M + i];
        }
    }
    uint16_t pp[M], pp_ref[M];
    float64_t f64_output_3[M * M];
    float64_t f64_output_ref_3[M * M];
    riscv_matrix_instance_f64 f64_B_back;
    riscv_mat_init_f64(&f64_B, M, M, (float64_t *)f64_output_3);
    riscv_mat_init_f64(&f64_B_back, M, M, f64_output_ref_3);
    riscv_mat_init_f64(&f64_des, M, M, f64_output);
    riscv_mat_init_f64(&f64_ref, M, M, f64_output_ref);
    BENCH_START(riscv_mat_ldlt_f64);
    riscv_mat_ldlt_f64(&f64_A, &f64_B, &f64_des, pp);
    BENCH_END(riscv_mat_ldlt_f64);
    ref_mat_ldlt_f64(&f64_A, &f64_B_back, &f64_ref, pp_ref);
    s1 = verify_results_f64(f64_output_ref, f64_output, M * M);
    s2 = verify_results_f64(f64_output_ref_3, f64_output_3, M * M);
    s3 = verify_results_q15((int16_t *)pp_ref, (int16_t *)pp, M);
    if (s1 != 0 || s2 != 0 || s3 != 0) {
        BENCH_ERROR(riscv_mat_ldlt_f64);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_ldlt_f64);
}

int main(void)
{
    unsigned long randvar;
    BENCH_INIT();
    do_srand();
    DSP_matrix_f32();
 //   DSP_matrix_f64();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
