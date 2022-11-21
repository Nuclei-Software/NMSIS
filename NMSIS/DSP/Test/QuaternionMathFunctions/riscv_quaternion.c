//
// Created by lujun on 19-6-28.DELTAF32
//

#include "riscv_math.h"
//#include "array.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

#define ARRAY_SIZE 232

float32_t f32_a_array[ARRAY_SIZE];
float32_t f32_b_array[ARRAY_SIZE];

float32_t f32_out[ARRAY_SIZE];
float32_t f32_out_ref[ARRAY_SIZE];

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

int main()
{
    BENCH_INIT();

    generate_rand_f32(f32_a_array, ARRAY_SIZE);
    generate_rand_f32(f32_b_array, ARRAY_SIZE);
    // conjugate
    BENCH_START(riscv_quaternion_conjugate_f32);
    riscv_quaternion_conjugate_f32(f32_a_array, f32_out, ARRAY_SIZE / 4);
    BENCH_END(riscv_quaternion_conjugate_f32);
    ref_quaternion_conjugate_f32(f32_a_array, f32_out_ref, ARRAY_SIZE / 4);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE / 4);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion_conjugate_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion_conjugate_f32);

    // inverse
    BENCH_START(riscv_quaternion_inverse_f32);
    riscv_quaternion_inverse_f32(f32_a_array, f32_out, ARRAY_SIZE / 4);
    BENCH_END(riscv_quaternion_inverse_f32);
    ref_quaternion_inverse_f32(f32_a_array, f32_out_ref, ARRAY_SIZE / 4);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE / 4);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion_inverse_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion_inverse_f32);

    // norm
    BENCH_START(riscv_quaternion_norm_f32);
    riscv_quaternion_norm_f32(f32_a_array, f32_out, ARRAY_SIZE / 4);
    BENCH_END(riscv_quaternion_norm_f32);
    ref_quaternion_norm_f32(f32_a_array, f32_out_ref, ARRAY_SIZE / 4);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE / 4);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion_norm_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion_norm_f32);

    // normalize
    BENCH_START(riscv_quaternion_normalize_f32);
    riscv_quaternion_normalize_f32(f32_a_array, f32_out, ARRAY_SIZE/4);
    BENCH_END(riscv_quaternion_normalize_f32);
    ref_quaternion_normalize_f32(f32_a_array, f32_out_ref, ARRAY_SIZE/4);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE / 4);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion_normalize_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion_normalize_f32);

    // product
    BENCH_START(riscv_quaternion_product_f32);
    riscv_quaternion_product_f32(f32_a_array, f32_b_array, f32_out, ARRAY_SIZE / 4);
    BENCH_END(riscv_quaternion_product_f32);
    ref_quaternion_product_f32(f32_a_array, f32_b_array, f32_out_ref, ARRAY_SIZE / 4);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE / 4);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion_product_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion_product_f32);

    // product_single
    BENCH_START(riscv_quaternion_product_single_f32);
    riscv_quaternion_product_single_f32(f32_a_array, f32_out, f32_out);
    BENCH_END(riscv_quaternion_product_single_f32);
    ref_quaternion_product_single_f32(f32_a_array, f32_out_ref, f32_out_ref);
    s = verify_results_f32(f32_out_ref, f32_out, 4);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion_product_single_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion_product_single_f32);

    // quaternion2rotation
    BENCH_START(riscv_quaternion2rotation_f32);
    riscv_quaternion2rotation_f32(f32_a_array, f32_out, ARRAY_SIZE / 9);
    BENCH_END(riscv_quaternion2rotation_f32);
    ref_quaternion2rotation_f32(f32_a_array, f32_out_ref, ARRAY_SIZE / 9);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_quaternion2rotation_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_quaternion2rotation_f32);

    // rotation2quaternion
    BENCH_START(riscv_rotation2quaternion_f32);
    riscv_rotation2quaternion_f32(f32_a_array, f32_out, ARRAY_SIZE / 9);
    BENCH_END(riscv_rotation2quaternion_f32);
    ref_rotation2quaternion_f32(f32_a_array, f32_out_ref, ARRAY_SIZE / 9);
    s = verify_results_f32(f32_out_ref, f32_out, ARRAY_SIZE / 9 * 4);
    if (s != 0) {
        BENCH_ERROR(riscv_rotation2quaternion_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rotation2quaternion_f32);

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
