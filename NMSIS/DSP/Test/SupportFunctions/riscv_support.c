//
// Created by lujun on 19-6-28.
//
// This contains f32, q31, q15, q7 to eachother, fill and copy
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "array.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include "../common.h"


int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

#define ARRAY_SIZE 100

float32_t pSrc_f32[ARRAY_SIZE];
float32_t pDst_f32[ARRAY_SIZE];
float32_t pDst_f32_ref[ARRAY_SIZE];
q31_t pSrc_q31[ARRAY_SIZE];
q31_t pDst_q31[ARRAY_SIZE];
q31_t pDst_q31_ref[ARRAY_SIZE];
q15_t pSrc_q15[ARRAY_SIZE];
q15_t pDst_q15[ARRAY_SIZE];
q15_t pDst_q15_ref[ARRAY_SIZE];
q7_t pSrc_q7[ARRAY_SIZE];
q7_t pDst_q7[ARRAY_SIZE];
q7_t pDst_q7_ref[ARRAY_SIZE];

static int DSP_Copy(void)
{
    int i;
    BENCH_START(riscv_copy_f32);
    riscv_copy_f32(pSrc_f32, pDst_f32, ARRAY_SIZE);
    BENCH_END(riscv_copy_f32);
    ref_copy_f32(pSrc_f32, pDst_f32_ref, ARRAY_SIZE);
    s = verify_results_f32(pDst_f32_ref, pDst_f32, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_copy_f32);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_copy_f32);

    /*****************************************************************/
    BENCH_START(riscv_copy_q31);
    riscv_copy_q31(pSrc_q31, pDst_q31, ARRAY_SIZE);
    BENCH_END(riscv_copy_q31);
    ref_copy_q31(pSrc_q31, pDst_q31_ref, ARRAY_SIZE);
    s = verify_results_q31(pDst_q31_ref, pDst_q31, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_copy_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_copy_q31);

    /*****************************************************************/
    BENCH_START(riscv_copy_q15);
    riscv_copy_q15(pSrc_q15, pDst_q15, ARRAY_SIZE);
    BENCH_END(riscv_copy_q15);
    ref_copy_q15(pSrc_q15, pDst_q15_ref, ARRAY_SIZE);
    s = verify_results_q15(pDst_q15_ref, pDst_q15, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_copy_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_copy_q15);
    /*****************************************************************/
    BENCH_START(riscv_copy_q7);
    riscv_copy_q7(pSrc_q7, pDst_q7, ARRAY_SIZE);
    BENCH_END(riscv_copy_q7);
    ref_copy_q7(pSrc_q7, pDst_q7_ref, ARRAY_SIZE);
    s = verify_results_q7(pDst_q7_ref, pDst_q7, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_copy_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_copy_q7);
}

static int DSP_Fill(void)
{
    int i;
    BENCH_START(riscv_fill_f32);
    riscv_fill_f32(3.33f, pDst_f32, ARRAY_SIZE);
    BENCH_END(riscv_fill_f32);
    ref_fill_f32(3.33f, pDst_f32_ref, ARRAY_SIZE);
    s = verify_results_f32(pDst_f32_ref, pDst_f32, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_fill_f32);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_fill_f32);

    /*****************************************************************/
    BENCH_START(riscv_fill_q31);
    riscv_fill_q31(0x11111111, pDst_q31, ARRAY_SIZE);
    BENCH_END(riscv_fill_q31);
    ref_fill_q31(0x11111111, pDst_q31_ref, ARRAY_SIZE);
    s = verify_results_q31(pDst_q31_ref, pDst_q31, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_fill_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_fill_q31);

    /*****************************************************************/
    BENCH_START(riscv_fill_q15);
    riscv_fill_q15(0x1111, pDst_q15, ARRAY_SIZE);
    BENCH_END(riscv_fill_q15);
    ref_fill_q15(0x1111, pDst_q15_ref, ARRAY_SIZE);
    s = verify_results_q15(pDst_q15_ref, pDst_q15, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_fill_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_fill_q15);

    /*****************************************************************/
    BENCH_START(riscv_fill_q7);
    riscv_fill_q7(0x11, pDst_q7, ARRAY_SIZE);
    BENCH_END(riscv_fill_q7);
    ref_fill_q7(0x11, pDst_q7_ref, ARRAY_SIZE);
    s = verify_results_q7(pDst_q7_ref, pDst_q7, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_fill_q7);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_fill_q7);
}

static int DSP_FloatToFix(void)
{
    int i;
    /*****************************************************************/
    BENCH_START(riscv_float_to_q31);
    riscv_float_to_q31(pSrc_f32, pDst_q31, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q31);
    ref_float_to_q31(pSrc_f32, pDst_q31_ref, ARRAY_SIZE);
    s = verify_results_q31(pDst_q31_ref, pDst_q31, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_float_to_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_float_to_q31);

    /*****************************************************************/
    BENCH_START(riscv_float_to_q15);
    riscv_float_to_q15(pSrc_f32, pDst_q15, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q15);
    ref_float_to_q15(pSrc_f32, pDst_q15_ref, ARRAY_SIZE);
    s = verify_results_q15(pDst_q15_ref, pDst_q15, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_float_to_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_float_to_q15);

    /*****************************************************************/
    BENCH_START(riscv_float_to_q7);
    riscv_float_to_q7(pSrc_f32, pDst_q7, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q7);
    ref_float_to_q7(pSrc_f32, pDst_q7_ref, ARRAY_SIZE);
    s = verify_results_q7(pDst_q7_ref, pDst_q7, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_float_to_q7);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_float_to_q7);
}

static int DSP_Q7(void)
{
    int i;
    BENCH_START(riscv_q7_to_float);
    riscv_q7_to_float(pSrc_q7, pDst_f32, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_float);
    ref_q7_to_float(pSrc_q7, pDst_f32_ref, ARRAY_SIZE);
    s = verify_results_f32(pDst_f32_ref, pDst_f32, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q7_to_float);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_q7_to_float);

    /*****************************************************************/
    BENCH_START(riscv_q7_to_q31);
    riscv_q7_to_q31(pSrc_q7, pDst_q31, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_q31);
    ref_q7_to_q31(pSrc_q7, pDst_q31_ref, ARRAY_SIZE);
    s = verify_results_q31(pDst_q31_ref, pDst_q31, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q7_to_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_q7_to_q31);

    /*****************************************************************/
    BENCH_START(riscv_q7_to_q15);
    riscv_q7_to_q15(pSrc_q7, pDst_q15, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_q15);
    ref_q7_to_q15(pSrc_q7, pDst_q15_ref, ARRAY_SIZE);
    s = verify_results_q15(pDst_q15_ref, pDst_q15, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q7_to_q15);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_q7_to_q15);
}

static int DSP_Q15(void)
{
    int i;
    BENCH_START(riscv_q15_to_float);
    riscv_q15_to_float(pSrc_q15, pDst_f32, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_float);
    ref_q15_to_float(pSrc_q15, pDst_f32_ref, ARRAY_SIZE);
    s = verify_results_f32(pDst_f32_ref, pDst_f32, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q15_to_float);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_q15_to_float);

    /*****************************************************************/
    BENCH_START(riscv_q15_to_q31);
    riscv_q15_to_q31(pSrc_q15, pDst_q31, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_q31);
    ref_q15_to_q31(pSrc_q15, pDst_q31_ref, ARRAY_SIZE);
    s = verify_results_q31(pDst_q31_ref, pDst_q31, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q15_to_q31);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_q15_to_q31);

    /*****************************************************************/
    BENCH_START(riscv_q15_to_q7);
    riscv_q15_to_q7(pSrc_q15, pDst_q7, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_q7);
    ref_q15_to_q7(pSrc_q15, pDst_q7_ref, ARRAY_SIZE);
    s = verify_results_q7(pDst_q7_ref, pDst_q7, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q15_to_q7);
        test_flag_error = 1;
    }

    BENCH_STATUS(riscv_q15_to_q7);
}

static int DSP_Q31(void)
{
    int i;
    BENCH_START(riscv_q31_to_float);
    riscv_q31_to_float(pSrc_q31, pDst_f32, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_float);
    ref_q31_to_float(pSrc_q31, pDst_f32_ref, ARRAY_SIZE);
    s = verify_results_f32(pDst_f32_ref, pDst_f32, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q31_to_float);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_q31_to_float);

    /*****************************************************************/
    BENCH_START(riscv_q31_to_q15);
    riscv_q31_to_q15(pSrc_q31, pDst_q15, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_q15);
    ref_q31_to_q15(pSrc_q31, pDst_q15_ref, ARRAY_SIZE);
    s = verify_results_q15(pDst_q15_ref, pDst_q15, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q31_to_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_q31_to_q15);

    /*****************************************************************/
    BENCH_START(riscv_q31_to_q7);
    riscv_q31_to_q7(pSrc_q31, pDst_q7, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_q7);
    ref_q31_to_q7(pSrc_q31, pDst_q7_ref, ARRAY_SIZE);
    s = verify_results_q7(pDst_q7_ref, pDst_q7, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_q31_to_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_q31_to_q7);
}

static int DSP_SORT(void)
{
    riscv_sort_instance_f32 S_sort;
    riscv_sort_init_f32(&S_sort, RISCV_SORT_BUBBLE, RISCV_SORT_ASCENDING);
    BENCH_START(riscv_sort_f32);
    riscv_sort_f32(&S_sort, f32_a_array, f32_out_array, ARRAY_SIZE1);
    BENCH_END(riscv_sort_f32);
    s = verify_results_f32(f32_out_sort_array_ref, f32_out_array, ARRAY_SIZE1);
    if (s != 0) {
        BENCH_ERROR(riscv_sort_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sort_f32);

    riscv_merge_sort_instance_f32 S_merge_sort;
    riscv_merge_sort_init_f32(&S_merge_sort, RISCV_SORT_ASCENDING, f32_temp_array);
    BENCH_START(riscv_merge_sort_f32);
    riscv_merge_sort_f32(&S_merge_sort, f32_a_array, f32_out_array, ARRAY_SIZE1);
    BENCH_END(riscv_merge_sort_f32);
    s = verify_results_f32(f32_out_sort_array_ref, f32_out_array, ARRAY_SIZE1);
    if (s != 0) {
        BENCH_ERROR(riscv_merge_sort_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_merge_sort_f32);

    //simulate y = sin(x)
    riscv_spline_instance_f32 S_spline;
    BENCH_START(riscv_spline_init_f32);
    riscv_spline_init_f32(&S_spline, RISCV_SPLINE_NATURAL, f32_x_array, f32_y_array, FUNC_SIZE, f32_spline_coef_array, f32_spline_temp_array);
    BENCH_END(riscv_spline_init_f32);
    BENCH_START(riscv_spline_f32);
    riscv_spline_f32(&S_spline, f32_test_x_array, f32_out_array, SPLINE_FUNC_SIZE);
    BENCH_END(riscv_spline_f32);
    s = verify_results_f32(f32_out_spline_array_ref, f32_out_array, SPLINE_FUNC_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_spline_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_spline_f32);

    BENCH_START(riscv_barycenter_f32);
    riscv_barycenter_f32(f32_barycenter_array, f32_barycenter_weights_array, f32_out_array, VEC_NUM, DIMENSION);
    BENCH_END(riscv_barycenter_f32);
    s = verify_results_f32(f32_out_barycenter_array_ref, f32_out_array, DIMENSION);
    if (s != 0) {
        BENCH_ERROR(riscv_barycenter_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_barycenter_f32);

    BENCH_START(riscv_weighted_sum_f32);
    f32_weighted_output = riscv_weighted_sum_f32(f32_weighted_sum_array, f32_weighted_array, WEIGHT_NUM);
    BENCH_END(riscv_weighted_sum_f32);
    s = verify_results_f32(&f32_weighted_output_ref, &f32_weighted_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_weighted_sum_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_weighted_sum_f32);

}

int main(void)
{
    BENCH_INIT();

    generate_rand_q7(pSrc_q7, ARRAY_SIZE);
    generate_rand_q15(pSrc_q15, ARRAY_SIZE);
    generate_rand_q31(pSrc_q31, ARRAY_SIZE);
    generate_rand_f32(pSrc_f32, ARRAY_SIZE);

    DSP_Q7();
    DSP_Q15();
    DSP_Q31();
    DSP_Fill();
    DSP_Copy();
    DSP_FloatToFix();
    DSP_SORT();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
