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

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include "../common.h"

#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

#define ARRAY_SIZE 100

int test_flag_error = 0;

/*
*********************************************************************************************************
*	? ? ?: DSP_Copy
*	????: ????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static int DSP_Copy(void)
{
    /* clang-format off */
	float32_t pSrc[ARRAY_SIZE] = {0.6557,  0.0357,  0.8491,  0.9340, 0.6787,  0.7577,  0.7431,  0.3922,  0.6555,  0.1712};
	float32_t pDst[ARRAY_SIZE];
	float32_t pDst_ref[ARRAY_SIZE];
	uint32_t pIndex;
    int i;

	q31_t pSrc1[ARRAY_SIZE] = {1481765933, 1085377743,1270216262,  1191391529,812669700, 553475508, 445349752, 1344887256,
						730417256, 1812158119};
	q31_t pDst1[ARRAY_SIZE];
	q31_t pDst1_ref[ARRAY_SIZE];

	q15_t pSrc2[ARRAY_SIZE] = {14335,21567, 9515,15089, 16892, 28633, 12154, 1942, 24073,28716};
	q15_t pDst2[ARRAY_SIZE];
	q15_t pDst2_ref[ARRAY_SIZE];

	q7_t pSrc3[ARRAY_SIZE] = {37,  87, 116, 100, 68, 47,21, 40, 36, 105};
	q7_t pDst3[ARRAY_SIZE];
	q7_t pDst3_ref[ARRAY_SIZE];
    /* clang-format on */

    BENCH_START(riscv_copy_f32);
    riscv_copy_f32(pSrc, pDst, ARRAY_SIZE);
    BENCH_END(riscv_copy_f32);
    ref_copy_f32(pSrc, pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(pDst[i] - pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_copy_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, pDst_ref[i],
                   pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_copy_f32);

    /*****************************************************************/
    BENCH_START(riscv_copy_q31);
    riscv_copy_q31(pSrc1, pDst1, ARRAY_SIZE);
    BENCH_END(riscv_copy_q31);
    ref_copy_q31(pSrc1, pDst1_ref, ARRAY_SIZE);

    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst1_ref[i] - pDst1[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_copy_q31);
            printf("index: %d,expect: %d, actual: %d\n", i, pDst1_ref[i],
                   pDst1[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_copy_q31);

    /*****************************************************************/
    BENCH_START(riscv_copy_q15);
    riscv_copy_q15(pSrc2, pDst2, ARRAY_SIZE);
    BENCH_END(riscv_copy_q15);
    ref_copy_q15(pSrc2, pDst2_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst2_ref[i] - pDst2[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_copy_q15);
            printf("index: %d,expect: %d, actual: %d\n", pDst2_ref[i], pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_copy_q15);

    /*****************************************************************/
    BENCH_START(riscv_copy_q7);
    riscv_copy_q7(pSrc3, pDst3, ARRAY_SIZE);
    BENCH_END(riscv_copy_q7);
    ref_copy_q7(pSrc3, pDst3_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst3_ref[i] - pDst3[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_copy_q7);
            printf("index: %d,expect: %f, actual: %f\n", i, pDst3_ref[i],
                   pDst3[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_copy_q7);

    /*****************************************************************/
    printf("all copy tests are passed,well done!\n");
}

/*
*********************************************************************************************************
*	? ? ?: DSP_Fill
*	????: ????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static int DSP_Fill(void)
{
    float32_t pDst[ARRAY_SIZE];
    float32_t pDst_ref[ARRAY_SIZE];
    uint32_t pIndex;
    int i;

    q31_t pDst1[ARRAY_SIZE];
    q31_t pDst1_ref[ARRAY_SIZE];

    q15_t pDst2[ARRAY_SIZE];
    q15_t pDst2_ref[ARRAY_SIZE];

    q7_t pDst3[ARRAY_SIZE];
    q7_t pDst3_ref[ARRAY_SIZE];

    BENCH_START(riscv_fill_f32);
    riscv_fill_f32(3.33f, pDst, ARRAY_SIZE);
    BENCH_END(riscv_fill_f32);
    ref_fill_f32(3.33f, pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(pDst[i] - pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_fill_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, pDst_ref[i],
                   pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_fill_f32);

    /*****************************************************************/
    BENCH_START(riscv_fill_q31);
    riscv_fill_q31(0x11111111, pDst1, ARRAY_SIZE);
    BENCH_END(riscv_fill_q31);
    ref_fill_q31(0x11111111, pDst1_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst1_ref[i] - pDst1[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_fill_q31);
            printf("index: %d,expect: %d, actual: %d\n", i, pDst1_ref[i],
                   pDst1[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_fill_q31);

    /*****************************************************************/
    BENCH_START(riscv_fill_q15);
    riscv_fill_q15(0x1111, pDst2, ARRAY_SIZE);
    BENCH_END(riscv_fill_q15);
    ref_fill_q15(0x1111, pDst2_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst2_ref[i] - pDst2[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_fill_q15);
            printf("index: %d,expect: %d, actual: %d\n", i, pDst2_ref[i],
                   pDst2[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_fill_q15);

    /*****************************************************************/
    BENCH_START(riscv_fill_q7);
    riscv_fill_q7(0x11, pDst3, ARRAY_SIZE);
    BENCH_END(riscv_fill_q7);
    ref_fill_q7(0x11, pDst3_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst3_ref[i] - pDst3[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_fill_q7);
            printf("index: %d,expect: %d, actual: %d\n", i, pDst3_ref[i],
                   pDst3[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_fill_q7);

    /*****************************************************************/
    printf("all fill tests are passed,well done!\n");
}

/*
*********************************************************************************************************
*	? ? ?: DSP_FloatToFix
*	????: ???????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static int DSP_FloatToFix(void)
{
    float32_t pSrc[ARRAY_SIZE] = {0.6557, 0.0357, 0.8491, 0.9340, 0.6787,
                                  0.7577, 0.7431, 0.3922, 0.6555, 0.1712};
    uint32_t pIndex;
    int i;

    q31_t pDst1[ARRAY_SIZE];
    q31_t pDst1_ref[ARRAY_SIZE];
    q15_t pDst2[ARRAY_SIZE];
    q15_t pDst2_ref[ARRAY_SIZE];
    q7_t pDst3[ARRAY_SIZE];
    q7_t pDst3_ref[ARRAY_SIZE];

    /*****************************************************************/
    BENCH_START(riscv_float_to_q31);
    riscv_float_to_q31(pSrc, pDst1, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q31);
    ref_float_to_q31(pSrc, pDst1_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst1_ref[i] - pDst1[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_float_to_q31);
            printf("index: %d,expect: %d, actual: %d\n", i, pDst1_ref[i],
                   pDst1[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_float_to_q31);

    /*****************************************************************/
    BENCH_START(riscv_float_to_q15);
    riscv_float_to_q15(pSrc, pDst2, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q15);
    ref_float_to_q15(pSrc, pDst2_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst2_ref[i] - pDst2[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_float_to_q15);
            printf("index: %d,index: %d, expect: %d, actual: %d\n", i,
                   pDst2_ref[i], pDst2[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_float_to_q15);

    /*****************************************************************/
    BENCH_START(riscv_float_to_q7);
    riscv_float_to_q7(pSrc, pDst3, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q7);
    ref_float_to_q7(pSrc, pDst3_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst3_ref[i] - pDst3[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_float_to_q7);
            printf("expect: %d, actual: %d\n", pDst3_ref[i], pDst3[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_float_to_q7);

    /*****************************************************************/
    printf("all float to fix tests are passed,well done!\n");
}

/*
*********************************************************************************************************
*	? ? ?: DSP_Q7
*	????: Q7???????????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static int DSP_Q7(void)
{
    float32_t pDst[ARRAY_SIZE];
    float32_t pDst_ref[ARRAY_SIZE];
    int i;

    q31_t pDst1[ARRAY_SIZE];
    q31_t pDst1_ref[ARRAY_SIZE];
    q15_t pDst2[ARRAY_SIZE];
    q15_t pDst2_ref[ARRAY_SIZE];
    q7_t pSrc[ARRAY_SIZE] = {45, 79, 70, 41, 4, 52, 120, 88, 104, 39};

    BENCH_START(riscv_q7_to_float);
    riscv_q7_to_float(pSrc, pDst, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_float);
    ref_q7_to_float(pSrc, pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(pDst[i] - pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_q7_to_float);
            printf("expect: %f, actual: %f\n", pDst_ref[i], pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q7_to_float);

    /*****************************************************************/
    BENCH_START(riscv_q7_to_q31);
    riscv_q7_to_q31(pSrc, pDst1, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_q31);
    ref_q7_to_q31(pSrc, pDst1_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst1[i] - pDst1_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_q7_to_q31);
            printf("expect: %d, actual: %d\n", pDst1_ref[i], pDst1[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q7_to_q31);

    /*****************************************************************/
    BENCH_START(riscv_q7_to_q15);
    riscv_q7_to_q15(pSrc, pDst2, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_q15);
    ref_q7_to_q15(pSrc, pDst2_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst2[i] - pDst2_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_q7_to_q15);
            printf("expect: %d, actual: %d\n", pDst2_ref[i], pDst2[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q7_to_q15);

    /*****************************************************************/
    printf("all q7 to others tests are passed,well done!\n");
}

/*
*********************************************************************************************************
*	? ? ?: DSP_Q15
*	????: Q15???????????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static int DSP_Q15(void)
{
    float32_t pDst[ARRAY_SIZE];
    float32_t pDst_ref[ARRAY_SIZE];
    q15_t pSrc[ARRAY_SIZE] = {14701, 10651, 22402, 17005, 518,
                              8222,  13968, 24166, 31278, 32307};
    int i;
    q31_t pDst1[ARRAY_SIZE];
    q31_t pDst1_ref[ARRAY_SIZE];
    q7_t pDst2[ARRAY_SIZE];
    q7_t pDst2_ref[ARRAY_SIZE];

    BENCH_START(riscv_q15_to_float);
    riscv_q15_to_float(pSrc, pDst, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_float);
    ref_q15_to_float(pSrc, pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(pDst[i] - pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_q15_to_float);
            printf("expect: %f, actual: %f\n", pDst_ref[i], pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q15_to_float);

    /*****************************************************************/
    BENCH_START(riscv_q15_to_q31);
    riscv_q15_to_q31(pSrc, pDst1, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_q31);
    ref_q15_to_q31(pSrc, pDst1_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst1[i] - pDst1_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_q15_to_q31);
            printf("expect: %d, actual: %d\n", pDst1_ref[i], pDst1[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q15_to_q31);

    /*****************************************************************/
    BENCH_START(riscv_q15_to_q7);
    riscv_q15_to_q7(pSrc, pDst2, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_q7);
    ref_q15_to_q7(pSrc, pDst2_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst2[i] - pDst2_ref[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_q15_to_q7);
            printf("expect: %d, actual: %d\n", pDst2_ref[i], pDst2[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q15_to_q7);

    /*****************************************************************/
    printf("all q15 to others tests are passed,well done!\n");
}

/*
*********************************************************************************************************
*	? ? ?: DSP_Q31
*	????: Q31???????????
*	?    ?:?
*	? ? ?: ?
*********************************************************************************************************
*/
static int DSP_Q31(void)
{
    float32_t pDst[ARRAY_SIZE];
    float32_t pDst_ref[ARRAY_SIZE];

    q31_t pSrc[ARRAY_SIZE] = {1873226917, 715669847, 1486937972, 1196032868,
                              777206980,  68706223,  1843638549, 212567592,
                              1883488164, 964776169};
    q15_t pDst1[ARRAY_SIZE];
    q15_t pDst1_ref[ARRAY_SIZE];
    q7_t pDst2[ARRAY_SIZE];
    q7_t pDst2_ref[ARRAY_SIZE];
    int i;

    BENCH_START(riscv_q31_to_float);
    riscv_q31_to_float(pSrc, pDst, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_float);
    ref_q31_to_float(pSrc, pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(pDst[i] - pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_q31_to_float);
            printf("expect: %f, actual: %f\n", pDst_ref[i], pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q31_to_float);

    /*****************************************************************/
    BENCH_START(riscv_q31_to_q15);
    riscv_q31_to_q15(pSrc, pDst1, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_q15);
    ref_q31_to_q15(pSrc, pDst1_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst1[i] - pDst1_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_q31_to_q15);
            printf("expect: %d, actual: %d\n", pDst1_ref[i], pDst1[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q31_to_q15);

    /*****************************************************************/
    BENCH_START(riscv_q31_to_q7);
    riscv_q31_to_q7(pSrc, pDst2, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_q7);
    ref_q31_to_q7(pSrc, pDst2_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(pDst2[i] - pDst2_ref[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_q31_to_q7);
            printf("expect: %d, actual: %d\n", pDst2_ref[i], pDst2[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_q31_to_q7);

    /*****************************************************************/
    printf("all q31 to others tests are passed,well done!\n");
}

int main()
{
    BENCH_INIT;
    DSP_Q7();
    DSP_Q15();
    DSP_Q31();
    DSP_Fill();
    DSP_Copy();
    DSP_FloatToFix();
    BENCH_FINISH;
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
