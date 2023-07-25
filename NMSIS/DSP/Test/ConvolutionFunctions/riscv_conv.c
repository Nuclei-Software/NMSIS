//
// Created by lujun on 19-6-28.
//

#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

#define ARRAYA_SIZE_F32 200
#define ARRAYB_SIZE_F32 100
float32_t test_conv_input_f32_A[ARRAYA_SIZE_F32];
float32_t test_conv_input_f32_B[ARRAYB_SIZE_F32];
float32_t output_f32[2 * max(ARRAYA_SIZE_F32, ARRAYB_SIZE_F32)];
float32_t output_f32_ref[2 * max(ARRAYA_SIZE_F32, ARRAYB_SIZE_F32)];

#define ARRAYA_SIZE_Q31 200
#define ARRAYB_SIZE_Q31 100
q31_t test_conv_input_q31_A[ARRAYA_SIZE_Q31];
q31_t test_conv_input_q31_B[ARRAYB_SIZE_Q31];
q31_t output_q31[2 * max(ARRAYA_SIZE_Q31, ARRAYB_SIZE_Q31)];
q31_t output_q31_ref[2 * max(ARRAYA_SIZE_Q31, ARRAYB_SIZE_Q31)];

#define ARRAYA_SIZE_Q15 200
#define ARRAYB_SIZE_Q15 100
q15_t test_conv_input_q15_A[ARRAYA_SIZE_Q15];
q15_t test_conv_input_q15_B[ARRAYB_SIZE_Q15];
q15_t output_q15[2 * max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15)];
q15_t output_q15_ref[2 * max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15)];
q15_t q15_pScratch1[max(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15) + 2 * min(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15) - 2];
q15_t q15_pScratch2[min(ARRAYA_SIZE_Q15, ARRAYB_SIZE_Q15)];

#define ARRAYA_SIZE_Q7  25
#define ARRAYB_SIZE_Q7  25
q7_t test_conv_input_q7_A[ARRAYA_SIZE_Q7];
q7_t test_conv_input_q7_B[ARRAYB_SIZE_Q7];
q7_t output_q7[2 * max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];
q7_t output_q7_ref[2 * max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];
q15_t pScratch1[max(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7) + 2 * min(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7) - 2];
q15_t pScratch2[min(ARRAYA_SIZE_Q7, ARRAYB_SIZE_Q7)];

void riscv_conv_functions(void)
{
    BENCH_START(riscv_conv_f32);
    riscv_conv_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                 output_f32);
    BENCH_END(riscv_conv_f32);
    ref_conv_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                 output_f32_ref);
    s = verify_results_f32(output_f32_ref, output_f32, ARRAYA_SIZE_F32 + ARRAYB_SIZE_F32 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_f32);

    BENCH_START(riscv_conv_q31);
    riscv_conv_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                 output_q31);
    BENCH_END(riscv_conv_q31);
    ref_conv_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                 output_q31_ref);
    s = verify_results_q31(output_q31_ref, output_q31, ARRAYA_SIZE_Q31 + ARRAYB_SIZE_Q31 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_q31);

    BENCH_START(riscv_conv_q15);
    riscv_conv_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                 output_q15);
    BENCH_END(riscv_conv_q15);
    ref_conv_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                 output_q15_ref);
    s = verify_results_q15(output_q15_ref, output_q15, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_q15);

    BENCH_START(riscv_conv_q7);
    riscv_conv_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7, output_q7);
    BENCH_END(riscv_conv_q7);
    ref_conv_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                output_q7_ref);
    s = verify_results_q7(output_q7_ref, output_q7, ARRAYA_SIZE_Q7 + ARRAYB_SIZE_Q7 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_q7);

    BENCH_START(riscv_conv_fast_q15);
    riscv_conv_fast_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                      output_q15);
    BENCH_END(riscv_conv_fast_q15);
    ref_conv_fast_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                      output_q15_ref);
    s = verify_results_q15(output_q15_ref, output_q15, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_fast_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_fast_q15);

    BENCH_START(riscv_conv_fast_q31);
    riscv_conv_fast_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                      output_q31);
    BENCH_END(riscv_conv_fast_q31);
    ref_conv_fast_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                      output_q31_ref);
    s = verify_results_q31_great_precision(output_q31_ref, output_q31, ARRAYA_SIZE_Q31 + ARRAYB_SIZE_Q31 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_fast_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_fast_q31);

    BENCH_START(riscv_conv_opt_q15);
    riscv_conv_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                     output_q15, q15_pScratch1, q15_pScratch2);
    BENCH_END(riscv_conv_opt_q15);
    ref_conv_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                     output_q15_ref, q15_pScratch1, q15_pScratch2);
    s = verify_results_q15(output_q15_ref, output_q15, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_opt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_opt_q15);

    BENCH_START(riscv_conv_opt_q7);
    riscv_conv_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                    output_q7, pScratch1, pScratch2);
    BENCH_END(riscv_conv_opt_q7);
    ref_conv_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                    output_q7_ref, pScratch1, pScratch2);
    s = verify_results_q7(output_q7_ref, output_q7, ARRAYA_SIZE_Q7 + ARRAYB_SIZE_Q7 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_opt_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_opt_q7);

    BENCH_START(riscv_conv_fast_opt_q15);
    riscv_conv_fast_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                          output_q15, q15_pScratch1, q15_pScratch2);
    BENCH_END(riscv_conv_fast_opt_q15);
    ref_conv_fast_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                          output_q15_ref, q15_pScratch1, q15_pScratch2);
    s = verify_results_q15(output_q15_ref, output_q15, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_fast_opt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_fast_opt_q15);

}

void riscv_correlate_functions(void)
{
    uint32_t zero_padding;

    BENCH_START(riscv_correlate_f32);
    riscv_correlate_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                      output_f32);
    BENCH_END(riscv_correlate_f32);
    ref_correlate_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                      output_f32_ref);
    zero_padding = abs(ARRAYA_SIZE_F32 - ARRAYB_SIZE_F32);
    s = verify_results_f32(output_f32_ref + zero_padding, output_f32 + zero_padding, ARRAYA_SIZE_F32 + ARRAYB_SIZE_F32 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_f32);

    BENCH_START(riscv_correlate_q31);
    riscv_correlate_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                      output_q31);
    BENCH_END(riscv_correlate_q31);
    ref_correlate_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                      output_q31_ref);
    zero_padding = abs(ARRAYA_SIZE_Q31 - ARRAYB_SIZE_Q31);
    s = verify_results_q31_low_precision(output_q31_ref + zero_padding, output_q31 + zero_padding, ARRAYA_SIZE_Q31 + ARRAYB_SIZE_Q31 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_q31);

    BENCH_START(riscv_correlate_q15);
    riscv_correlate_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                      output_q15);
    BENCH_END(riscv_correlate_q15);
    ref_correlate_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                      output_q15_ref);
    zero_padding = abs(ARRAYA_SIZE_Q15 - ARRAYB_SIZE_Q15);
    s = verify_results_q15(output_q15_ref + zero_padding, output_q15 + zero_padding, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_q15);

    BENCH_START(riscv_correlate_q7);
    riscv_correlate_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                     output_q7);
    BENCH_END(riscv_correlate_q7);
    ref_correlate_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                     output_q7_ref);
    zero_padding = abs(ARRAYA_SIZE_Q7 - ARRAYB_SIZE_Q7);
    s = verify_results_q7(output_q7_ref + zero_padding, output_q7 + zero_padding, ARRAYA_SIZE_Q7 + ARRAYB_SIZE_Q7 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_q7);

    BENCH_START(riscv_correlate_fast_q15);
    riscv_correlate_fast_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B,
                           ARRAYB_SIZE_Q15, output_q15);
    BENCH_END(riscv_correlate_fast_q15);
    ref_correlate_fast_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B,
                           ARRAYB_SIZE_Q15, output_q15_ref);
    zero_padding = abs(ARRAYA_SIZE_Q15 - ARRAYB_SIZE_Q15);
    s = verify_results_q15(output_q15_ref + zero_padding, output_q15 + zero_padding, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_fast_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_fast_q15);

    BENCH_START(riscv_correlate_fast_q31);
    riscv_correlate_fast_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B,
                           ARRAYB_SIZE_Q31, output_q31);
    BENCH_END(riscv_correlate_fast_q31);
    ref_correlate_fast_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B,
                           ARRAYB_SIZE_Q31, output_q31_ref);
    zero_padding = abs(ARRAYA_SIZE_Q31 - ARRAYB_SIZE_Q31);
    s = verify_results_q31_great_precision(output_q31_ref + zero_padding, output_q31 + zero_padding, ARRAYA_SIZE_Q31 + ARRAYB_SIZE_Q31 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_fast_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_fast_q31);

    BENCH_START(riscv_correlate_opt_q15);
    riscv_correlate_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                          output_q15, q15_pScratch1);
    BENCH_END(riscv_correlate_opt_q15);
    ref_correlate_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                          output_q15_ref, q15_pScratch1);
    zero_padding = abs(ARRAYA_SIZE_Q15 - ARRAYB_SIZE_Q15);
    s = verify_results_q15(output_q15_ref + zero_padding, output_q15 + zero_padding, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_opt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_opt_q15);

    BENCH_START(riscv_correlate_opt_q7);
    riscv_correlate_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                         output_q7, pScratch1, pScratch2);
    BENCH_END(riscv_correlate_opt_q7);
    ref_correlate_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                         output_q7_ref, pScratch1, pScratch2);
    zero_padding = abs(ARRAYA_SIZE_Q7 - ARRAYB_SIZE_Q7);
    s = verify_results_q7(output_q7_ref + zero_padding, output_q7 + zero_padding, ARRAYA_SIZE_Q7 + ARRAYB_SIZE_Q7 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_opt_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_opt_q7);

    BENCH_START(riscv_correlate_fast_opt_q15);
    riscv_correlate_fast_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15,
                               test_conv_input_q15_B, ARRAYB_SIZE_Q15, output_q15,
                               q15_pScratch1);
    BENCH_END(riscv_correlate_fast_opt_q15);
    ref_correlate_fast_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15,
                               test_conv_input_q15_B, ARRAYB_SIZE_Q15, output_q15_ref,
                               q15_pScratch1);
    zero_padding = abs(ARRAYA_SIZE_Q15 - ARRAYB_SIZE_Q15);
    s = verify_results_q15(output_q15_ref + zero_padding, output_q15 + zero_padding, ARRAYA_SIZE_Q15 + ARRAYB_SIZE_Q15 - 2);
    if (s != 0) {
        BENCH_ERROR(riscv_correlate_fast_opt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_correlate_fast_opt_q15);
}

void riscv_conv_partial_functions(void)
{
    uint32_t firstIndex, numPoints;

    // *****************************************************
    firstIndex = 10;
    numPoints = 100;
    BENCH_START(riscv_conv_partial_f32);
    riscv_conv_partial_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                         output_f32, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_f32);
    ref_conv_partial_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32, test_conv_input_f32_B, ARRAYB_SIZE_F32,
                         output_f32_ref, firstIndex, numPoints);
    s = verify_results_f32(output_f32_ref + firstIndex, output_f32 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_f32);

    firstIndex = 10;
    numPoints = 100;
    BENCH_START(riscv_conv_partial_q31);
    riscv_conv_partial_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                         output_q31, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_q31);
    ref_conv_partial_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B, ARRAYB_SIZE_Q31,
                         output_q31_ref, firstIndex, numPoints);
    s = verify_results_q31(output_q31_ref + firstIndex, output_q31 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_q31);

    firstIndex = 5;
    numPoints = 100;
    BENCH_START(riscv_conv_partial_q15);
    riscv_conv_partial_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                         output_q15, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_q15);
    ref_conv_partial_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B, ARRAYB_SIZE_Q15,
                         output_q15_ref, firstIndex, numPoints);
    s = verify_results_q15(output_q15_ref + firstIndex, output_q15 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_q15);

    firstIndex = 2;
    numPoints = 14;
    BENCH_START(riscv_conv_partial_q7);
    riscv_conv_partial_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                        output_q7, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_q7);
    ref_conv_partial_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                        output_q7_ref, firstIndex, numPoints);
    s = verify_results_q7(output_q7_ref + firstIndex, output_q7 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_q7);
    firstIndex = 10;
    numPoints = 200;
    BENCH_START(riscv_conv_partial_fast_q31);
    riscv_conv_partial_fast_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B,
                              ARRAYB_SIZE_Q31, output_q31, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_fast_q31);
    ref_conv_partial_fast_q31(test_conv_input_q31_A, ARRAYA_SIZE_Q31, test_conv_input_q31_B,
                              ARRAYB_SIZE_Q31, output_q31_ref, firstIndex, numPoints);
    s = verify_results_q31_great_precision(output_q31_ref + firstIndex, output_q31 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_fast_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_fast_q31);
    firstIndex = 10;
    numPoints = 110;
    BENCH_START(riscv_conv_partial_fast_q15);
    riscv_conv_partial_fast_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B,
                              ARRAYB_SIZE_Q15, output_q15, firstIndex, numPoints);
    BENCH_END(riscv_conv_partial_fast_q15);
    ref_conv_partial_fast_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B,
                              ARRAYB_SIZE_Q15, output_q15_ref, firstIndex, numPoints);
    s = verify_results_q15(output_q15_ref + firstIndex, output_q15 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_fast_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_fast_q15);
    firstIndex = 10;
    numPoints = 100;
    BENCH_START(riscv_conv_partial_opt_q15);
    riscv_conv_partial_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B,
                             ARRAYB_SIZE_Q15, output_q15, firstIndex, numPoints, q15_pScratch1, q15_pScratch2);
    BENCH_END(riscv_conv_partial_opt_q15);
    ref_conv_partial_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15, test_conv_input_q15_B,
                             ARRAYB_SIZE_Q15, output_q15_ref, firstIndex, numPoints, q15_pScratch1,
                             q15_pScratch2);
    s = verify_results_q15(output_q15_ref + firstIndex, output_q15 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_opt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_opt_q15);
    firstIndex = 2;
    numPoints = 14;
    BENCH_START(riscv_conv_partial_opt_q7);
    riscv_conv_partial_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                            output_q7, firstIndex, numPoints, pScratch1, pScratch2);
    BENCH_END(riscv_conv_partial_opt_q7);
    ref_conv_partial_opt_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7, test_conv_input_q7_B, ARRAYB_SIZE_Q7,
                            output_q7_ref, firstIndex, numPoints, pScratch1, pScratch2);
    s = verify_results_q7(output_q7_ref + firstIndex, output_q7 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_opt_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_opt_q7);
    firstIndex = 10;
    numPoints = 100;
    BENCH_START(riscv_conv_partial_fast_opt_q15);
    riscv_conv_partial_fast_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15,
                                  test_conv_input_q15_B, ARRAYB_SIZE_Q15, output_q15, firstIndex,
                                  numPoints, q15_pScratch1, q15_pScratch2);
    BENCH_END(riscv_conv_partial_fast_opt_q15);
    ref_conv_partial_fast_opt_q15(test_conv_input_q15_A, ARRAYA_SIZE_Q15,
                                  test_conv_input_q15_B, ARRAYB_SIZE_Q15, output_q15_ref,
                                  firstIndex, numPoints, q15_pScratch1, q15_pScratch2);
    s = verify_results_q15(output_q15_ref + firstIndex, output_q15 + firstIndex, numPoints - 1);
    if (s != 0) {
        BENCH_ERROR(riscv_conv_partial_fast_opt_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_conv_partial_fast_opt_q15);
}

int main(void)
{
    uint16_t i;

    BENCH_INIT();
    generate_rand_q7(test_conv_input_q7_A, ARRAYA_SIZE_Q7);
    generate_rand_q7(test_conv_input_q7_B, ARRAYB_SIZE_Q7);

    for (int i = 0; i < ARRAYA_SIZE_Q15; i++) {
        test_conv_input_q15_A[i] = (q15_t)(rand() % (Q15_MAX / 2 ) - Q15_MAX / 4);
    }
    for (int i = 0; i < ARRAYB_SIZE_Q15; i++) {
        test_conv_input_q15_B[i] = (q15_t)(rand() % (Q15_MAX / 2 ) - Q15_MAX / 4);
    }

    for (int i = 0; i < ARRAYA_SIZE_Q31; i++) {
        test_conv_input_q31_A[i] = (q31_t)(rand() % (Q31_MAX / 2) - Q31_MAX / 4);
    }
    for (int i = 0; i < ARRAYB_SIZE_Q31; i++) {
        test_conv_input_q31_B[i] = (q31_t)(rand() % (Q31_MAX / 2) - Q31_MAX / 4);
    }
    generate_rand_f32(test_conv_input_f32_A, ARRAYA_SIZE_F32);
    generate_rand_f32(test_conv_input_f32_B, ARRAYB_SIZE_F32);

    // *****************************************************
    // conv part
    // *****************************************************
    riscv_conv_functions();
    riscv_correlate_functions();
    riscv_conv_partial_functions();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
