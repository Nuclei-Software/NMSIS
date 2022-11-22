
#ifndef _REF_H
#define _REF_H

#include "riscv_math.h"
#include <math.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PI
#define PI 3.14159265358979f
#endif

/**
 * @brief 8-bit fractional data type in 1.7 format.
 */
//  typedef int8_t q7_t;

/**
 * @brief 16-bit fractional data type in 1.15 format.
 */
//  typedef int16_t q15_t;

/**
 * @brief 32-bit fractional data type in 1.31 format.
 */
//  typedef int32_t q31_t;

/**
 * @brief 64-bit fractional data type in 1.63 format.
 */
//  typedef int64_t q63_t;

/**
 * @brief 32-bit floating-point type definition.
 */
//  typedef float float32_t;

/**
 * @brief 64-bit floating-point type definition.
 */
//  typedef double float64_t;

/**
 * @brief Error status returned by some functions in the library.
 */

typedef enum {
    REF_Q7 = 0,
    REF_Q15,
    REF_Q31,
    REF_F32,
} dataType;

#ifndef FLT_MAX
#define FLT_MAX 3.40282347e+38F
#endif

#ifndef DBL_MAX
#define DBL_MAX 1.79769313486231571e+308
#endif

#ifndef FLT_MIN
#define FLT_MIN 1.175494351e-38F
#endif

#ifndef DBL_MIN
#define DBL_MIN 2.22507385850720138e-308
#endif

#ifndef SCHAR_MIN
/* mimimum value for an object of type signed char */
#define SCHAR_MIN (-128)
#endif

#ifndef SCHAR_MAX
/* maximum value for an object of type signed char */
#define SCHAR_MAX 127
#endif

#ifndef UCHAR_MAX
/* maximum value for an object of type unsigned char */
#define UCHAR_MAX 255
#endif

#ifndef SHRT_MIN
/* minimum value for an object of type short int */
#define SHRT_MIN (-0x8000)
#endif

#ifndef SHRT_MAX
/* maximum value for an object of type short int */
#define SHRT_MAX 0x7fff
#endif

#ifndef USHRT_MAX
/* maximum value for an object of type unsigned short int */
#define USHRT_MAX 65535
#endif

#ifndef INT_MIN
/* minimum value for an object of type int */
#define INT_MIN (~0x7fffffff) /* -2147483648 and 0x80000000 are unsigned */
#endif

#ifndef INT_MAX
/* maximum value for an object of type int */
#define INT_MAX 0x7fffffff
#endif

#ifndef UINT_MAX
/* maximum value for an object of type unsigned int */
#define UINT_MAX 0xffffffffU
#endif

#ifndef LONG_MIN
/* minimum value for an object of type long int */
#define LONG_MIN (~0x7fffffffL)
#endif

#ifndef LONG_MAX
/* maximum value for an object of type long int */
#define LONG_MAX 0x7fffffffL
#endif

#ifndef ULONG_MAX
/* maximum value for an object of type unsigned long int */
#define ULONG_MAX 0xffffffffUL
#endif

/*
 * Ref Lib Global Variables
 */

/*
 * Ref Lib Functions
 */

/*
 * Helper Functions
 */
q31_t ref_sat_n(q31_t num, uint32_t bits);

q31_t ref_sat_q31(q63_t num);

q15_t ref_sat_q15(q31_t num);

q7_t ref_sat_q7(q15_t num);

float32_t ref_pow(float32_t a, uint32_t b);

extern float32_t tempMatrixArray[];

float32_t ref_detrm(float32_t *pSrc, float32_t *temp, uint32_t size);

void ref_cofact(float32_t *pSrc, float32_t *pDst, float32_t *temp,
                uint32_t size);

float64_t ref_detrm64(float64_t *pSrc, float64_t *temp, uint32_t size);

void ref_cofact64(float64_t *pSrc, float64_t *pDst, float64_t *temp,
                  uint32_t size);

/*
 * Basic Math Functions
 */
void ref_abs_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_abs_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_abs_q15(q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_abs_q7(q7_t *pSrc, q7_t *pDst, uint32_t blockSize);

void ref_add_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                 uint32_t blockSize);

void ref_add_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize);

void ref_add_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize);

void ref_add_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize);

void ref_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB, uint32_t blockSize,
                      float32_t *result);

void ref_dot_prod_q31(q31_t *pSrcA, q31_t *pSrcB, uint32_t blockSize,
                      q63_t *result);

void ref_dot_prod_q15(q15_t *pSrcA, q15_t *pSrcB, uint32_t blockSize,
                      q63_t *result);

void ref_dot_prod_q7(q7_t *pSrcA, q7_t *pSrcB, uint32_t blockSize,
                     q31_t *result);

void ref_mult_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                  uint32_t blockSize);

void ref_mult_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize);

void ref_mult_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize);

void ref_mult_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize);

void ref_negate_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_negate_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_negate_q15(q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_negate_q7(q7_t *pSrc, q7_t *pDst, uint32_t blockSize);

void ref_offset_f32(float32_t *pSrc, float32_t offset, float32_t *pDst,
                    uint32_t blockSize);

void ref_offset_q31(q31_t *pSrc, q31_t offset, q31_t *pDst, uint32_t blockSize);

void ref_offset_q15(q15_t *pSrc, q15_t offset, q15_t *pDst, uint32_t blockSize);

void ref_offset_q7(q7_t *pSrc, q7_t offset, q7_t *pDst, uint32_t blockSize);

void ref_scale_f32(float32_t *pSrc, float32_t scale, float32_t *pDst,
                   uint32_t blockSize);

void ref_scale_q31(q31_t *pSrc, q31_t scaleFract, int8_t shift, q31_t *pDst,
                   uint32_t blockSize);

void ref_scale_q15(q15_t *pSrc, q15_t scaleFract, int8_t shift, q15_t *pDst,
                   uint32_t blockSize);

void ref_scale_q7(q7_t *pSrc, q7_t scaleFract, int8_t shift, q7_t *pDst,
                  uint32_t blockSize);

void ref_shift_q31(q31_t *pSrc, int8_t shiftBits, q31_t *pDst,
                   uint32_t blockSize);

void ref_shift_q15(q15_t *pSrc, int8_t shiftBits, q15_t *pDst,
                   uint32_t blockSize);

void ref_shift_q7(q7_t *pSrc, int8_t shiftBits, q7_t *pDst, uint32_t blockSize);

void ref_sub_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst,
                 uint32_t blockSize);

void ref_sub_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst, uint32_t blockSize);

void ref_sub_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst, uint32_t blockSize);

void ref_sub_q7(q7_t *pSrcA, q7_t *pSrcB, q7_t *pDst, uint32_t blockSize);

void ref_clip_f32(const float32_t * pSrc, float32_t * pDst, float32_t low, float32_t high, uint32_t numSamples);

void ref_clip_q7(const q7_t * pSrc, q7_t * pDst, q7_t low, q7_t high, uint32_t numSamples);

void ref_clip_q15(const q15_t * pSrc, q15_t * pDst, q15_t low, q15_t high, uint32_t numSamples);

void ref_clip_q31(const q31_t * pSrc, q31_t * pDst, q31_t low, q31_t high, uint32_t numSamples);

void ref_and_u16(const uint16_t * pSrcA, const uint16_t * pSrcB, uint16_t * pDst, uint32_t blockSize);

void ref_and_u32(const uint32_t * pSrcA, const uint32_t * pSrcB, uint32_t * pDst, uint32_t blockSize);

void ref_and_u8(const uint8_t * pSrcA, const uint8_t * pSrcB, uint8_t * pDst, uint32_t blockSize);

void ref_or_u16(const uint16_t * pSrcA, const uint16_t * pSrcB, uint16_t * pDst, uint32_t blockSize);

void ref_or_u32(const uint32_t * pSrcA, const uint32_t * pSrcB, uint32_t * pDst, uint32_t blockSize);

void ref_or_u8(const uint8_t * pSrcA, const uint8_t * pSrcB, uint8_t * pDst, uint32_t blockSize);

void ref_not_u16(const uint16_t * pSrc, uint16_t * pDst, uint32_t blockSize);

void ref_not_u32(const uint32_t * pSrc, uint32_t * pDst, uint32_t blockSize);

void ref_not_u8(const uint8_t * pSrc, uint8_t * pDst, uint32_t blockSize);

void ref_xor_u16(const uint16_t * pSrcA, const uint16_t * pSrcB, uint16_t * pDst, uint32_t blockSize);

void ref_xor_u32(const uint32_t * pSrcA, const uint32_t * pSrcB, uint32_t * pDst, uint32_t blockSize);

void ref_xor_u8(const uint8_t * pSrcA, const uint8_t * pSrcB, uint8_t * pDst, uint32_t blockSize);

/*
 * Complex Math Functions
 */
void ref_cmplx_conj_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples);

void ref_cmplx_conj_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples);

void ref_cmplx_conj_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples);

void ref_cmplx_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB,
                            uint32_t numSamples, float32_t *realResult,
                            float32_t *imagResult);

void ref_cmplx_dot_prod_q31(q31_t *pSrcA, q31_t *pSrcB, uint32_t numSamples,
                            q63_t *realResult, q63_t *imagResult);

void ref_cmplx_dot_prod_q15(q15_t *pSrcA, q15_t *pSrcB, uint32_t numSamples,
                            q31_t *realResult, q31_t *imagResult);

void ref_cmplx_mag_f32(float32_t *pSrc, float32_t *pDst, uint32_t numSamples);

void ref_cmplx_mag_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples);

void ref_cmplx_mag_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples);

void ref_cmplx_mag_squared_f32(float32_t *pSrc, float32_t *pDst,
                               uint32_t numSamples);

void ref_cmplx_mag_squared_q31(q31_t *pSrc, q31_t *pDst, uint32_t numSamples);

void ref_cmplx_mag_squared_q15(q15_t *pSrc, q15_t *pDst, uint32_t numSamples);

void ref_cmplx_mult_cmplx_f32(float32_t *pSrcA, float32_t *pSrcB,
                              float32_t *pDst, uint32_t numSamples);

void ref_cmplx_mult_cmplx_q31(q31_t *pSrcA, q31_t *pSrcB, q31_t *pDst,
                              uint32_t numSamples);

void ref_cmplx_mult_cmplx_q15(q15_t *pSrcA, q15_t *pSrcB, q15_t *pDst,
                              uint32_t numSamples);

void ref_cmplx_mult_real_f32(float32_t *pSrcCmplx, float32_t *pSrcReal,
                             float32_t *pCmplxDst, uint32_t numSamples);

void ref_cmplx_mult_real_q31(q31_t *pSrcCmplx, q31_t *pSrcReal,
                             q31_t *pCmplxDst, uint32_t numSamples);

void ref_cmplx_mult_real_q15(q15_t *pSrcCmplx, q15_t *pSrcReal,
                             q15_t *pCmplxDst, uint32_t numSamples);

/*
 * Controller Functions
 */
void ref_sin_cos_f32(float32_t theta, float32_t *pSinVal, float32_t *pCosVal);

void ref_sin_cos_q31(q31_t theta, q31_t *pSinVal, q31_t *pCosVal);

float32_t ref_pid_f32(riscv_pid_instance_f32 *S, float32_t in);

q31_t ref_pid_q31(riscv_pid_instance_q31 *S, q31_t in);

q15_t ref_pid_q15(riscv_pid_instance_q15 *S, q15_t in);

/*
 * Fast Math Functions
 */
#define ref_sin_f32(a) sinf(a)

q31_t ref_sin_q31(q31_t x);

q15_t ref_sin_q15(q15_t x);

#define ref_cos_f32(a) cosf(a)

q31_t ref_cos_q31(q31_t x);

q15_t ref_cos_q15(q15_t x);

riscv_status ref_sqrt_q31(q31_t in, q31_t *pOut);

riscv_status ref_sqrt_q15(q15_t in, q15_t *pOut);

riscv_status ref_divide_q15(q15_t numerator, q15_t denominator, q15_t *quotient, int16_t *shift);

void ref_vexp_f32(const float32_t * pSrc, float32_t * pDst, uint32_t blockSize);
void ref_vlog_f32(const float32_t * pSrc, float32_t * pDst, uint32_t blockSize);

/*
 * distance Functions
 */
float32_t ref_braycurtis_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize);
float32_t ref_canberra_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize);
float32_t ref_chebyshev_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize);
float32_t ref_cityblock_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize);
float32_t ref_cityblock_distance_f32(const float32_t *pA, const float32_t *pB, uint32_t blockSize);
float32_t ref_cosine_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize);
float32_t ref_euclidean_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize);
float32_t ref_jensenshannon_distance_f32(const float32_t *pA,const float32_t *pB, uint32_t blockSize);
float32_t ref_minkowski_distance_f32(const float32_t *pA, const float32_t *pB, int32_t order, uint32_t blockSize);
/*
 * Filtering Functions
 */
void ref_biquad_cascade_df2T_f32(const riscv_biquad_cascade_df2T_instance_f32 *S,
                                 float32_t *pSrc, float32_t *pDst,
                                 uint32_t blockSize);

void ref_biquad_cascade_stereo_df2T_f32(
    const riscv_biquad_cascade_stereo_df2T_instance_f32 *S, float32_t *pSrc,
    float32_t *pDst, uint32_t blockSize);

void ref_biquad_cascade_df2T_f64(const riscv_biquad_cascade_df2T_instance_f64 *S,
                                 float64_t *pSrc, float64_t *pDst,
                                 uint32_t blockSize);

void ref_biquad_cascade_df1_f32(const riscv_biquad_casd_df1_inst_f32 *S,
                                float32_t *pSrc, float32_t *pDst,
                                uint32_t blockSize);

void ref_biquad_cas_df1_32x64_q31(const riscv_biquad_cas_df1_32x64_ins_q31 *S,
                                  q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_biquad_cascade_df1_q31(const riscv_biquad_casd_df1_inst_q31 *S,
                                q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_biquad_cascade_df1_fast_q31(const riscv_biquad_casd_df1_inst_q31 *S,
                                     q31_t *pSrc, q31_t *pDst,
                                     uint32_t blockSize);

void ref_biquad_cascade_df1_fast_q15(const riscv_biquad_casd_df1_inst_q15 *S,
                                     q15_t *pSrc, q15_t *pDst,
                                     uint32_t blockSize);

void ref_biquad_cascade_df1_q15(const riscv_biquad_casd_df1_inst_q15 *S,
                                q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_conv_f32(float32_t *pSrcA, uint32_t srcALen, float32_t *pSrcB,
                  uint32_t srcBLen, float32_t *pDst);

riscv_status ref_conv_partial_f32(float32_t *pSrcA, uint32_t srcALen,
                                float32_t *pSrcB, uint32_t srcBLen,
                                float32_t *pDst, uint32_t firstIndex,
                                uint32_t numPoints);

void ref_conv_q31(q31_t *pSrcA, uint32_t srcALen, q31_t *pSrcB,
                  uint32_t srcBLen, q31_t *pDst);

void ref_conv_fast_q31(q31_t *pSrcA, uint32_t srcALen, q31_t *pSrcB,
                       uint32_t srcBLen, q31_t *pDst);

riscv_status ref_conv_partial_q31(q31_t *pSrcA, uint32_t srcALen, q31_t *pSrcB,
                                uint32_t srcBLen, q31_t *pDst,
                                uint32_t firstIndex, uint32_t numPoints);

riscv_status ref_conv_partial_fast_q31(q31_t *pSrcA, uint32_t srcALen,
                                     q31_t *pSrcB, uint32_t srcBLen,
                                     q31_t *pDst, uint32_t firstIndex,
                                     uint32_t numPoints);

void ref_conv_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                  uint32_t srcBLen, q15_t *pDst);

#define ref_conv_opt_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst, pScratch1,      \
                         pScratch2)                                            \
    ref_conv_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst)

void ref_conv_fast_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                       uint32_t srcBLen, q15_t *pDst);

void ref_conv_fast_opt_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                           uint32_t srcBLen, q15_t *pDst, q15_t *pScratch1,
                           q15_t *pScratch2);

riscv_status ref_conv_partial_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                                uint32_t srcBLen, q15_t *pDst,
                                uint32_t firstIndex, uint32_t numPoints);

#define ref_conv_partial_opt_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst,         \
                                 firstIndex, numPoints, pScratch1, pScratch2)  \
    ref_conv_partial_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst, firstIndex,     \
                         numPoints)

riscv_status ref_conv_partial_fast_q15(q15_t *pSrcA, uint32_t srcALen,
                                     q15_t *pSrcB, uint32_t srcBLen,
                                     q15_t *pDst, uint32_t firstIndex,
                                     uint32_t numPoints);

riscv_status ref_conv_partial_fast_opt_q15(q15_t *pSrcA, uint32_t srcALen,
                                         q15_t *pSrcB, uint32_t srcBLen,
                                         q15_t *pDst, uint32_t firstIndex,
                                         uint32_t numPoints, q15_t *pScratch1,
                                         q15_t *pScratch2);

void ref_conv_q7(q7_t *pSrcA, uint32_t srcALen, q7_t *pSrcB, uint32_t srcBLen,
                 q7_t *pDst);

#define ref_conv_opt_q7(pSrcA, srcALen, pSrcB, srcBLen, pDst, pScratch1,       \
                        pScratch2)                                             \
    ref_conv_q7(pSrcA, srcALen, pSrcB, srcBLen, pDst)

riscv_status ref_conv_partial_q7(q7_t *pSrcA, uint32_t srcALen, q7_t *pSrcB,
                               uint32_t srcBLen, q7_t *pDst,
                               uint32_t firstIndex, uint32_t numPoints);

#define ref_conv_partial_opt_q7(pSrcA, srcALen, pSrcB, srcBLen, pDst,          \
                                firstIndex, numPoints, pScratch1, pScratch2)   \
    ref_conv_partial_q7(pSrcA, srcALen, pSrcB, srcBLen, pDst, firstIndex,      \
                        numPoints)

void ref_correlate_f32(float32_t *pSrcA, uint32_t srcALen, float32_t *pSrcB,
                       uint32_t srcBLen, float32_t *pDst);

void ref_correlate_q31(q31_t *pSrcA, uint32_t srcALen, q31_t *pSrcB,
                       uint32_t srcBLen, q31_t *pDst);

void ref_correlate_fast_q31(q31_t *pSrcA, uint32_t srcALen, q31_t *pSrcB,
                            uint32_t srcBLen, q31_t *pDst);

void ref_correlate_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                       uint32_t srcBLen, q15_t *pDst);

#define ref_correlate_opt_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst, pScratch1) \
    ref_correlate_q15(pSrcA, srcALen, pSrcB, srcBLen, pDst)

void ref_correlate_fast_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                            uint32_t srcBLen, q15_t *pDst);

void ref_correlate_fast_opt_q15(q15_t *pSrcA, uint32_t srcALen, q15_t *pSrcB,
                                uint32_t srcBLen, q15_t *pDst, q15_t *pScratch);

void ref_correlate_q7(q7_t *pSrcA, uint32_t srcALen, q7_t *pSrcB,
                      uint32_t srcBLen, q7_t *pDst);

#define ref_correlate_opt_q7(pSrcA, srcALen, pSrcB, srcBLen, pDst, pScratch1,  \
                             pScratch2)                                        \
    ref_correlate_q7(pSrcA, srcALen, pSrcB, srcBLen, pDst)

void ref_fir_f32(const riscv_fir_instance_f32 *S, float32_t *pSrc,
                 float32_t *pDst, uint32_t blockSize);

void ref_fir_q31(const riscv_fir_instance_q31 *S, q31_t *pSrc, q31_t *pDst,
                 uint32_t blockSize);

void ref_fir_fast_q31(const riscv_fir_instance_q31 *S, q31_t *pSrc, q31_t *pDst,
                      uint32_t blockSize);

void ref_fir_q15(const riscv_fir_instance_q15 *S, q15_t *pSrc, q15_t *pDst,
                 uint32_t blockSize);

void ref_fir_fast_q15(const riscv_fir_instance_q15 *S, q15_t *pSrc, q15_t *pDst,
                      uint32_t blockSize);

void ref_fir_q7(const riscv_fir_instance_q7 *S, q7_t *pSrc, q7_t *pDst,
                uint32_t blockSize);

void ref_fir_decimate_f32(const riscv_fir_decimate_instance_f32 *S,
                          float32_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_fir_decimate_q31(const riscv_fir_decimate_instance_q31 *S, q31_t *pSrc,
                          q31_t *pDst, uint32_t blockSize);

void ref_fir_decimate_fast_q31(const riscv_fir_decimate_instance_q31 *S,
                               q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_fir_decimate_q15(const riscv_fir_decimate_instance_q15 *S, q15_t *pSrc,
                          q15_t *pDst, uint32_t blockSize);

void ref_fir_decimate_fast_q15(const riscv_fir_decimate_instance_q15 *S,
                               q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_fir_lattice_f32(const riscv_fir_lattice_instance_f32 *S, float32_t *pSrc,
                         float32_t *pDst, uint32_t blockSize);

void ref_fir_lattice_q31(const riscv_fir_lattice_instance_q31 *S, q31_t *pSrc,
                         q31_t *pDst, uint32_t blockSize);

void ref_fir_lattice_q15(const riscv_fir_lattice_instance_q15 *S, q15_t *pSrc,
                         q15_t *pDst, uint32_t blockSize);

void ref_fir_sparse_f32(riscv_fir_sparse_instance_f32 *S, float32_t *pSrc,
                        float32_t *pDst, float32_t *pScratchIn,
                        uint32_t blockSize);

void ref_fir_sparse_q31(riscv_fir_sparse_instance_q31 *S, q31_t *pSrc,
                        q31_t *pDst, q31_t *pScratchIn, uint32_t blockSize);

void ref_fir_sparse_q15(riscv_fir_sparse_instance_q15 *S, q15_t *pSrc,
                        q15_t *pDst, q15_t *pScratchIn, q31_t *pScratchOut,
                        uint32_t blockSize);

void ref_fir_sparse_q7(riscv_fir_sparse_instance_q7 *S, q7_t *pSrc, q7_t *pDst,
                       q7_t *pScratchIn, q31_t *pScratchOut,
                       uint32_t blockSize);

void ref_iir_lattice_f32(const riscv_iir_lattice_instance_f32 *S, float32_t *pSrc,
                         float32_t *pDst, uint32_t blockSize);

void ref_iir_lattice_q31(const riscv_iir_lattice_instance_q31 *S, q31_t *pSrc,
                         q31_t *pDst, uint32_t blockSize);

void ref_iir_lattice_q15(const riscv_iir_lattice_instance_q15 *S, q15_t *pSrc,
                         q15_t *pDst, uint32_t blockSize);

void ref_lms_f32(const riscv_lms_instance_f32 *S, float32_t *pSrc,
                 float32_t *pRef, float32_t *pOut, float32_t *pErr,
                 uint32_t blockSize);

void ref_lms_norm_f32(riscv_lms_norm_instance_f32 *S, float32_t *pSrc,
                      float32_t *pRef, float32_t *pOut, float32_t *pErr,
                      uint32_t blockSize);

void ref_lms_q31(const riscv_lms_instance_q31 *S, q31_t *pSrc, q31_t *pRef,
                 q31_t *pOut, q31_t *pErr, uint32_t blockSize);

void ref_lms_norm_q31(riscv_lms_norm_instance_q31 *S, q31_t *pSrc, q31_t *pRef,
                      q31_t *pOut, q31_t *pErr, uint32_t blockSize);

void ref_lms_q15(const riscv_lms_instance_q15 *S, q15_t *pSrc, q15_t *pRef,
                 q15_t *pOut, q15_t *pErr, uint32_t blockSize);

void ref_lms_norm_q15(riscv_lms_norm_instance_q15 *S, q15_t *pSrc, q15_t *pRef,
                      q15_t *pOut, q15_t *pErr, uint32_t blockSize);

void ref_fir_interpolate_f32(const riscv_fir_interpolate_instance_f32 *S,
                             float32_t *pSrc, float32_t *pDst,
                             uint32_t blockSize);

void ref_fir_interpolate_q31(const riscv_fir_interpolate_instance_q31 *S,
                             q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_fir_interpolate_q15(const riscv_fir_interpolate_instance_q15 *S,
                             q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_levinson_durbin_f32(const float32_t *phi, float32_t *a, float32_t *err, int nbCoefs);

void ref_levinson_durbin_q31(const q31_t *phi, q31_t *a, q31_t *err, int nbCoefs);
/*
 * Matrix Functions
 */
riscv_status ref_mat_cmplx_mult_f32(const riscv_matrix_instance_f32 *pSrcA,
                                  const riscv_matrix_instance_f32 *pSrcB,
                                  riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_cmplx_mult_q31(const riscv_matrix_instance_q31 *pSrcA,
                                  const riscv_matrix_instance_q31 *pSrcB,
                                  riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_cmplx_mult_q15(const riscv_matrix_instance_q15 *pSrcA,
                                  const riscv_matrix_instance_q15 *pSrcB,
                                  riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_inverse_f32(const riscv_matrix_instance_f32 *pSrc,
                               riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_inverse_f64(const riscv_matrix_instance_f64 *pSrc,
                               riscv_matrix_instance_f64 *pDst);

riscv_status ref_mat_mult_f32(const riscv_matrix_instance_f32 *pSrcA,
                            const riscv_matrix_instance_f32 *pSrcB,
                            riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_mult_q31(const riscv_matrix_instance_q31 *pSrcA,
                            const riscv_matrix_instance_q31 *pSrcB,
                            riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_mult_fast_q31(const riscv_matrix_instance_q31 *pSrcA,
                            const riscv_matrix_instance_q31 *pSrcB,
                            riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_mult_opt_q31(const riscv_matrix_instance_q31 *pSrcA,
                            const riscv_matrix_instance_q31 *pSrcB,
                            riscv_matrix_instance_q31 *pDst);

void ref_mat_vec_mult_f32(const riscv_matrix_instance_f32 *pSrcMat,
                        const float32_t *pVec,
                        float32_t *pDst);

void ref_mat_vec_mult_q31(const riscv_matrix_instance_q31 *pSrcMat,
                        const q31_t *pVec,
                        q31_t *pDst);

void ref_mat_vec_mult_q15(const riscv_matrix_instance_q15 *pSrcMat,
                        const q15_t *pVec,
                        q15_t *pDst);

void ref_mat_vec_mult_q7(const riscv_matrix_instance_q7 *pSrcMat,
                       const q7_t *pVec,
                       q7_t *pDst);


riscv_status ref_mat_mult_q15(const riscv_matrix_instance_q15 *pSrcA,
                            const riscv_matrix_instance_q15 *pSrcB,
                            riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_mult_fast_q15(const riscv_matrix_instance_q15 *pSrcA,
                            const riscv_matrix_instance_q15 *pSrcB,
                            riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_mult_q7(const riscv_matrix_instance_q7 *pSrcA,
                            const riscv_matrix_instance_q7 *pSrcB,
                            riscv_matrix_instance_q7 *pDst);

riscv_status ref_mat_scale_f32(const riscv_matrix_instance_f32 *pSrc,
                             float32_t scale, riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_scale_q31(const riscv_matrix_instance_q31 *pSrc, q31_t scale,
                             int32_t shift, riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_scale_q15(const riscv_matrix_instance_q15 *pSrc, q15_t scale,
                             int32_t shift, riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_sub_f32(const riscv_matrix_instance_f32 *pSrcA,
                           const riscv_matrix_instance_f32 *pSrcB,
                           riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_sub_q31(const riscv_matrix_instance_q31 *pSrcA,
                           const riscv_matrix_instance_q31 *pSrcB,
                           riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_sub_q15(const riscv_matrix_instance_q15 *pSrcA,
                           const riscv_matrix_instance_q15 *pSrcB,
                           riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_trans_f64(const riscv_matrix_instance_f64 *pSrc,
                             riscv_matrix_instance_f64 *pDst);

riscv_status ref_mat_trans_f32(const riscv_matrix_instance_f32 *pSrc,
                             riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_trans_q31(const riscv_matrix_instance_q31 *pSrc,
                             riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_trans_q15(const riscv_matrix_instance_q15 *pSrc,
                             riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_add_f32(const riscv_matrix_instance_f32 *pSrcA,
                           const riscv_matrix_instance_f32 *pSrcB,
                           riscv_matrix_instance_f32 *pDst);

riscv_status ref_mat_add_q31(const riscv_matrix_instance_q31 *pSrcA,
                           const riscv_matrix_instance_q31 *pSrcB,
                           riscv_matrix_instance_q31 *pDst);

riscv_status ref_mat_add_q15(const riscv_matrix_instance_q15 *pSrcA,
                           const riscv_matrix_instance_q15 *pSrcB,
                           riscv_matrix_instance_q15 *pDst);

riscv_status ref_mat_solve_upper_triangular_f32(const riscv_matrix_instance_f32 * ut,
                                                const riscv_matrix_instance_f32 * a,
                                                riscv_matrix_instance_f32 * dst);

riscv_status ref_mat_solve_lower_triangular_f32(const riscv_matrix_instance_f32 * lt,
                                                const riscv_matrix_instance_f32 * a,
                                                riscv_matrix_instance_f32 * dst);

riscv_status ref_mat_cholesky_f32(const riscv_matrix_instance_f32 * pSrc,
                                  riscv_matrix_instance_f32 * pDst);

riscv_status ref_mat_ldlt_f32(const riscv_matrix_instance_f32 * pSrc,
                              riscv_matrix_instance_f32 * pl,
                              riscv_matrix_instance_f32 * pd,
                              uint16_t * pp);

riscv_status ref_mat_cholesky_f64(const riscv_matrix_instance_f64 * pSrc,
                                  riscv_matrix_instance_f64 * pDst);

riscv_status ref_mat_solve_upper_triangular_f64(const riscv_matrix_instance_f64 * ut,
                                                const riscv_matrix_instance_f64 * a,
                                                riscv_matrix_instance_f64 * dst);

riscv_status ref_mat_solve_lower_triangular_f64(const riscv_matrix_instance_f64 * lt,
                                                const riscv_matrix_instance_f64 * a,
                                                riscv_matrix_instance_f64 * dst);
riscv_status ref_mat_ldlt_f64(const riscv_matrix_instance_f64 * pSrc,
                              riscv_matrix_instance_f64 * pl,
                              riscv_matrix_instance_f64 * pd,
                              uint16_t * pp);
/*
 * Quaternion Math Functions
 */
void ref_quaternion_conjugate_f32(const float32_t *pInputQuaternions,
    float32_t *pConjugateQuaternions,
    uint32_t nbQuaternions);

void ref_quaternion_inverse_f32(const float32_t *pInputQuaternions,
  float32_t *pInverseQuaternions,
  uint32_t nbQuaternions);

void ref_quaternion_norm_f32(const float32_t *pInputQuaternions,
  float32_t *pNorms,
  uint32_t nbQuaternions);

void ref_quaternion_normalize_f32(const float32_t *pInputQuaternions,
    float32_t *pNormalizedQuaternions,
    uint32_t nbQuaternions);

void ref_quaternion_product_f32(const float32_t *qa,
    const float32_t *qb,
    float32_t *qr,
    uint32_t nbQuaternions);

void ref_quaternion_product_single_f32(const float32_t *qa,
    const float32_t *qb,
    float32_t *qr);

void ref_quaternion2rotation_f32(const float32_t *pInputQuaternions,
    float32_t *pOutputRotations,
    uint32_t nbQuaternions);

void ref_rotation2quaternion_f32(const float32_t *pInputRotations,
    float32_t *pOutputQuaternions,
    uint32_t nbQuaternions);
/*
 * Statistics Functions
 */
void ref_absmax_f32(const float32_t *pSrc, uint32_t blockSize,
                    float32_t *pResult, uint32_t * pIndex);

void ref_absmax_no_idx_f32(const float32_t *pSrc, uint32_t blockSize,
                           float32_t *pResult);

void ref_absmin_f32(const float32_t *pSrc, uint32_t blockSize,
                    float32_t *pResult, uint32_t *pIndex);

void ref_absmin_no_idx_f32(const float32_t *pSrc, uint32_t blockSize,
                           float32_t *pResult);

void ref_absmax_q31(const q31_t *pSrc, uint32_t blockSize, q31_t *pResult,
                    uint32_t *pIndex);

void ref_absmax_no_idx_q31(const q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_absmin_q31(const q31_t *pSrc, uint32_t blockSize, q31_t *pResult,
                    uint32_t *pIndex);

void ref_absmin_no_idx_q31(const q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_absmax_q15(const q15_t *pSrc, uint32_t blockSize, q15_t *pResult,
                    uint32_t *pIndex);

void ref_absmax_no_idx_q15(const q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_absmin_q15(const q15_t *pSrc, uint32_t blockSize, q15_t *pResult,
                    uint32_t *pIndex);

void ref_absmin_no_idx_q15(const q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_absmax_q7(const q7_t *pSrc, uint32_t blockSize, q7_t *pResult,
                   uint32_t *pIndex);

void ref_absmax_no_idx_q7(const q7_t *pSrc, uint32_t blockSize, q7_t *pResult);

void ref_absmin_q7(const q7_t *pSrc, uint32_t blockSize, q7_t *pResult,
                   uint32_t *pIndex);

void ref_absmin_no_idx_q7(const q7_t *pSrc, uint32_t blockSize, q7_t *pResult);

void ref_max_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult,
                 uint32_t *pIndex);
void ref_max_no_idx_f32(float32_t * pSrc, uint32_t blockSize, float32_t * pResult);

void ref_max_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult,
                 uint32_t *pIndex);

void ref_max_no_idx_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_max_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult,
                 uint32_t *pIndex);

void ref_max_no_idx_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_max_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult,
                uint32_t *pIndex);

void ref_max_no_idx_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult);

void ref_mean_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult);

void ref_mean_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_mean_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_mean_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult);

void ref_min_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult,
                 uint32_t *pIndex);

void ref_min_no_idx_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult);

void ref_min_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult,
                 uint32_t *pIndex);

void ref_min_no_idx_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_min_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult,
                 uint32_t *pIndex);

void ref_min_no_idx_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_min_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult,
                uint32_t *pIndex);

void ref_min_no_idx_q7(q7_t *pSrc, uint32_t blockSize, q7_t *pResult);

void ref_power_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult);

void ref_power_q31(q31_t *pSrc, uint32_t blockSize, q63_t *pResult);

void ref_power_q15(q15_t *pSrc, uint32_t blockSize, q63_t *pResult);

void ref_power_q7(q7_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_rms_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult);

void ref_rms_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_rms_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_std_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult);

void ref_std_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_std_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_var_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult);

void ref_var_q31(q31_t *pSrc, uint32_t blockSize, q31_t *pResult);

void ref_var_q15(q15_t *pSrc, uint32_t blockSize, q15_t *pResult);

void ref_mse_f32(float32_t *pSrcA, const float32_t *pSrcB,
                   uint32_t blockSize, float32_t *result);

void ref_mse_q31(q31_t *pSrcA, const q31_t *pSrcB, uint32_t blockSize,
                   q31_t *pResult);

void ref_mse_q15(q15_t *pSrcA, const q15_t *pSrcB, uint32_t blockSize,
                   q15_t *pResult);

void ref_mse_q7(q7_t *pSrcA, const q7_t *pSrcB, uint32_t blockSize,
                  q7_t *pResult);

/*
 * Support Functions
 */
void ref_copy_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_copy_q31(q31_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_copy_q15(q15_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_copy_q7(q7_t *pSrc, q7_t *pDst, uint32_t blockSize);

void ref_fill_f32(float32_t value, float32_t *pDst, uint32_t blockSize);

void ref_fill_q31(q31_t value, q31_t *pDst, uint32_t blockSize);

void ref_fill_q15(q15_t value, q15_t *pDst, uint32_t blockSize);

void ref_fill_q7(q7_t value, q7_t *pDst, uint32_t blockSize);

void ref_q31_to_q15(q31_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_q31_to_q7(q31_t *pSrc, q7_t *pDst, uint32_t blockSize);

void ref_q15_to_q31(q15_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_q15_to_q7(q15_t *pSrc, q7_t *pDst, uint32_t blockSize);

void ref_q7_to_q31(q7_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_q7_to_q15(q7_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_q63_to_float(q63_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_q31_to_float(q31_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_q15_to_float(q15_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_q7_to_float(q7_t *pSrc, float32_t *pDst, uint32_t blockSize);

void ref_float_to_q31(float32_t *pSrc, q31_t *pDst, uint32_t blockSize);

void ref_float_to_q15(float32_t *pSrc, q15_t *pDst, uint32_t blockSize);

void ref_float_to_q7(float32_t *pSrc, q7_t *pDst, uint32_t blockSize);

/*
 * Transform Functions
 */
void ref_cfft_f32(const riscv_cfft_instance_f32 *S, float32_t *p1,
                  uint8_t ifftFlag, uint8_t bitReverseFlag);

void ref_cfft_q31(const riscv_cfft_instance_q31 *S, q31_t *p1, uint8_t ifftFlag,
                  uint8_t bitReverseFlag);

void ref_cfft_q15(const riscv_cfft_instance_q15 *S, q15_t *p1, uint8_t ifftFlag,
                  uint8_t bitReverseFlag);

void ref_cfft_radix2_f32(const riscv_cfft_radix2_instance_f32 *S,
                         float32_t *pSrc);

void ref_cfft_radix2_q31(const riscv_cfft_radix2_instance_q31 *S, q31_t *pSrc);

void ref_cfft_radix2_q15(const riscv_cfft_radix2_instance_q15 *S, q15_t *pSrc);

void ref_cfft_radix4_f32(const riscv_cfft_radix4_instance_f32 *S,
                         float32_t *pSrc);

void ref_cfft_radix4_q31(const riscv_cfft_radix4_instance_q31 *S, q31_t *pSrc);

void ref_cfft_radix4_q15(const riscv_cfft_radix4_instance_q15 *S, q15_t *pSrc);

void ref_rfft_f32(riscv_rfft_instance_f32 *S, float32_t *pSrc, float32_t *pDst);

void ref_rfft_fast_f32(riscv_rfft_fast_instance_f32 *S, float32_t *p,
                       float32_t *pOut, uint8_t ifftFlag);

void ref_rfft_q31(const riscv_rfft_instance_q31 *S, q31_t *pSrc, q31_t *pDst);

void ref_rfft_q15(const riscv_rfft_instance_q15 *S, q15_t *pSrc, q15_t *pDst);

void ref_dct4_f32(const riscv_dct4_instance_f32 *S, float32_t *pState,
                  float32_t *pInlineBuffer);

void ref_dct4_q31(const riscv_dct4_instance_q31 *S, q31_t *pState,
                  q31_t *pInlineBuffer);

void ref_dct4_q15(const riscv_dct4_instance_q15 *S, q15_t *pState,
                  q15_t *pInlineBuffer);













/*
 * Intrinsics
 */
q31_t ref__QADD8(q31_t x, q31_t y);
q31_t ref__QSUB8(q31_t x, q31_t y);
q31_t ref__QADD16(q31_t x, q31_t y);
q31_t ref__SHADD16(q31_t x, q31_t y);
q31_t ref__QSUB16(q31_t x, q31_t y);
q31_t ref__SHSUB16(q31_t x, q31_t y);
q31_t ref__QASX(q31_t x, q31_t y);
q31_t ref__SHASX(q31_t x, q31_t y);
q31_t ref__QSAX(q31_t x, q31_t y);
q31_t ref__SHSAX(q31_t x, q31_t y);
q31_t ref__SMUSDX(q31_t x, q31_t y);
q31_t ref__SMUADX(q31_t x, q31_t y);
q31_t ref__QADD(q31_t x, q31_t y);
q31_t ref__QSUB(q31_t x, q31_t y);
q31_t ref__SMLAD(q31_t x, q31_t y, q31_t sum);
q31_t ref__SMLADX(q31_t x, q31_t y, q31_t sum);
q31_t ref__SMLSDX(q31_t x, q31_t y, q31_t sum);
q63_t ref__SMLALD(q31_t x, q31_t y, q63_t sum);
q63_t ref__SMLALDX(q31_t x, q31_t y, q63_t sum);
q31_t ref__SMUAD(q31_t x, q31_t y);
q31_t ref__SMUSD(q31_t x, q31_t y);
q31_t ref__SXTB16(q31_t x);

#ifdef __cplusplus
}
#endif

#endif
