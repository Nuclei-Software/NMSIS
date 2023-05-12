//
// Created by lujun on 19-6-21.
//
#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

#define ENABLE_ALL

#ifdef ENABLE_ALL
#define ABS
#define ADD
#define DOT_PROD
#define MULT
#define NEGATE
#define AND
#define OR
#define NOT
#define XOR

#define OFFSET
#define SCALE
#define SHIFT
#define SUB
#define CLIP
#endif

#define ARRAY_SIZE 127
float32_t f32_a_array[ARRAY_SIZE];
float32_t f32_b_array[ARRAY_SIZE];
float32_t f32_out_array[ARRAY_SIZE];
float32_t f32_out_ref_array[ARRAY_SIZE];
q7_t q7_a_array[ARRAY_SIZE];
q7_t q7_b_array[ARRAY_SIZE];
q7_t q7_out_array[ARRAY_SIZE];
q7_t q7_out_ref_array[ARRAY_SIZE];
q15_t q15_a_array[ARRAY_SIZE];
q15_t q15_b_array[ARRAY_SIZE];
q15_t q15_out_array[ARRAY_SIZE];
q15_t q15_out_ref_array[ARRAY_SIZE];
q31_t q31_a_array[ARRAY_SIZE];
q31_t q31_b_array[ARRAY_SIZE];
q31_t q31_out_array[ARRAY_SIZE];
q31_t q31_out_ref_array[ARRAY_SIZE];

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

static void riscv_abs_functions(void)
{
#ifdef ABS
    // riscv_abs_f32.c
    BENCH_START(riscv_abs_f32);
    riscv_abs_f32(f32_a_array, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_abs_f32);
    ref_abs_f32(f32_a_array, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_abs_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_abs_f32);

    // riscv_abs_q7.c
    BENCH_START(riscv_abs_q7);
    riscv_abs_q7(q7_a_array, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_abs_q7);
    ref_abs_q7(q7_a_array, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_abs_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_abs_q7);

    // riscv_abs_q15.c
    BENCH_START(riscv_abs_q15);
    riscv_abs_q15(q15_a_array, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_abs_q15);
    ref_abs_q15(q15_a_array, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_abs_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_abs_q15);

    // riscv_abs_q31.c
    BENCH_START(riscv_abs_q31);
    riscv_abs_q31(q31_a_array, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_abs_q31);
    ref_abs_q31(q31_a_array, q31_out_ref_array, ARRAY_SIZE);
    verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_abs_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_abs_q31);
#endif
}

static void riscv_add_functions(void)
{
#ifdef ADD
    // riscv_add_f32.c
    BENCH_START(riscv_add_f32);
    riscv_add_f32(f32_a_array, f32_b_array, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_add_f32);
    ref_add_f32(f32_a_array, f32_b_array, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_add_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_add_f32);

    // riscv_add_q7.c
    BENCH_START(riscv_add_q7);
    riscv_add_q7(q7_a_array, q7_b_array, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_add_q7);
    ref_add_q7(q7_a_array, q7_b_array, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_add_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_add_q7);
    // riscv_add_q15.c
    BENCH_START(riscv_add_q15);
    riscv_add_q15(q15_a_array, q15_b_array, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_add_q15);
    ref_add_q15(q15_a_array, q15_b_array, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_add_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_add_q15);
    // riscv_add_q31.c
    BENCH_START(riscv_add_q31);
    riscv_add_q31(q31_a_array, q31_b_array, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_add_q31);
    ref_add_q31(q31_a_array, q31_b_array, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_add_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_add_q31);
#endif
}

static void riscv_dot_prod_functions(void)
{
#ifdef DOT_PROD
    q63_t q63_out_array;
    q63_t q63_out_ref_array;
    // f32_dot
    BENCH_START(riscv_dot_prod_f32);
    riscv_dot_prod_f32(f32_a_array, f32_b_array, ARRAY_SIZE, f32_out_array);
    BENCH_END(riscv_dot_prod_f32);
    ref_dot_prod_f32(f32_a_array, f32_b_array, ARRAY_SIZE, f32_out_ref_array);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_dot_prod_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dot_prod_f32);

    // q7_dot
    BENCH_START(riscv_dot_prod_q7);
    riscv_dot_prod_q7(q7_a_array, q7_b_array, ARRAY_SIZE, q31_out_array);
    BENCH_END(riscv_dot_prod_q7);
    ref_dot_prod_q7(q7_a_array, q7_b_array, ARRAY_SIZE, q31_out_ref_array);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_dot_prod_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dot_prod_q7);

    // q15_dot
    BENCH_START(riscv_dot_prod_q15);
    riscv_dot_prod_q15(q15_a_array, q15_b_array, ARRAY_SIZE, &q63_out_array);
    BENCH_END(riscv_dot_prod_q15);
    ref_dot_prod_q15(q15_a_array, q15_b_array, ARRAY_SIZE, &q63_out_ref_array);
    s = verify_results_q63(&q63_out_ref_array, &q63_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_dot_prod_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dot_prod_q15);

    // q31_dot
    BENCH_START(riscv_dot_prod_q31);
    riscv_dot_prod_q31(q31_a_array, q31_b_array, ARRAY_SIZE, &q63_out_array);
    BENCH_END(riscv_dot_prod_q31);
    ref_dot_prod_q31(q31_a_array, q31_b_array, ARRAY_SIZE, &q63_out_ref_array);
    s = verify_results_q63(&q63_out_ref_array, &q63_out_array, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_dot_prod_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dot_prod_q31);
#endif
}

static void riscv_mult_functions(void)
{
#ifdef MULT
    // f32_mult
    BENCH_START(riscv_mult_f32);
    riscv_mult_f32(f32_a_array, f32_b_array, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_mult_f32);
    ref_mult_f32(f32_a_array, f32_b_array, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_mult_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mult_f32);

    // riscv_mult_q7.c
    BENCH_START(riscv_mult_q7);
    riscv_mult_q7(q7_a_array, q7_b_array, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_mult_q7);
    ref_mult_q7(q7_a_array, q7_b_array, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_mult_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mult_q7);

    // riscv_mult_q15.c
    BENCH_START(riscv_mult_q15);
    riscv_mult_q15(q15_a_array, q15_b_array, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_mult_q15);
    ref_mult_q15(q15_a_array, q15_b_array, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_mult_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mult_q15);

    // riscv_mult_q31.c
    BENCH_START(riscv_mult_q31);
    riscv_mult_q31(q31_a_array, q31_b_array, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_mult_q31);
    ref_mult_q31(q31_a_array, q31_b_array, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_mult_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_mult_q31);
#endif

}

static void riscv_logical_functions(void)
{
#ifdef NEGATE
    // riscv_negate_f32.c
    BENCH_START(riscv_negate_f32);
    riscv_negate_f32(f32_a_array, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_negate_f32);
    ref_negate_f32(f32_a_array, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_negate_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_negate_f32);

    // riscv_negate_q7.c
    BENCH_START(riscv_negate_q7);
    riscv_negate_q7(q7_a_array, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_negate_q7);
    ref_negate_q7(q7_a_array, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(ref_negate_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(ref_negate_q7);

    // riscv_negate_q15.c
    BENCH_START(riscv_negate_q15);
    riscv_negate_q15(q15_a_array, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_negate_q15);
    ref_negate_q15(q15_a_array, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_negate_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_negate_q15);
    // riscv_negate_q31.c
    BENCH_START(riscv_negate_q31);
    riscv_negate_q31(q31_a_array, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_negate_q31);
    ref_negate_q31(q31_a_array, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_negate_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_negate_q31);
#endif

#ifdef AND
    BENCH_START(riscv_and_u8);
    riscv_and_u8((uint8_t *)q7_a_array, (uint8_t *)q7_b_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_and_u8);
    ref_and_u8((uint8_t *)q7_a_array, (uint8_t *)q7_b_array, (uint8_t *)q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_u8((uint8_t *)q7_out_ref_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_and_u8);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_and_u8);

    BENCH_START(riscv_and_u16);
    riscv_and_u16((uint16_t *)q15_a_array, (uint16_t *)q15_b_array, (uint16_t *)q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_and_u16);
    ref_and_u16((uint16_t *)q15_a_array, (uint16_t *)q15_b_array, (uint16_t *)q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_u16((uint16_t *)q15_out_ref_array, (uint16_t *)q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_and_u16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_and_u16);

    BENCH_START(riscv_and_u32);
    riscv_and_u32((uint32_t *)q31_a_array, (uint32_t *)q31_b_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_and_u32);
    ref_and_u32((uint32_t *)q31_a_array, (uint32_t *)q31_b_array, (uint32_t *)q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_u32((uint32_t *)q31_out_ref_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_and_u32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_and_u32);

#endif

#ifdef OR
    BENCH_START(riscv_or_u8);
    riscv_or_u8((uint8_t *)q7_a_array, (uint8_t *)q7_b_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_or_u8);
    ref_or_u8((uint8_t *)q7_a_array, (uint8_t *)q7_b_array, (uint8_t *)q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_u8((uint8_t *)q7_out_ref_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_or_u8);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_or_u8);

    BENCH_START(riscv_or_u16);
    riscv_or_u16((uint16_t *)q15_a_array, (uint16_t *)q15_b_array, (uint16_t *)q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_or_u16);
    ref_or_u16((uint16_t *)q15_a_array, (uint16_t *)q15_b_array, (uint16_t *)q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_u16((uint16_t *)q15_out_array, (uint16_t *)q15_out_ref_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_or_u16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_or_u16);

    BENCH_START(riscv_or_u32);
    riscv_or_u32((uint32_t *)q31_a_array, (uint32_t *)q31_b_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_or_u32);
    ref_or_u32((uint32_t *)q31_a_array, (uint32_t *)q31_b_array, (uint32_t *)q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_u32((uint32_t *)q31_out_ref_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_or_u32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_or_u32);
#endif

#ifdef NOT
    BENCH_START(riscv_not_u8);
    riscv_not_u8((uint8_t *)q7_a_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_not_u8);
    ref_not_u8((uint8_t *)q7_a_array, (uint8_t *)q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_u8((uint8_t *)q7_out_ref_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_not_u8);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_not_u8);

    BENCH_START(riscv_not_u16);
    riscv_not_u16((uint16_t *)q15_a_array, (uint16_t *)q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_not_u16);
    ref_not_u16((uint16_t *)q15_a_array, (uint16_t *)q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_u16((uint16_t *)q15_out_array, (uint16_t *)q15_out_ref_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_not_u16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_not_u16);

    BENCH_START(riscv_not_u32);
    riscv_not_u32((uint32_t *)q31_a_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_not_u32);
    ref_not_u32((uint32_t *)q31_a_array, (uint32_t *)q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_u32((uint32_t *)q31_out_ref_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_not_u32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_not_u32);
#endif

#ifdef XOR
    BENCH_START(riscv_xor_u8);
    riscv_xor_u8((uint8_t *)q7_a_array, (uint8_t *)q7_b_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_xor_u8);
    ref_xor_u8((uint8_t *)q7_a_array, (uint8_t *)q7_b_array, (uint8_t *)q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_u8((uint8_t *)q7_out_ref_array, (uint8_t *)q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_xor_u8);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_xor_u8);

    BENCH_START(riscv_xor_u16);
    riscv_xor_u16((uint16_t *)q15_a_array, (uint16_t *)q15_b_array, (uint16_t *)q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_xor_u16);
    ref_xor_u16((uint16_t *)q15_a_array, (uint16_t *)q15_b_array, (uint16_t *)q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_u16((uint16_t *)q15_out_ref_array, (uint16_t *)q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_xor_u16);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_xor_u16);

    BENCH_START(riscv_xor_u32);
    riscv_xor_u32((uint32_t *)q31_a_array, (uint32_t *)q31_b_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_xor_u32);
    ref_xor_u32((uint32_t *)q31_a_array, (uint32_t *)q31_b_array, (uint32_t *)q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_u32((uint32_t *)q31_out_ref_array, (uint32_t *)q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_xor_u32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_xor_u32);
#endif
}

static void riscv_offset_functions(void)
{
#ifdef OFFSET
    // riscv_offset_f32.c
    float32_t offset_f32;
    generate_rand_f32(&offset_f32, 1);
    BENCH_START(riscv_offset_f32);
    riscv_offset_f32(f32_a_array, offset_f32, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_offset_f32);
    ref_offset_f32(f32_a_array, offset_f32, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_offset_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_offset_f32);

    // riscv_offset_q7.c
    q7_t offset_q7;
    generate_rand_q7(&offset_q7, 1);
    BENCH_START(riscv_offset_q7);
    riscv_offset_q7(q7_a_array, offset_q7, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_offset_q7);
    ref_offset_q7(q7_a_array, offset_q7, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_offset_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_offset_q7);

    // riscv_offset_q15.c
    q15_t offset_q15;
    generate_rand_q15(&offset_q15, 1);
    BENCH_START(riscv_offset_q15);
    riscv_offset_q15(q15_a_array, offset_q15, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_offset_q15);
    ref_offset_q15(q15_a_array, offset_q15, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_offset_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_offset_q15);
    // riscv_offset_q31.c
    q31_t offset_q31;
    generate_rand_q31(&offset_q31, 1);
    BENCH_START(riscv_offset_q31);
    riscv_offset_q31(q31_a_array, offset_q31, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_offset_q31);
    ref_offset_q31(q31_a_array, offset_q31, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_offset_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_offset_q31);
#endif

}

static void riscv_scale_functions(void)
{
#ifdef SCALE
    // *************************************   scale ***************************
    // f32_scale
    float32_t scale_f32;
    generate_rand_f32(&scale_f32, 1);
    BENCH_START(riscv_scale_f32);
    riscv_scale_f32(f32_a_array, scale_f32, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_scale_f32);
    ref_scale_f32(f32_a_array, scale_f32, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_scale_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_scale_f32);

    // riscv_scale_q7.c
    q7_t scaleFract_q7 = -2;
    int8_t shift_q7 = 9;
    generate_rand_q7(&scaleFract_q7, 1);
    BENCH_START(riscv_scale_q7);
    riscv_scale_q7(q7_a_array, scaleFract_q7, shift_q7, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_scale_q7);
    ref_scale_q7(q7_a_array, scaleFract_q7, shift_q7, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_scale_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_scale_q7);

    // riscv_scale_q15.c
    q15_t scaleFract_q15 = -2;
    int8_t shift_q15 = 17;
    generate_rand_q15(&scaleFract_q15, 1);
    BENCH_START(riscv_scale_q15);
    riscv_scale_q15(q15_a_array, scaleFract_q15, shift_q15, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_scale_q15);
    ref_scale_q15(q15_a_array, scaleFract_q15, shift_q15, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_scale_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_scale_q15);
    // riscv_scale_q31.c
    q31_t scaleFract_q31 = -1;
    generate_rand_q31(&scaleFract_q31, 1);
    int8_t shift_q31 = -1;
    BENCH_START(riscv_scale_q31);
    riscv_scale_q31(q31_a_array, scaleFract_q31, shift_q31, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_scale_q31);
    ref_scale_q31(q31_a_array, scaleFract_q31, shift_q31, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_scale_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_scale_q31);
#endif

}

static void riscv_shift_functions(void)
{
#ifdef SHIFT
    // riscv_shift_q7.c
    int8_t shiftBits_q7 = -1;
    BENCH_START(riscv_shift_q7);
    riscv_shift_q7(q7_a_array, shiftBits_q7, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_shift_q7);
    ref_shift_q7(q7_a_array, shiftBits_q7, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_shift_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_shift_q7);

    // riscv_shift_q15.c
    int8_t shiftBits_q15 = -3;
    BENCH_START(riscv_shift_q15);
    riscv_shift_q15(q15_a_array, shiftBits_q15, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_shift_q15);
    ref_shift_q15(q15_a_array, shiftBits_q15, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_shift_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_shift_q15);

    // riscv_shift_q31.c
    int8_t shiftBits_q31 = -3;
    BENCH_START(riscv_shift_q31);
    riscv_shift_q31(q31_a_array, shiftBits_q31, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_shift_q31);
    ref_shift_q31(q31_a_array, shiftBits_q31, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_shift_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_shift_q31);
#endif
}

static void riscv_sub_functions(void)
{
#ifdef SUB
    // riscv_sub_f32.c
    BENCH_START(riscv_sub_f32);
    riscv_sub_f32(f32_a_array, f32_b_array, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_sub_f32);
    ref_sub_f32(f32_a_array, f32_b_array, f32_out_ref_array, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_sub_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sub_f32);

    // riscv_sub_q7.c
    BENCH_START(riscv_sub_q7);
    riscv_sub_q7(q7_a_array, q7_b_array, q7_out_array, ARRAY_SIZE);
    BENCH_END(riscv_sub_q7);
    ref_sub_q7(q7_a_array, q7_b_array, q7_out_ref_array, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_sub_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sub_q7);

    // riscv_sub_q15.c
    BENCH_START(riscv_sub_q15);
    riscv_sub_q15(q15_a_array, q15_b_array, q15_out_array, ARRAY_SIZE);
    BENCH_END(riscv_sub_q15);
    ref_sub_q15(q15_a_array, q15_b_array, q15_out_ref_array, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_sub_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sub_q15);

    // riscv_sub_q31.c
    BENCH_START(riscv_sub_q31);
    riscv_sub_q31(q31_a_array, q31_b_array, q31_out_array, ARRAY_SIZE);
    BENCH_END(riscv_sub_q31);
    ref_sub_q31(q31_a_array, q31_b_array, q31_out_ref_array, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_sub_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sub_q31);
#endif
}

static void riscv_clip_functions(void)
{
#ifdef CLIP
    float32_t low_f32 = -4230.37;
    float32_t high_f32 = 4230.45;
    BENCH_START(riscv_clip_f32);
    riscv_clip_f32(f32_a_array, f32_out_array, low_f32, high_f32, ARRAY_SIZE);
    BENCH_END(riscv_clip_f32);
    ref_clip_f32(f32_a_array, f32_out_ref_array, low_f32, high_f32, ARRAY_SIZE);
    s = verify_results_f32(f32_out_ref_array, f32_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_clip_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_clip_f32);

    q7_t low_q7 = -100;
    q7_t high_q7 = 100;
    BENCH_START(riscv_clip_q7);
    riscv_clip_q7(q7_a_array, q7_out_array, low_q7, high_q7, ARRAY_SIZE);
    BENCH_END(riscv_clip_q7);
    ref_clip_q7(q7_a_array, q7_out_ref_array, low_q7, high_q7, ARRAY_SIZE);
    s = verify_results_q7(q7_out_ref_array, q7_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_clip_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_clip_q7);

    q15_t low_q15 = -2638;
    q15_t high_q15 = 1000;
    BENCH_START(riscv_clip_q15);
    riscv_clip_q15(q15_a_array, q15_out_array, low_q15, high_q15, ARRAY_SIZE);
    BENCH_END(riscv_clip_q15);
    ref_clip_q15(q15_a_array, q15_out_ref_array, low_q15, high_q15, ARRAY_SIZE);
    s = verify_results_q15(q15_out_ref_array, q15_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_clip_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_clip_q15);

    q31_t low_q31 = -214712319;
    q31_t high_q31 = 214712319;
    BENCH_START(riscv_clip_q31);
    riscv_clip_q31(q31_a_array, q31_out_array, low_q31, high_q31, ARRAY_SIZE);
    BENCH_END(riscv_clip_q31);
    ref_clip_q31(q31_a_array, q31_out_ref_array, low_q31, high_q31, ARRAY_SIZE);
    s = verify_results_q31(q31_out_ref_array, q31_out_array, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_clip_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_clip_q31);
#endif

}

int main(void)
{

    BENCH_INIT();

    generate_rand_q7(q7_a_array, ARRAY_SIZE);
    generate_rand_q7(q7_b_array, ARRAY_SIZE);
    generate_rand_q15(q15_a_array, ARRAY_SIZE);
    generate_rand_q15(q15_b_array, ARRAY_SIZE);
    generate_rand_q31(q31_a_array, ARRAY_SIZE);
    generate_rand_q31(q31_b_array, ARRAY_SIZE);
    generate_rand_f32(f32_a_array, ARRAY_SIZE);
    generate_rand_f32(f32_b_array, ARRAY_SIZE);

    riscv_abs_functions();
    riscv_add_functions();
    riscv_dot_prod_functions();
    riscv_mult_functions();
    riscv_logical_functions();
    riscv_offset_functions();
    riscv_scale_functions();
    riscv_shift_functions();
    riscv_sub_functions();
    riscv_clip_functions();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
