//
// Created by lujun on 19-6-28.
//
// This contains complx calculate in Conjugate, Dot Product, Magnitude,
// Magnitude Squared, by-Complex Multiplication, by-Real Multiplication each one
// has it's own function. All function can be found in main function. If you
// don't want to use it, then comment it. contains following functions,eachone
// includes q15,q31 and f32 versions
#include "riscv_math.h"
#include <stdint.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#include <stdlib.h>

#define ENABLE_ALL

#ifdef ENABLE_ALL
#define CMPLXMAG
#define CMPLXDOTPRODUCT
#define CONJ
#define MAGSQUARED
#define CMPLXMULT
#define CMPLXMULTREAL
#endif

// Array for ComplxMathFunctions part1
#define ARRAY_SIZE 64
float32_t f32_pSrcA[ARRAY_SIZE * 2];
float32_t f32_pSrcB[ARRAY_SIZE * 2];
float32_t f32_pDst[ARRAY_SIZE * 2];
float32_t f32_pDst_ref[ARRAY_SIZE * 2];

q31_t q31_pSrcA[ARRAY_SIZE * 2];
q31_t q31_pSrcB[ARRAY_SIZE * 2];
q31_t q31_pDst[ARRAY_SIZE * 2];
q31_t q31_pDst_ref[ARRAY_SIZE * 2];

q15_t q15_pSrcA[ARRAY_SIZE * 2];
q15_t q15_pSrcB[ARRAY_SIZE * 2];
q15_t q15_pDst[ARRAY_SIZE * 2];
q15_t q15_pDst_ref[ARRAY_SIZE * 2];

int test_flag_error = 0;
int8_t s, s1;
BENCH_DECLARE_VAR();

/*
*********************************************************************************************************
*    DSP_MagSquared
*********************************************************************************************************
*/
static int DSP_MagSquared(void)
{
    /***F32*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_squared_f32);
    riscv_cmplx_mag_squared_f32(f32_pSrcA, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_squared_f32);
    ref_cmplx_mag_squared_f32(f32_pSrcA, f32_pDst_ref, ARRAY_SIZE);
    s = verify_results_f32(f32_pDst_ref, f32_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mag_squared_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mag_squared_f32);

    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_squared_q31);
    riscv_cmplx_mag_squared_q31(q31_pSrcA, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_squared_q31);
    ref_cmplx_mag_squared_q31(q31_pSrcA, q31_pDst_ref, ARRAY_SIZE);
    s = verify_results_q31(q31_pDst_ref, q31_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mag_squared_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mag_squared_q31);

    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_squared_q15);
    riscv_cmplx_mag_squared_q15(q15_pSrcA, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_squared_q15);
    ref_cmplx_mag_squared_q15(q15_pSrcA, q15_pDst_ref, ARRAY_SIZE);
    s = verify_results_q15(q15_pDst_ref, q15_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mag_squared_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mag_squared_q15);
}
/*
*********************************************************************************************************
* DSP_CmplxMult
*********************************************************************************************************
*/
static int DSP_CmplxMult(void)
{
    /***F32*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_cmplx_f32);
    riscv_cmplx_mult_cmplx_f32(f32_pSrcA, f32_pSrcB, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_cmplx_f32);
    ref_cmplx_mult_cmplx_f32(f32_pSrcA, f32_pSrcB, f32_pDst_ref, ARRAY_SIZE);
    s = verify_results_f32(f32_pDst_ref, f32_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mult_cmplx_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mult_cmplx_f32);

    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_cmplx_q31);
    riscv_cmplx_mult_cmplx_q31(q31_pSrcA, q31_pSrcB, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_cmplx_q31);
    ref_cmplx_mult_cmplx_q31(q31_pSrcA, q31_pSrcB, q31_pDst_ref, ARRAY_SIZE);
    s = verify_results_q31(q31_pDst_ref, q31_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mult_cmplx_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mult_cmplx_q31);

    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_cmplx_q15);
    riscv_cmplx_mult_cmplx_q15(q15_pSrcA, q15_pSrcB, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_cmplx_q15);
    ref_cmplx_mult_cmplx_q15(q15_pSrcA, q15_pSrcB, q15_pDst_ref, ARRAY_SIZE);
    s = verify_results_q15(q15_pDst_ref, q15_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mult_cmplx_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mult_cmplx_q15);
}

/*
*********************************************************************************************************
* DSP_CmplxMultReal
*********************************************************************************************************
*/
static int DSP_CmplxMultReal(void)
{
    /***F32*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_real_f32);
    riscv_cmplx_mult_real_f32(f32_pSrcA, f32_pSrcB, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_real_f32);
    ref_cmplx_mult_real_f32(f32_pSrcA, f32_pSrcB, f32_pDst_ref, ARRAY_SIZE);
    s = verify_results_f32(f32_pDst_ref, f32_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mult_real_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mult_real_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_real_q31);
    riscv_cmplx_mult_real_q31(q31_pSrcA, q31_pSrcB, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_real_q31);
    ref_cmplx_mult_real_q31(q31_pSrcA, q31_pSrcB, q31_pDst_ref, ARRAY_SIZE);
    s = verify_results_q31(q31_pDst_ref, q31_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mult_real_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mult_real_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mult_real_q15);
    riscv_cmplx_mult_real_q15(q15_pSrcA, q15_pSrcB, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mult_real_q15);
    ref_cmplx_mult_real_q15(q15_pSrcA, q15_pSrcB, q15_pDst_ref, ARRAY_SIZE);
    s = verify_results_q15(q15_pDst_ref, q15_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mult_real_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mult_real_q15);
}

/*
*********************************************************************************************************
* DSP_CONJ
*********************************************************************************************************
*/
static int DSP_CONJ(void)
{
    /***F32*******************************************************************************/
    BENCH_START(riscv_cmplx_conj_f32);
    riscv_cmplx_conj_f32(f32_pSrcA, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_conj_f32);
    ref_cmplx_conj_f32(f32_pSrcA, f32_pDst_ref, ARRAY_SIZE);
    s = verify_results_f32(f32_pDst_ref, f32_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_conj_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_conj_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_conj_q31);
    riscv_cmplx_conj_q31(q31_pSrcA, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_conj_q31);
    ref_cmplx_conj_q31(q31_pSrcA, q31_pDst_ref, ARRAY_SIZE);
    s = verify_results_q31(q31_pDst_ref, q31_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_conj_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_conj_q31);
    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_conj_q15);
    riscv_cmplx_conj_q15(q15_pSrcA, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_conj_q15);
    ref_cmplx_conj_q15(q15_pSrcA, q15_pDst_ref, ARRAY_SIZE);
    s = verify_results_q15(q15_pDst_ref, q15_pDst, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_conj_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_conj_q15);
}

/*
*********************************************************************************************************
* DSP_CmplxDotProduct
*********************************************************************************************************
*/
static int DSP_CmplxDotProduct(void)
{
    float32_t f32_realResult;
    float32_t f32_imagResult;
    float32_t f32_realResult_ref;
    float32_t f32_imagResult_ref;
    q63_t q63_realResult;
    q63_t q63_imagResult;
    q63_t q63_realResult_ref;
    q63_t q63_imagResult_ref;
    q31_t q31_realResult;
    q31_t q31_imagResult;
    q31_t q31_realResult_ref;
    q31_t q31_imagResult_ref;

    /***F32*******************************************************************************/
    BENCH_START(riscv_cmplx_dot_prod_f32);
    riscv_cmplx_dot_prod_f32(f32_pSrcA, f32_pSrcB, ARRAY_SIZE, &f32_realResult,
                           &f32_imagResult);
    BENCH_END(riscv_cmplx_dot_prod_f32);
    ref_cmplx_dot_prod_f32(f32_pSrcA, f32_pSrcB, ARRAY_SIZE,
                           &f32_realResult_ref, &f32_imagResult_ref);

    s = verify_results_f32(&f32_realResult, &f32_realResult_ref, 1);
    s1 = verify_results_f32(&f32_imagResult, &f32_imagResult_ref, 1);
    if (s != 0 || s1 != 0) {
        BENCH_ERROR(riscv_cmplx_dot_prod_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_dot_prod_f32);
    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_dot_prod_q31);
    riscv_cmplx_dot_prod_q31(q31_pSrcA, q31_pSrcB, ARRAY_SIZE, &q63_realResult,
                           &q63_imagResult);
    BENCH_END(riscv_cmplx_dot_prod_q31);
    ref_cmplx_dot_prod_q31(q31_pSrcA, q31_pSrcB, ARRAY_SIZE,
                           &q63_realResult_ref, &q63_imagResult_ref);
    s = verify_results_q63(&q63_realResult, &q63_realResult_ref, 1);
    s1 = verify_results_q63(&q63_imagResult, &q63_imagResult_ref, 1);
    if (s != 0 || s1 != 0) {
        BENCH_ERROR(riscv_cmplx_dot_prod_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_dot_prod_q31);

    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_dot_prod_q15);
    riscv_cmplx_dot_prod_q15(q15_pSrcA, q15_pSrcB, ARRAY_SIZE, &q31_realResult,
                           &q31_imagResult);
    BENCH_END(riscv_cmplx_dot_prod_q15);
    ref_cmplx_dot_prod_q15(q15_pSrcA, q15_pSrcB, ARRAY_SIZE,
                           &q31_realResult_ref, &q31_imagResult_ref);
    s = verify_results_q31(&q31_realResult, &q31_realResult_ref, 1);
    s1 = verify_results_q31(&q31_imagResult, &q31_imagResult_ref, 1);
    if (s != 0 || s1 != 0) {
        BENCH_ERROR(riscv_cmplx_dot_prod_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_dot_prod_q15);
}

/*
*********************************************************************************************************
* DSP_CmplxMag
*********************************************************************************************************
*/
static int DSP_CmplxMag(void)
{
    /***F32*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_f32);
    riscv_cmplx_mag_f32(f32_pSrcA, f32_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_f32);
    ref_cmplx_mag_f32(f32_pSrcA, f32_pDst_ref, ARRAY_SIZE);
    s = verify_results_f32(f32_pDst, f32_pDst_ref, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mag_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mag_f32);

    /***Q31*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_q31);
    riscv_cmplx_mag_q31(q31_pSrcA, q31_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_q31);
    ref_cmplx_mag_q31(q31_pSrcA, q31_pDst_ref, ARRAY_SIZE);
    s = verify_results_q31_low_precision(q31_pDst, q31_pDst_ref, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mag_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mag_q31);

    /***Q15*******************************************************************************/
    BENCH_START(riscv_cmplx_mag_q15);
    riscv_cmplx_mag_q15(q15_pSrcA, q15_pDst, ARRAY_SIZE);
    BENCH_END(riscv_cmplx_mag_q15);
    ref_cmplx_mag_q15(q15_pSrcA, q15_pDst_ref, ARRAY_SIZE);
    s = verify_results_q15_low_precision(q15_pDst, q15_pDst_ref, ARRAY_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_cmplx_mag_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cmplx_mag_q15);
}

int main(void)
{
    BENCH_INIT();
    generate_rand_q15(q15_pSrcA, ARRAY_SIZE * 2);
    generate_rand_q15(q15_pSrcB, ARRAY_SIZE * 2);
    generate_rand_q31(q31_pSrcA, ARRAY_SIZE * 2);
    generate_rand_q31(q31_pSrcB, ARRAY_SIZE * 2);
    generate_rand_f32(f32_pSrcA, ARRAY_SIZE * 2);
    generate_rand_f32(f32_pSrcB, ARRAY_SIZE * 2);
#ifdef CMPLXMAG
    DSP_CmplxMag();
#endif
#ifdef CMPLXDOTPRODUCT
    DSP_CmplxDotProduct();
#endif
#ifdef CONJ
    DSP_CONJ();
#endif
#ifdef MAGSQUARED
    DSP_MagSquared();
#endif
#ifdef CMPLXMULT
    DSP_CmplxMult();
#endif
#ifdef CMPLXMULTREAL
    DSP_CmplxMultReal();
#endif

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
