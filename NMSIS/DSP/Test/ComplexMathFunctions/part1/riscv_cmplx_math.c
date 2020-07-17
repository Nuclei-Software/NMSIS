//
// Created by lujun on 19-6-28.
//
// This contains complx calculate in Conjugate, Dot Product, Magnitude,
// Magnitude Squared, by-Complex Multiplication, by-Real Multiplication each one
// has it's own function. All function can be found in main function. If you
// don't want to use it, then comment it. contains following functions,eachone
// includes q15,q31 and f32 versions
#include "../common.h"
#include "riscv_math.h"
#include <stdint.h>

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#include <stdlib.h>
#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

#define ARRAY_SIZE 30

int test_flag_error = 0;

/*
*********************************************************************************************************
*	DSP_MagSquared
*********************************************************************************************************
*/
static int DSP_MagSquared(void)
{
    uint8_t i;
    /* clang-format off */
	float32_t f32_pSrc[ARRAY_SIZE*2] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
	float32_t f32_pDst[ARRAY_SIZE];
	float32_t f32_pDst_ref[ARRAY_SIZE];

	q31_t q31_pSrc[ARRAY_SIZE*2] = {263084,268304,5336008,536308,803052,800352,107036,107036,1033420,13304320};
	q31_t q31_pDst[ARRAY_SIZE];
	q31_t q31_pDst_ref[ARRAY_SIZE];

	q15_t q15_pSrc[ARRAY_SIZE*2] = {1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000};
	q15_t q15_pDst[ARRAY_SIZE];
	q15_t q15_pDst_ref[ARRAY_SIZE];
    /* clang-format on */

    /**********************************************************************************/
    BENCH_START(riscv_cmplx_mag_squared_f32);
    riscv_cmplx_mag_squared_f32(f32_pSrc, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_squared_f32);
    ref_cmplx_mag_squared_f32(f32_pSrc, f32_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(f32_pDst[i] - f32_pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_cmplx_mag_squared_f32);
            printf("index: %d, expect: %f, actual: %f\n", i, f32_pDst_ref[i],
                   f32_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mag_squared_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_squared_q31);
    riscv_cmplx_mag_squared_q31(q31_pSrc, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_squared_q31);
    ref_cmplx_mag_squared_q31(q31_pSrc, q31_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (labs(q31_pDst[i] - q31_pDst_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_cmplx_mag_squared_q31);
            printf("index: %d, expect: %x, actual: %x\n", i, q31_pDst_ref[i],
                   q31_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mag_squared_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_squared_q15);
    riscv_cmplx_mag_squared_q15(q15_pSrc, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_squared_q15);
    ref_cmplx_mag_squared_q15(q15_pSrc, q15_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(q15_pDst[i] - q15_pDst_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_cmplx_mag_squared_q15);
            printf("index: %d, expect: %x, actual: %x\n", i, q15_pDst_ref[i],
                   q15_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mag_squared_q15);
}
/*
*********************************************************************************************************
* DSP_CmplxMult
*
*
*
*********************************************************************************************************
*/
static int DSP_CmplxMult(void)
{
    uint8_t i;
    /* clang-format off */
	float32_t f32_pSrcA[ARRAY_SIZE*2] = {1.1f, 1.2f, 2.1f, 2.2f, 3.1f, 3.2f, 4.1f, 4.2f, 5.1f, 5.2f};
	float32_t f32_pSrcB[ARRAY_SIZE*2] = {1.2f, 1.2f, 2.2f, 2.2f, 3.2f, 3.2f, 4.2f, 4.2f, 5.2f, 5.2f};
	float32_t f32_pDst[ARRAY_SIZE];
	float32_t f32_pDst_ref[ARRAY_SIZE];

	q31_t q31_pSrcA[ARRAY_SIZE*2] = {260084,268004,536008,536008,805002,800052,1070360,1073006,1300420,1349020};
	q31_t q31_pSrcB[ARRAY_SIZE*2] = {260084,260804,536008,530608,800502,800052,1070036,1007036,1304200,1304920};
	q31_t q31_pDst[ARRAY_SIZE];
	q31_t q31_pDst_ref[ARRAY_SIZE];

	q15_t q15_pSrcA[ARRAY_SIZE*2] = {520, 1300, 1045, 2400, 2530, 450, 1060, 1035, 2060, 2650};
	q15_t q15_pSrcB[ARRAY_SIZE*2] = {250, 1010, 1510, 2100, 2105, 550, 1030, 1560, 2010, 2305};
	q15_t q15_pDst[ARRAY_SIZE];
	q15_t q15_pDst_ref[ARRAY_SIZE];
    /* clang-format on */

    /**********************************************************************************/
    BENCH_START(riscv_cmplx_mult_cmplx_f32);
    riscv_cmplx_mult_cmplx_f32(f32_pSrcA, f32_pSrcB, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_cmplx_f32);
    ref_cmplx_mult_cmplx_f32(f32_pSrcA, f32_pSrcB, f32_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(f32_pDst[i] - f32_pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_cmplx_mult_cmplx_f32);
            printf("index: %d, expect: %f, actual: %f\n", i, f32_pDst_ref[i],
                   f32_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mult_cmplx_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_cmplx_q31);
    riscv_cmplx_mult_cmplx_q31(q31_pSrcA, q31_pSrcB, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_cmplx_q31);
    ref_cmplx_mult_cmplx_q31(q31_pSrcA, q31_pSrcB, q31_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (labs(q31_pDst[i] - q31_pDst_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_cmplx_mult_cmplx_q31);
            printf("index: %d, expect: %x, actual: %x\n", i, q31_pDst_ref[i],
                   q31_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mult_cmplx_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_cmplx_q15);
    riscv_cmplx_mult_cmplx_q15(q15_pSrcA, q15_pSrcB, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_cmplx_q15);
    ref_cmplx_mult_cmplx_q15(q15_pSrcA, q15_pSrcB, q15_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(q15_pDst[i] - q15_pDst_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_cmplx_mult_cmplx_q15);
            printf("index: %d, expect: %x, actual: %x\n", i, q15_pDst_ref[i],
                   q15_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mult_cmplx_q15);
}

/*
*********************************************************************************************************
* DSP_CmplxMultReal
*
*
*
*********************************************************************************************************
*/
static int DSP_CmplxMultReal(void)
{
    uint8_t i;
    /* clang-format off */
	float32_t f32_pSrcCmplx[ARRAY_SIZE*2] = {1.1f, 1.2f, 2.1f, 2.2f, 3.1f, 3.2f, 4.1f, 4.2f, 5.1f, 5.2f};
	float32_t f32_pSrcReal[ARRAY_SIZE] = {1.2f, 1.2f, 2.2f, 2.2f, 3.2f};
	float32_t f32_pCmplxDst[ARRAY_SIZE*2];
	float32_t f32_pCmplxDst_ref[ARRAY_SIZE*2];

	q31_t q31_pSrcCmplx[ARRAY_SIZE*2] = {1 * 2684, 1 * 2684, 2 * 2684, 2 * 2684, 3 * 2684, 3 * 2684, 4 * 2684, 4 * 2684, 5 * 2684, 5 * 2684};
	q31_t q31_pSrcReal[ARRAY_SIZE] = {3, 5, 7, 9, 11};
	q31_t q31_pCmplxDst[ARRAY_SIZE*2];
	q31_t q31_pCmplxDst_ref[ARRAY_SIZE*2];

	q15_t q15_pSrcCmplx[ARRAY_SIZE*2] = {-164, -166, 270, 270, 302, 331, 112, 123, 114, 215};
	q15_t q15_pSrcReal[ARRAY_SIZE] = {38079, 15880, 16490, 25405, 11024};
	q15_t q15_pCmplxDst[ARRAY_SIZE*2];
	q15_t q15_pCmplxDst_ref[ARRAY_SIZE*2];
    /* clang-format on */

    /**********************************************************************************/
    BENCH_START(riscv_cmplx_mult_real_f32);
    riscv_cmplx_mult_real_f32(f32_pSrcCmplx, f32_pSrcReal, f32_pCmplxDst,
                            ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_real_f32);
    ref_cmplx_mult_real_f32(f32_pSrcCmplx, f32_pSrcReal, f32_pCmplxDst_ref,
                            ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE * 2; i++) {
        if (fabs(f32_pCmplxDst[i] - f32_pCmplxDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_cmplx_mult_real_f32);
            printf("index: %d, expect: %f, actual: %f\n", i,
                   f32_pCmplxDst_ref[i], f32_pCmplxDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mult_real_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_real_q31);
    riscv_cmplx_mult_real_q31(q31_pSrcCmplx, q31_pSrcReal, q31_pCmplxDst,
                            ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_real_q31);
    ref_cmplx_mult_real_q31(q31_pSrcCmplx, q31_pSrcReal, q31_pCmplxDst_ref,
                            ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE * 2; i++) {
        if (labs(q31_pCmplxDst[i] - q31_pCmplxDst_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_cmplx_mult_real_q31);
            printf("index: %d, expect: %x, actual: %x\n", i,
                   q31_pCmplxDst_ref[i], q31_pCmplxDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mult_real_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_real_q15);
    riscv_cmplx_mult_real_q15(q15_pSrcCmplx, q15_pSrcReal, q15_pCmplxDst,
                            ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_real_q15);
    ref_cmplx_mult_real_q15(q15_pSrcCmplx, q15_pSrcReal, q15_pCmplxDst_ref,
                            ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE * 2; i++) {
        if (abs(q15_pCmplxDst[i] - q15_pCmplxDst_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_cmplx_mult_real_q15);
            printf("index: %d, expect: %x, actual: %x\n", i,
                   q15_pCmplxDst_ref[i], q15_pCmplxDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mult_real_q15);
}

/*
*********************************************************************************************************
* DSP_CONJ
*
*
*
*********************************************************************************************************
*/
static int DSP_CONJ(void)
{
    uint8_t i;

    /* clang-format off */

	float32_t f32_pSrc[ARRAY_SIZE*2] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
	float32_t f32_pDst[ARRAY_SIZE*2];
	float32_t f32_pDst_ref[ARRAY_SIZE*2];

	q31_t q31_pSrc[ARRAY_SIZE*2] = {1, -1, -2, 2, 3, -3, -4, 4, 5, 5,6,6,7,7,8,8};
	q31_t q31_pDst[ARRAY_SIZE*2];
	q31_t q31_pDst_ref[ARRAY_SIZE*2];

	q15_t q15_pSrc[ARRAY_SIZE*2] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5,6,6,7,7,8,8};
	q15_t q15_pDst[ARRAY_SIZE*2];
	q15_t q15_pDst_ref[ARRAY_SIZE*2];
    /* clang-format on */

    /**********************************************************************************/
    BENCH_START(riscv_cmplx_conj_f32);
    riscv_cmplx_conj_f32(f32_pSrc, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_conj_f32);
    ref_cmplx_conj_f32(f32_pSrc, f32_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(f32_pDst[i] - f32_pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_cmplx_conj_f32);
            printf("index: %d, expect: %f, actual: %f\n", i, f32_pDst_ref[i],
                   f32_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_conj_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_conj_q31);
    riscv_cmplx_conj_q31(q31_pSrc, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_conj_q31);
    ref_cmplx_conj_q31(q31_pSrc, q31_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (labs(q31_pDst[i] - q31_pDst_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_cmplx_conj_q31);
            printf("index: %d, expect: %x, actual: %x\n", i, q31_pDst_ref[i],
                   q31_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_conj_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_conj_q15);
    riscv_cmplx_conj_q15(q15_pSrc, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_conj_q15);
    ref_cmplx_conj_q15(q15_pSrc, q15_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(q15_pDst[i] - q15_pDst_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_cmplx_conj_q15);
            printf("index: %d, expect: %x, actual: %x\n", i, q15_pDst_ref[i],
                   q15_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_conj_q15);
}

/*
*********************************************************************************************************
* DSP_CmplxDotProduct
*
*
*
*********************************************************************************************************
*/
static int DSP_CmplxDotProduct(void)
{
    /* clang-format off */

	float32_t f32_pSrcA[ARRAY_SIZE*2] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
	float32_t f32_pSrcB[ARRAY_SIZE*2] = {1.1f, 1.1f, 2.1f, 2.1f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
	float32_t f32_realResult;
	float32_t f32_imagResult;
	float32_t f32_realResult_ref;
	float32_t f32_imagResult_ref;

	q31_t q31_pSrcA[ARRAY_SIZE*2] = {1 * 2684356, 1 * 2684356, 2 * 2684356, 2 * 2684356, 3 * 2684356, 3 * 2684356,
						4 * 2684356, 4 * 2684356, 5 * 2684356, 5 * 2684356};
	q31_t q31_pSrcB[ARRAY_SIZE*2] = {1 * 2635456, 1 * 2685456, 2 * 2684356, 2 * 2684356, 3 * 2684356, 3 * 2684356,
						4 * 2684456, 4 * 2635456, 5 * 2684356, 5 * 2684356};
	q63_t q31_realResult;
	q63_t q31_imagResult;
	q63_t q31_realResult_ref;
	q63_t q31_imagResult_ref;

	q15_t q15_pSrcA[ARRAY_SIZE*2] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
	q15_t q15_pSrcB[ARRAY_SIZE*2] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
	q31_t q15_realResult;
	q31_t q15_imagResult;
	q31_t q15_realResult_ref;
	q31_t q15_imagResult_ref;
    /* clang-format on */

    /**********************************************************************************/
    BENCH_START(riscv_cmplx_dot_prod_f32);
    riscv_cmplx_dot_prod_f32(f32_pSrcA, f32_pSrcB, ARRAY_SIZE, &f32_realResult,
                           &f32_imagResult);
    BENCH_END(riscv_cmplx_dot_prod_f32);
    ref_cmplx_dot_prod_f32(f32_pSrcA, f32_pSrcB, ARRAY_SIZE,
                           &f32_realResult_ref, &f32_imagResult_ref);
    if ((fabs(f32_realResult - f32_realResult_ref) > DELTAF32) ||
        (fabs(f32_imagResult - f32_imagResult_ref) > DELTAF32)) {
        BENCH_ERROR(riscv_cmplx_dot_prod_f32);
        printf("real expect: %f, actual: %f\nimag expect: %f, actual: %f\n",
               f32_realResult_ref, f32_realResult, f32_imagResult_ref,
               f32_imagResult);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_dot_prod_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_dot_prod_q31);
    riscv_cmplx_dot_prod_q31(q31_pSrcA, q31_pSrcB, ARRAY_SIZE, &q31_realResult,
                           &q31_imagResult);
    BENCH_END(riscv_cmplx_dot_prod_q31);
    ref_cmplx_dot_prod_q31(q31_pSrcA, q31_pSrcB, ARRAY_SIZE,
                           &q31_realResult_ref, &q31_imagResult_ref);
    if ((labs(q31_realResult - q31_realResult_ref) > DELTAQ31) ||
        (labs(q31_imagResult - q31_imagResult_ref) > DELTAQ31)) {
        BENCH_ERROR(riscv_cmplx_dot_prod_q31);
        printf("real expect: %x, actual: %x\nimag expect: %x, actual: %x\n",
               q31_realResult_ref, q31_realResult, q31_imagResult_ref,
               q31_imagResult);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_dot_prod_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_dot_prod_q15);
    riscv_cmplx_dot_prod_q15(q15_pSrcA, q15_pSrcB, ARRAY_SIZE, &q15_realResult,
                           &q15_imagResult);
    BENCH_END(riscv_cmplx_dot_prod_q15);
    ref_cmplx_dot_prod_q15(q15_pSrcA, q15_pSrcB, ARRAY_SIZE,
                           &q15_realResult_ref, &q15_imagResult_ref);
    if ((abs(q15_realResult - q15_realResult_ref) > DELTAQ15) ||
        (abs(q15_imagResult - q15_imagResult_ref) > DELTAQ15)) {
        BENCH_ERROR(riscv_cmplx_dot_prod_q15);
        printf("real expect: %x, actual: %x\nimag expect: %x, actual: %x\n",
               q15_realResult_ref, q15_realResult, q15_imagResult_ref,
               q15_imagResult);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_dot_prod_q15);
}

/*
*********************************************************************************************************
* DSP_CmplxMag
*
*
*
*********************************************************************************************************
*/
static int DSP_CmplxMag(void)
{
    uint8_t i;

    /* clang-format off */

	float32_t f32_pSrc[ARRAY_SIZE*2] = {1.1f, 1.1f, 6.3f, 8.4f, 3.1f, 3.1f, 4.1f, 4.1f, 5.1f, 5.1f};
	float32_t f32_pDst[ARRAY_SIZE];
	float32_t f32_pDst_ref[ARRAY_SIZE];

	q31_t q31_pSrc[ARRAY_SIZE*2] = {1 * 2684356, 1 * 2684356, 2 * 2684356, 2 * 2684356, 3 * 2684356, 3 * 2684356, 4 * 2684356, 4 * 2684356, 5 * 2684356, 5 * 2684356};
	q31_t q31_pDst[ARRAY_SIZE];
	q31_t q31_pDst_ref[ARRAY_SIZE];

	q15_t q15_pSrc[ARRAY_SIZE*2] = {5000, 10000, 15000, 20000, 25000, 5000, 10000, 15000, 20000, 25000};
	q15_t q15_pDst[ARRAY_SIZE];
	q15_t q15_pDst_ref[ARRAY_SIZE];
    /* clang-format on */

    /**********************************************************************************/
    BENCH_START(riscv_cmplx_mag_f32);
    riscv_cmplx_mag_f32(f32_pSrc, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_f32);
    ref_cmplx_mag_f32(f32_pSrc, f32_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (fabs(f32_pDst[i] - f32_pDst_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_cmplx_mag_f32);
            printf("index: %d, expect: %f, actual: %f\n", i, f32_pDst_ref[i],
                   f32_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mag_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_q31);
    riscv_cmplx_mag_q31(q31_pSrc, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_q31);
    ref_cmplx_mag_q31(q31_pSrc, q31_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (labs(q31_pDst[i] - q31_pDst_ref[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_cmplx_mag_q31);
            printf("index: %d, expect: %x, actual: %x\n", i, q31_pDst_ref[i],
                   q31_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mag_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_q15);
    riscv_cmplx_mag_q15(q15_pSrc, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_q15);
    ref_cmplx_mag_q15(q15_pSrc, q15_pDst_ref, ARRAY_SIZE);
    for (i = 0; i < ARRAY_SIZE; i++) {
        if (abs(q15_pDst[i] - q15_pDst_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_cmplx_mag_q15);
            printf("index: %d, expect: %x, actual: %x\n", i, q15_pDst_ref[i],
                   q15_pDst[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_cmplx_mag_q15);
}

int main()
{
    BENCH_INIT;
    DSP_CmplxMag();
    DSP_CmplxDotProduct();
    DSP_CONJ();
    DSP_MagSquared();
    DSP_CmplxMult();
    DSP_CmplxMultReal();
    BENCH_FINISH;
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
