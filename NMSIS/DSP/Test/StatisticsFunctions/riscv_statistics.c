//
// Created by lujun on 19-6-28.DELTAF32
//

#include "riscv_math.h"
#include "array.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

#define ENABLE_ALL

#ifdef ENABLE_ALL
#define F32
#define F16
#define Q31
#define Q15
#define Q7
#define OTHER
#endif

#define ARRAY_SIZE 120
float32_t f32_a_array[ARRAY_SIZE];
float32_t f32_b_array[ARRAY_SIZE];
#if defined (RISCV_FLOAT16_SUPPORTED)
float16_t f16_a_array[ARRAY_SIZE];
float16_t f16_b_array[ARRAY_SIZE];
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
q7_t q7_a_array[ARRAY_SIZE];
q7_t q7_b_array[ARRAY_SIZE];
q15_t q15_a_array[ARRAY_SIZE];
q15_t q15_b_array[ARRAY_SIZE];
q31_t q31_a_array[ARRAY_SIZE];
q31_t q31_b_array[ARRAY_SIZE];
int8_t s;
int test_flag_error = 0;

BENCH_DECLARE_VAR();
int main(void)
{
    q63_t q63_out, q63_out_ref;
    float32_t f32_out, f32_out_ref;
    q31_t q31_out, q31_out_ref;
    q15_t q15_out, q15_out_ref;
    q7_t q7_out, q7_out_ref;
    uint32_t index, index_ref;

    BENCH_INIT();

    generate_rand_q7(q7_a_array, ARRAY_SIZE);
    generate_rand_q7(q7_b_array, ARRAY_SIZE);
    generate_rand_q15(q15_a_array, ARRAY_SIZE);
    generate_rand_q15(q15_b_array, ARRAY_SIZE);
    generate_rand_q31(q31_a_array, ARRAY_SIZE);
    generate_rand_q31(q31_b_array, ARRAY_SIZE);
    generate_rand_f32(f32_a_array, ARRAY_SIZE);
    generate_rand_f32(f32_b_array, ARRAY_SIZE);
#if defined F32
    //*****************************   f32   *************************
    // max
    BENCH_START(riscv_max_f32);
    riscv_max_f32(f32_a_array, ARRAY_SIZE, &f32_out, &index);
    BENCH_END(riscv_max_f32);
    ref_max_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref, &index_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_max_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_f32);

    // max_no_idx
    BENCH_START(riscv_max_no_idx_f32);
    riscv_max_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_max_no_idx_f32);
    ref_max_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_max_no_idx_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_no_idx_f32);

    // mean
    BENCH_START(riscv_mean_f32);
    riscv_mean_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_mean_f32);
    ref_mean_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mean_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mean_f32);

    // min
    BENCH_START(riscv_min_f32);
    riscv_min_f32(f32_a_array, ARRAY_SIZE, &f32_out, &index);
    BENCH_END(riscv_min_f32);
    ref_min_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref, &index_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_min_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_f32);
    // min_no_idx
    BENCH_START(riscv_min_no_idx_f32);
    riscv_min_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_min_no_idx_f32);
    ref_min_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_min_no_idx_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_no_idx_f32);

    // power
    BENCH_START(riscv_power_f32);
    riscv_power_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_power_f32);
    ref_power_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_power_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_power_f32);
    // rms
    BENCH_START(riscv_rms_f32);
    riscv_rms_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_rms_f32);
    ref_rms_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_rms_f32);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_rms_f32);
    // std
    BENCH_START(riscv_std_f32);
    riscv_std_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_std_f32);
    ref_std_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_std_f32);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_std_f32);
    // var
    BENCH_START(riscv_var_f32);
    riscv_var_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_var_f32);
    ref_var_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_var_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_var_f32);
    // absmax
    BENCH_START(riscv_absmax_f32);
    riscv_absmax_f32(f32_a_array, ARRAY_SIZE, &f32_out, &index);
    BENCH_END(riscv_absmax_f32);
    ref_absmax_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref, &index_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmax_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_f32);

    // absmax_no_idx
    BENCH_START(riscv_absmax_no_idx_f32);
    riscv_absmax_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_absmax_no_idx_f32);
    ref_absmax_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmax_no_idx_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_no_idx_f32);

    // absmin
    BENCH_START(riscv_absmin_f32);
    riscv_absmin_f32(f32_a_array, ARRAY_SIZE, &f32_out, &index);
    BENCH_END(riscv_absmin_f32);
    ref_absmin_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref, &index_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmin_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_f32);

    // absmin_no_idx
    BENCH_START(riscv_absmin_no_idx_f32);
    riscv_absmin_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_absmin_no_idx_f32);
    ref_absmin_no_idx_f32(f32_a_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmin_no_idx_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_no_idx_f32);

    // mse
    BENCH_START(riscv_mse_f32);
    riscv_mse_f32(f32_a_array, f32_b_array, ARRAY_SIZE, &f32_out);
    BENCH_END(riscv_mse_f32);
    ref_mse_f32(f32_a_array, f32_b_array, ARRAY_SIZE, &f32_out_ref);
    s = verify_results_f32(&f32_out_ref, &f32_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mse_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mse_f32);
#endif
#if defined (RISCV_FLOAT16_SUPPORTED)
    //*****************************   f16   *************************
    float16_t f16_out, f16_out_ref;
    generate_rand_f16(f16_a_array, ARRAY_SIZE);
    generate_rand_f16(f16_b_array, ARRAY_SIZE);
    // max
    BENCH_START(riscv_max_f16);
    riscv_max_f16(f16_a_array, ARRAY_SIZE, &f16_out, &index);
    BENCH_END(riscv_max_f16);
    ref_max_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref, &index_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_max_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_f16);

    // max_no_idx
    BENCH_START(riscv_max_no_idx_f16);
    riscv_max_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_max_no_idx_f16);
    ref_max_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_max_no_idx_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_no_idx_f16);

    // mean
    BENCH_START(riscv_mean_f16);
    riscv_mean_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_mean_f16);
    ref_mean_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mean_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mean_f16);

    // min
    BENCH_START(riscv_min_f16);
    riscv_min_f16(f16_a_array, ARRAY_SIZE, &f16_out, &index);
    BENCH_END(riscv_min_f16);
    ref_min_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref, &index_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_min_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_f16);
    // min_no_idx
    BENCH_START(riscv_min_no_idx_f16);
    riscv_min_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_min_no_idx_f16);
    ref_min_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_min_no_idx_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_no_idx_f16);

    // power
    BENCH_START(riscv_power_f16);
    riscv_power_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_power_f16);
    ref_power_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_power_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_power_f16);
    // rms
    BENCH_START(riscv_rms_f16);
    riscv_rms_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_rms_f16);
    ref_rms_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_rms_f16);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_rms_f16);
    // std
    BENCH_START(riscv_std_f16);
    riscv_std_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_std_f16);
    ref_std_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_std_f16);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_std_f16);
    // var
    BENCH_START(riscv_var_f16);
    riscv_var_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_var_f16);
    ref_var_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_var_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_var_f16);
    // absmax
    BENCH_START(riscv_absmax_f16);
    riscv_absmax_f16(f16_a_array, ARRAY_SIZE, &f16_out, &index);
    BENCH_END(riscv_absmax_f16);
    ref_absmax_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref, &index_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmax_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_f16);

    // absmax_no_idx
    BENCH_START(riscv_absmax_no_idx_f16);
    riscv_absmax_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_absmax_no_idx_f16);
    ref_absmax_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmax_no_idx_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_no_idx_f16);

    // absmin
    BENCH_START(riscv_absmin_f16);
    riscv_absmin_f16(f16_a_array, ARRAY_SIZE, &f16_out, &index);
    BENCH_END(riscv_absmin_f16);
    ref_absmin_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref, &index_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmin_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_f16);

    // absmin_no_idx
    BENCH_START(riscv_absmin_no_idx_f16);
    riscv_absmin_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_absmin_no_idx_f16);
    ref_absmin_no_idx_f16(f16_a_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmin_no_idx_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_no_idx_f16);

    // mse
    BENCH_START(riscv_mse_f16);
    riscv_mse_f16(f16_a_array, f16_b_array, ARRAY_SIZE, &f16_out);
    BENCH_END(riscv_mse_f16);
    ref_mse_f16(f16_a_array, f16_b_array, ARRAY_SIZE, &f16_out_ref);
    s = verify_results_f16(&f16_out_ref, &f16_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mse_f16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mse_f16);
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
#if defined Q31
    // ********************************* q31 *****************************
    // max
    BENCH_START(riscv_max_q31);
    riscv_max_q31(q31_a_array, ARRAY_SIZE, &q31_out, &index);
    BENCH_END(riscv_max_q31);
    ref_max_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref, &index_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_max_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_q31);

    // max_no_idx
    BENCH_START(riscv_max_no_idx_q31);
    riscv_max_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_max_no_idx_q31);
    ref_max_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_max_no_idx_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_no_idx_q31);
    // mean
    BENCH_START(riscv_mean_q31);
    riscv_mean_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_mean_q31);
    ref_mean_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mean_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mean_q31);

    // min
    BENCH_START(riscv_min_q31);
    riscv_min_q31(q31_a_array, ARRAY_SIZE, &q31_out, &index);
    BENCH_END(riscv_min_q31);
    ref_min_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref, &index_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_min_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_q31);

    // min_no_idx
    BENCH_START(riscv_min_no_idx_q31);
    riscv_min_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_min_no_idx_q31);
    ref_min_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_min_no_idx_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_min_no_idx_q31);
    // power
    BENCH_START(riscv_power_q31);
    riscv_power_q31(q31_a_array, ARRAY_SIZE, &q63_out);
    BENCH_END(riscv_power_q31);
    ref_power_q31(q31_a_array, ARRAY_SIZE, &q63_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_power_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_power_q31);
    // rms
    BENCH_START(riscv_rms_q31);
    riscv_rms_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_rms_q31);
    ref_rms_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_rms_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_rms_q31);

    // std
    BENCH_START(riscv_std_q31);
    riscv_std_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_std_q31);
    ref_std_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_std_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_std_q31);

    // var
    BENCH_START(riscv_var_q31);
    riscv_var_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_var_q31);
    ref_var_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_var_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_var_q31);

    // absmax
    BENCH_START(riscv_absmax_q31);
    riscv_absmax_q31(q31_a_array, ARRAY_SIZE, &q31_out, &index);
    BENCH_END(riscv_absmax_q31);
    ref_absmax_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref, &index_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmax_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_q31);

    // absmax_no_idx
    BENCH_START(riscv_absmax_no_idx_q31);
    riscv_absmax_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_absmax_no_idx_q31);
    ref_absmax_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmax_no_idx_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_absmax_no_idx_q31);

    // absmin
    BENCH_START(riscv_absmin_q31);
    riscv_absmin_q31(q31_a_array, ARRAY_SIZE, &q31_out, &index);
    BENCH_END(riscv_absmin_q31);
    ref_absmin_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref, &index_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmin_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_q31);

    // absmin_no_idx
    BENCH_START(riscv_absmin_no_idx_q31);
    riscv_absmin_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_absmin_no_idx_q31);
    ref_absmin_no_idx_q31(q31_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmin_no_idx_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_absmin_no_idx_q31);

    // mse
    BENCH_START(riscv_mse_q31);
    riscv_mse_q31(q31_a_array, q31_b_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_mse_q31);
    ref_mse_q31(q31_a_array, q31_b_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mse_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_mse_q31);
#endif
#if defined Q15
    // ********************************* q15 *****************************
   // max
    BENCH_START(riscv_max_q15);
    riscv_max_q15(q15_a_array, ARRAY_SIZE, &q15_out, &index);
    BENCH_END(riscv_max_q15);
    ref_max_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref, &index_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_max_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_q15);
    // max_no_idx
    BENCH_START(riscv_max_no_idx_q15);
    riscv_max_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_max_no_idx_q15);
    ref_max_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_max_no_idx_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_no_idx_q15);
    // mean
    BENCH_START(riscv_mean_q15);
    riscv_mean_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_mean_q15);
    ref_mean_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mean_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mean_q15);
    // min
    BENCH_START(riscv_min_q15);
    riscv_min_q15(q15_a_array, ARRAY_SIZE, &q15_out, &index);
    BENCH_END(riscv_min_q15);
    ref_min_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref, &index_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_min_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_q15);
    // min_no_idx
    BENCH_START(riscv_min_no_idx_q15);
    riscv_min_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_min_no_idx_q15);
    ref_min_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_min_no_idx_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_no_idx_q15);
    // power
    BENCH_START(riscv_power_q15);
    riscv_power_q15(q15_a_array, ARRAY_SIZE, &q63_out);
    BENCH_END(riscv_power_q15);
    ref_power_q15(q15_a_array, ARRAY_SIZE, &q63_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_power_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_power_q15);
    // rms
    BENCH_START(riscv_rms_q15);
    riscv_rms_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_rms_q15);
    ref_rms_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_rms_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rms_q15);
    // std
    BENCH_START(riscv_std_q15);
    riscv_std_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_std_q15);
    ref_std_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_std_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_std_q15);
    // var
    BENCH_START(riscv_var_q15);
    riscv_var_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_var_q15);
    ref_var_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_var_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_var_q15);
    // absmax
    BENCH_START(riscv_absmax_q15);
    riscv_absmax_q15(q15_a_array, ARRAY_SIZE, &q15_out, &index);
    BENCH_END(riscv_absmax_q15);
    ref_absmax_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref, &index_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmax_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_q15);
    // absmax_no_idx
    BENCH_START(riscv_absmax_no_idx_q15);
    riscv_absmax_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_absmax_no_idx_q15);
    ref_absmax_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmax_no_idx_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_absmax_no_idx_q15);
    // absmin
    BENCH_START(riscv_absmin_q15);
    riscv_absmin_q15(q15_a_array, ARRAY_SIZE, &q15_out, &index);
    BENCH_END(riscv_absmin_q15);
    ref_absmin_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref, &index_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmin_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_q15);
    // absmin_no_idx
    BENCH_START(riscv_absmin_no_idx_q15);
    riscv_absmin_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_absmin_no_idx_q15);
    ref_absmin_no_idx_q15(q15_a_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmin_no_idx_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_absmin_no_idx_q15);
    // mse
    BENCH_START(riscv_mse_q15);
    riscv_mse_q15(q15_a_array, q15_b_array, ARRAY_SIZE, &q15_out);
    BENCH_END(riscv_mse_q15);
    ref_mse_q15(q15_a_array, q15_b_array, ARRAY_SIZE, &q15_out_ref);
    s = verify_results_q15(&q15_out_ref, &q15_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mse_q15);
        test_flag_error = 1;
    }
#endif
#if defined Q7
    // ********************************* q7 *****************************
    BENCH_START(riscv_max_q7);
    riscv_max_q7(q7_a_array, ARRAY_SIZE, &q7_out, &index);
    BENCH_END(riscv_max_q7);
    ref_max_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref, &index_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_max_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_q7);
    // max_no_idx
    BENCH_START(riscv_max_no_idx_q7);
    riscv_max_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out);
    BENCH_END(riscv_max_no_idx_q7);
    ref_max_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
      if (s != 0) {
        BENCH_ERROR(riscv_max_no_idx_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_max_no_idx_q7);
    // mean
    BENCH_START(riscv_mean_q7);
    riscv_mean_q7(q7_a_array, ARRAY_SIZE, &q7_out);
    BENCH_END(riscv_mean_q7);
    ref_mean_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mean_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mean_q7);
    // min
    BENCH_START(riscv_min_q7);
    riscv_min_q7(q7_a_array, ARRAY_SIZE, &q7_out, &index);
    BENCH_END(riscv_min_q7);
    ref_min_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref, &index_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_min_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_q7);
    // min_no_idx
    BENCH_START(riscv_min_no_idx_q7);
    riscv_min_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out);
    BENCH_END(riscv_min_no_idx_q7);
    ref_min_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_min_no_idx_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_min_no_idx_q7);
    // power
    BENCH_START(riscv_power_q7);
    riscv_power_q7(q7_a_array, ARRAY_SIZE, &q31_out);
    BENCH_END(riscv_power_q7);
    ref_power_q7(q7_a_array, ARRAY_SIZE, &q31_out_ref);
    s = verify_results_q31(&q31_out_ref, &q31_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_power_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_power_q7);
    // absmax
    BENCH_START(riscv_absmax_q7);
    riscv_absmax_q7(q7_a_array, ARRAY_SIZE, &q7_out, &index);
    BENCH_END(riscv_absmax_q7);
    ref_absmax_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref, &index_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmax_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmax_q7);
    // absmax_no_idx
    BENCH_START(riscv_absmax_no_idx_q7);
    riscv_absmax_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out);
    BENCH_END(riscv_absmax_no_idx_q7);
    ref_absmax_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmax_no_idx_q7);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_absmax_no_idx_q7);
    // absmin
    BENCH_START(riscv_absmin_q7);
    riscv_absmin_q7(q7_a_array, ARRAY_SIZE, &q7_out, &index);
    BENCH_END(riscv_absmin_q7);
    ref_absmin_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref, &index_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if ((s != 0) || (index != index_ref)) {
        BENCH_ERROR(riscv_absmin_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_absmin_q7);
    // absmin_no_idx
    BENCH_START(riscv_absmin_no_idx_q7);
    riscv_absmin_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out);
    BENCH_END(riscv_absmin_no_idx_q7);
    ref_absmin_no_idx_q7(q7_a_array, ARRAY_SIZE, &q7_out_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_absmin_no_idx_q7);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_absmin_no_idx_q7);
    // mse
    BENCH_START(riscv_mse_q7);
    riscv_mse_q7(q7_a_array, q7_b_array, ARRAY_SIZE, &q7_out);
    BENCH_END(riscv_mse_q7);
    ref_mse_q7(q7_a_array, q7_b_array, ARRAY_SIZE, &q7_out_ref);
    s = verify_results_q7(&q7_out_ref, &q7_out, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_mse_q7);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_mse_q7);
#endif
#ifdef OTHER
    //entropy -Sum(p ln p)
    float32_t f32_out_array;
    float32_t f32_out_entropy_array_ref = -1175041.33630915;
    BENCH_START(riscv_entropy_f32);
    f32_out_array = riscv_entropy_f32(f32_c_array, ARRAY_SIZE1);
    BENCH_END(riscv_entropy_f32);
    s = verify_results_f32(&f32_out_entropy_array_ref, &f32_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_entropy_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_entropy_f32);

    //entropy -Sum(p ln p)
    float64_t f64_out_entropy_array_ref = -197945362.477341;
    BENCH_START(riscv_entropy_f64);
    f64_out_array = riscv_entropy_f64(f64_c_array, ARRAY_SIZE1);
    BENCH_END(riscv_entropy_f64);
    s = verify_results_f64(&f64_out_entropy_array_ref, &f64_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_entropy_f64);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_entropy_f64);

    float32_t f32_out_kl_array_ref = 79161.1759381185;
    BENCH_START(riscv_kullback_leibler_f32);
    f32_out_array = riscv_kullback_leibler_f32(f32_c_array, f32_d_array, ARRAY_SIZE1);
    BENCH_END(riscv_kullback_leibler_f32);
    s = verify_results_f32(&f32_out_kl_array_ref, &f32_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_kullback_leibler_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_kullback_leibler_f32);

    float64_t f64_out_kl_array_ref = 6839705.86036978;
    BENCH_START(riscv_kullback_leibler_f64);
    f64_out_array = riscv_kullback_leibler_f64(f64_c_array, f64_d_array, ARRAY_SIZE1);
    BENCH_END(riscv_kullback_leibler_f64);
    s = verify_results_f64(&f64_out_kl_array_ref, &f64_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_kullback_leibler_f64);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_kullback_leibler_f64);

    float32_t f32_out_lsedp_array_ref = 2957.82258923639;
    BENCH_START(riscv_logsumexp_dot_prod_f32);
    f32_out_array = riscv_logsumexp_dot_prod_f32(f32_c_array, f32_d_array, ARRAY_SIZE1, f32_temp_array);
    BENCH_END(riscv_logsumexp_dot_prod_f32);
    s = verify_results_f32(&f32_out_lsedp_array_ref, &f32_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_logsumexp_dot_prod_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_logsumexp_dot_prod_f32);

    float32_t f32_out_lse_array_ref = 1470.84425707590;
    BENCH_START(riscv_logsumexp_f32);
    f32_out_array = riscv_logsumexp_f32(f32_c_array, ARRAY_SIZE1);
    BENCH_END(riscv_logsumexp_f32);
    s = verify_results_f32(&f32_out_lse_array_ref, &f32_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_logsumexp_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_logsumexp_f32);
#endif
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
