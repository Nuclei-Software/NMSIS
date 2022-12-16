
//
// Created by lujun on 19-6-28.
//
// This contains f32, q31 and q7 versions of matrix
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

q7_t q7_a_array[M * K];
q7_t q7_b_array[K * N];
q7_t q7_output[M * N];
q7_t q7_output_ref[M * N];
q7_t q7_output_back[K * N];
q7_t q7_b_vec[K];

int test_flag_error = 0;

BENCH_DECLARE_VAR();

int DSP_matrix_q7(void)
{
    int8_t s;

    riscv_matrix_instance_q7 q7_A;
    riscv_matrix_instance_q7 q7_B;
    riscv_matrix_instance_q7 q7_ref;
    riscv_matrix_instance_q7 q7_des;

    q7_t q7_ref_vec[M];
    q7_t q7_dst_vec[M];

    riscv_mat_init_q7(&q7_A, M, K, (q7_t *)q7_a_array);
    riscv_mat_init_q7(&q7_B, K, N, (q7_t *)q7_b_array);
    riscv_mat_init_q7(&q7_des, M, N, q7_output);
    riscv_mat_init_q7(&q7_ref, M, N, q7_output_ref);
    generate_rand_q7(q7_a_array, M * K);
    generate_rand_q7(q7_b_array, K * N);
    generate_rand_q7(q7_b_vec, K);

    // ****************   q7   *********************
    // mat_mult_q7
    generate_rand_q7(q7_output, M * N);
    // ensure q7_output_ref[i] = q7_output[i], i = 0, 1, 2, ...
    memcpy(q7_output, q7_output_ref, sizeof(q7_t) * M * N);
    BENCH_START(riscv_mat_mult_q7);
    riscv_mat_mult_q7(&q7_A, &q7_B, &q7_des, q7_output_back);
    BENCH_END(riscv_mat_mult_q7);
    ref_mat_mult_q7(&q7_A, &q7_B, &q7_ref);
    s = verify_results_q7(q7_output, q7_output_ref, M * N);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_mult_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_mult_q7);

    // mat_vec_mult
    BENCH_START(riscv_mat_vec_mult_q7);
    riscv_mat_vec_mult_q7(&q7_A, q7_b_vec, q7_dst_vec);
    BENCH_END(riscv_mat_vec_mult_q7);
    ref_mat_vec_mult_q7(&q7_A, q7_b_vec, q7_ref_vec);
    s = verify_results_q7(q7_ref_vec, q7_dst_vec, M);
    if (s != 0) {
        BENCH_ERROR(riscv_mat_vec_mult_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mat_vec_mult_q7);

}

int main(void)
{
    BENCH_INIT();
    DSP_matrix_q7();
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}

