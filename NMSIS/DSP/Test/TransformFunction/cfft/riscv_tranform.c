//
// Created by lujun on 19-6-28.
//
// This contains SIN_COS , clarke, inv_clarke, park, inv_park and pid
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "riscv_math.h"
#include <stdint.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

#define CFFTSIZE 512
#define SNR_THRESHOLD_F16 58
#define SNR_THRESHOLD_F32 120
#define SNR_THRESHOLD_Q15 30
#define SNR_THRESHOLD_Q31 90

float32_t scratchArray[CFFTSIZE * 2];

float32_t cfft_testinput_f32_50hz_200Hz[CFFTSIZE * 2] __attribute__((aligned(16)));
float32_t cfft_testinput_f32_50hz_200Hz_ref[CFFTSIZE * 2] __attribute__((aligned(16)));
float32_t testOutput_f32[CFFTSIZE * 2], testOutput_f32_ref[CFFTSIZE * 2];

#if defined (RISCV_FLOAT16_SUPPORTED)
float16_t cfft_testinput_f16_50hz_200Hz[CFFTSIZE * 2] __attribute__((aligned(16)));
float16_t cfft_testinput_f16_50hz_200Hz_ref[CFFTSIZE * 2] __attribute__((aligned(16)));
float16_t testOutput_f16[CFFTSIZE], testOutput_f16_ref[CFFTSIZE];
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

q31_t cfft_testinput_q31_50hz_200Hz[CFFTSIZE * 2] __attribute__((aligned(16)));
q31_t cfft_testinput_q31_50hz_200Hz_ref[CFFTSIZE * 2] __attribute__((aligned(16)));

q15_t cfft_testinput_q15_50hz_200Hz[CFFTSIZE * 2] __attribute__((aligned(16)));
q15_t cfft_testinput_q15_50hz_200Hz_ref[CFFTSIZE * 2] __attribute__((aligned(16)));

int test_flag_error = 0;

uint8_t ifftFlag = 0;
uint8_t doBitReverse = 1;

BENCH_DECLARE_VAR();

void DSP_cfft_test(void)
{
    // f32
    uint32_t index, index_ref;

    generate_rand_f32(cfft_testinput_f32_50hz_200Hz, CFFTSIZE * 2);
    memcpy(cfft_testinput_f32_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz, sizeof(cfft_testinput_f32_50hz_200Hz));
    BENCH_START(riscv_cfft_f32);
    riscv_cfft_f32(&riscv_cfft_sR_f32_len512, cfft_testinput_f32_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_f32);
    ref_cfft_f32(&riscv_cfft_sR_f32_len512, cfft_testinput_f32_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    float snr = riscv_snr_f32(cfft_testinput_f32_50hz_200Hz, cfft_testinput_f32_50hz_200Hz_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_cfft_f32);
        printf("riscv_cfft_f32 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_f32);

   // f16
#if defined (RISCV_FLOAT16_SUPPORTED)
    generate_rand_f16(cfft_testinput_f16_50hz_200Hz, CFFTSIZE * 2);
    memcpy(cfft_testinput_f16_50hz_200Hz_ref, cfft_testinput_f16_50hz_200Hz, sizeof(cfft_testinput_f16_50hz_200Hz));
    BENCH_START(riscv_cfft_f16);
    riscv_cfft_f16(&riscv_cfft_sR_f16_len512, cfft_testinput_f16_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_f16);
    ref_cfft_f16(&riscv_cfft_sR_f16_len512, cfft_testinput_f16_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    riscv_f16_to_float(cfft_testinput_f16_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz_ref, CFFTSIZE * 2);
    riscv_f16_to_float(cfft_testinput_f16_50hz_200Hz, cfft_testinput_f32_50hz_200Hz, CFFTSIZE * 2);
    snr = riscv_snr_f32(cfft_testinput_f32_50hz_200Hz, cfft_testinput_f32_50hz_200Hz_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_F16) {
        BENCH_ERROR(riscv_cfft_f16);
        printf("riscv_cfft_f16 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_f16);
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

    // q31
    generate_rand_q31(cfft_testinput_q31_50hz_200Hz, CFFTSIZE * 2);
    memcpy(cfft_testinput_q31_50hz_200Hz_ref, cfft_testinput_q31_50hz_200Hz, sizeof(cfft_testinput_q31_50hz_200Hz));

    BENCH_START(riscv_cfft_q31);
    riscv_cfft_q31(&riscv_cfft_sR_q31_len512, cfft_testinput_q31_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_q31);
    ref_cfft_q31(&riscv_cfft_sR_q31_len512, cfft_testinput_q31_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    riscv_q31_to_float(cfft_testinput_q31_50hz_200Hz_ref, testOutput_f32_ref, CFFTSIZE * 2);
    riscv_q31_to_float(cfft_testinput_q31_50hz_200Hz, testOutput_f32, CFFTSIZE * 2);
    snr = riscv_snr_f32(testOutput_f32, testOutput_f32_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_Q31) {
        BENCH_ERROR(riscv_cfft_q31);
        printf("riscv_cfft_q31 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_q31);

    // q15
    // generate_rand_q15(cfft_testinput_q15_50hz_200Hz, 1024);
    for (int i = 0; i < CFFTSIZE * 2; i++) {
        cfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(cfft_testinput_q15_50hz_200Hz_ref, cfft_testinput_q15_50hz_200Hz, sizeof(cfft_testinput_q15_50hz_200Hz));
    BENCH_START(riscv_cfft_q15);
    riscv_cfft_q15(&riscv_cfft_sR_q15_len512, cfft_testinput_q15_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_q15);
    ref_cfft_q15(&riscv_cfft_sR_q15_len512, cfft_testinput_q15_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    riscv_q15_to_float(cfft_testinput_q15_50hz_200Hz_ref, testOutput_f32_ref, CFFTSIZE * 2);
    riscv_q15_to_float(cfft_testinput_q15_50hz_200Hz, testOutput_f32, CFFTSIZE * 2);
    snr = riscv_snr_f32(testOutput_f32, testOutput_f32_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_Q15) {
        BENCH_ERROR(riscv_cfft_q15);
        printf("riscv_cfft_q15 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_q15);
}

// index: 		FFT dot index
// totalLayer:	FFT total layer number
static uint32_t reverseBits(uint32_t index,uint8_t totalLayer) {
    uint32_t rev = 0;
    for (int i = 0; i < totalLayer && index > 0; ++i) {
        rev |= (index & 1) << (totalLayer - 1 - i);
        index >>= 1;
    }
    return rev;
}

#define FFT_DOT CFFTSIZE * 2
#if defined(RISCV_MATH_VECTOR)
extern uint16_t bitrevIndexGrp [FFT_DOT];
#endif

static void init_bitrev(int fftSize)
{
#if defined(RISCV_MATH_VECTOR)
	for(uint32_t i = 0;i < fftSize;i++)
	{
        //bit reverse index
        bitrevIndexGrp[i] = reverseBits(i, (int)log2((double)fftSize));
        #ifdef DEBUG_PRINT
        printf("rev[%d]:%d\r\n",i,bitrevIndexGrp[i]);
        #endif
        //index for index load
        bitrevIndexGrp[i] = 4 * 2 * bitrevIndexGrp[i];
	}
#endif
}
void DSP_cfftx2_test(void)
{
    // f32
    uint32_t index, index_ref;

    // generate_rand_f32(cfft_testinput_f32_50hz_200Hz, CFFTSIZE * 2);
    memcpy(cfft_testinput_f32_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz, sizeof(cfft_testinput_f32_50hz_200Hz));
    riscv_cfft_radix2_instance_f32 f32_S;
    riscv_cfft_radix2_init_f32(&f32_S, CFFTSIZE, ifftFlag, doBitReverse);
    init_bitrev(CFFTSIZE); /* generate bit reverse index group */

    BENCH_START(riscv_cfft_radix2_f32);
    riscv_cfft_radix2_f32(&f32_S, cfft_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_f32);
    riscv_cfft_radix2_init_f32(&f32_S, CFFTSIZE, ifftFlag, doBitReverse);
    ref_cfft_radix2_f32(&f32_S, cfft_testinput_f32_50hz_200Hz_ref);
    float snr = riscv_snr_f32(cfft_testinput_f32_50hz_200Hz, cfft_testinput_f32_50hz_200Hz_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_cfft_radix2_f32);
        printf("riscv_cfft_radix2_f32 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_f32);

    // f16
#if defined (RISCV_FLOAT16_SUPPORTED)
    memcpy(cfft_testinput_f16_50hz_200Hz_ref, cfft_testinput_f16_50hz_200Hz, sizeof(cfft_testinput_f16_50hz_200Hz));
    riscv_cfft_radix2_instance_f16 f16_S;
    riscv_cfft_radix2_init_f16(&f16_S, CFFTSIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix2_f16);
    riscv_cfft_radix2_f16(&f16_S, cfft_testinput_f16_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_f16);
    riscv_cfft_radix2_init_f16(&f16_S, CFFTSIZE, ifftFlag, doBitReverse);
    ref_cfft_radix2_f16(&f16_S, cfft_testinput_f16_50hz_200Hz_ref);
    riscv_f16_to_float(cfft_testinput_f16_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz_ref, CFFTSIZE * 2);
    riscv_f16_to_float(cfft_testinput_f16_50hz_200Hz, cfft_testinput_f32_50hz_200Hz, CFFTSIZE * 2);
    snr = riscv_snr_f32(cfft_testinput_f32_50hz_200Hz, cfft_testinput_f32_50hz_200Hz_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_F16) {
        BENCH_ERROR(riscv_cfft_radix2_f16);
        printf("riscv_cfft_radix2_f16 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_f16);
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */

    // q31
    // generate_rand_q31(cfft_testinput_q31_50hz_200Hz, CFFTSIZE * 2);
    memcpy(cfft_testinput_q31_50hz_200Hz_ref, cfft_testinput_q31_50hz_200Hz, sizeof(cfft_testinput_q31_50hz_200Hz));
    riscv_cfft_radix2_instance_q31 q31_S;
    riscv_cfft_radix2_init_q31(&q31_S, CFFTSIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix2_q31);
    riscv_cfft_radix2_q31(&q31_S, cfft_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_q31);
    riscv_cfft_radix2_init_q31(&q31_S, CFFTSIZE, ifftFlag, doBitReverse);
    ref_cfft_radix2_q31(&q31_S, cfft_testinput_q31_50hz_200Hz_ref);
    riscv_q31_to_float(cfft_testinput_q31_50hz_200Hz_ref, testOutput_f32_ref, CFFTSIZE * 2);
    riscv_q31_to_float(cfft_testinput_q31_50hz_200Hz, testOutput_f32, CFFTSIZE * 2);
    snr = riscv_snr_f32(testOutput_f32, testOutput_f32_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_Q31) {
        BENCH_ERROR(riscv_cfft_radix2_q31);
        printf("riscv_cfft_radix2_q31 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_q31);

    // q15
    // generate_rand_q15(cfft_testinput_q15_50hz_200Hz, CFFTSIZE * 2);
    for (int i = 0; i < CFFTSIZE * 2; i++) {
        cfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(cfft_testinput_q15_50hz_200Hz_ref, cfft_testinput_q15_50hz_200Hz, sizeof(cfft_testinput_q15_50hz_200Hz));
    riscv_cfft_radix2_instance_q15 q15_S;
    riscv_cfft_radix2_init_q15(&q15_S, CFFTSIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix2_q15);
    riscv_cfft_radix2_q15(&q15_S, cfft_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_q15);
    riscv_cfft_radix2_init_q15(&q15_S, CFFTSIZE, ifftFlag, doBitReverse);
    ref_cfft_radix2_q15(&q15_S, cfft_testinput_q15_50hz_200Hz_ref);
    riscv_q15_to_float(cfft_testinput_q15_50hz_200Hz_ref, testOutput_f32_ref, CFFTSIZE * 2);
    riscv_q15_to_float(cfft_testinput_q15_50hz_200Hz, testOutput_f32, CFFTSIZE * 2);
    snr = riscv_snr_f32(testOutput_f32, testOutput_f32_ref, CFFTSIZE * 2);
    if (snr < SNR_THRESHOLD_Q15) {
        BENCH_ERROR(riscv_cfft_radix2_q15);
        printf("riscv_cfft_radix2_q15 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_q15);
}

#define CFFTx4SIZE 256
void DSP_cfftx4_test(void)
{
    uint32_t index, index_ref;
    // f32
    // generate_rand_f32(cfft_testinput_f32_50hz_200Hz, CFFTx4SIZE * 2);
    memcpy(cfft_testinput_f32_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz, CFFTx4SIZE * 2 * sizeof(float32_t));

    riscv_cfft_radix4_instance_f32 f32_S;
    riscv_cfft_radix4_init_f32(&f32_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_f32);
    riscv_cfft_radix4_f32(&f32_S, cfft_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_f32);
    riscv_cfft_radix4_init_f32(&f32_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    ref_cfft_radix4_f32(&f32_S, cfft_testinput_f32_50hz_200Hz_ref);
    float snr = riscv_snr_f32(cfft_testinput_f32_50hz_200Hz, cfft_testinput_f32_50hz_200Hz_ref, CFFTx4SIZE * 2);
    if (snr < SNR_THRESHOLD_F32) {
        BENCH_ERROR(riscv_cfft_radix4_f32);
        printf("riscv_cfft_radix4_f32 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix4_f32);
    // f16
#if defined (RISCV_FLOAT16_SUPPORTED)
    memcpy(cfft_testinput_f16_50hz_200Hz_ref, cfft_testinput_f16_50hz_200Hz, CFFTx4SIZE * 2 * sizeof(float16_t));

    riscv_cfft_radix4_instance_f16 f16_S;
    riscv_cfft_radix4_init_f16(&f16_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_f16);
    riscv_cfft_radix4_f16(&f16_S, cfft_testinput_f16_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_f16);
    riscv_cfft_radix4_init_f16(&f16_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    ref_cfft_radix4_f16(&f16_S, cfft_testinput_f16_50hz_200Hz_ref);
    riscv_f16_to_float(cfft_testinput_f16_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz_ref, CFFTx4SIZE * 2);
    riscv_f16_to_float(cfft_testinput_f16_50hz_200Hz, cfft_testinput_f32_50hz_200Hz, CFFTx4SIZE * 2);
    snr = riscv_snr_f32(cfft_testinput_f32_50hz_200Hz, cfft_testinput_f32_50hz_200Hz_ref, CFFTx4SIZE * 2);
    if (snr < SNR_THRESHOLD_F16) {
        BENCH_ERROR(riscv_cfft_radix4_f16);
        printf("riscv_cfft_radix4_f16 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix4_f16);
#endif /* defined (RISCV_FLOAT16_SUPPORTED) */
    // q31
    // generate_rand_q31(cfft_testinput_q31_50hz_200Hz, CFFTx4SIZE * 2);
    memcpy(cfft_testinput_q31_50hz_200Hz_ref, cfft_testinput_q31_50hz_200Hz, CFFTx4SIZE * 2 * sizeof(q31_t));
    riscv_cfft_radix4_instance_q31 q31_S;
    riscv_cfft_radix4_init_q31(&q31_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_q31);
    riscv_cfft_radix4_q31(&q31_S, cfft_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_q31);
    riscv_cfft_radix4_init_q31(&q31_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    ref_cfft_radix4_q31(&q31_S, cfft_testinput_q31_50hz_200Hz_ref);
    riscv_q31_to_float(cfft_testinput_q31_50hz_200Hz_ref, testOutput_f32_ref, CFFTx4SIZE * 2);
    riscv_q31_to_float(cfft_testinput_q31_50hz_200Hz, testOutput_f32, CFFTx4SIZE * 2);
    snr = riscv_snr_f32(testOutput_f32, testOutput_f32_ref, CFFTx4SIZE * 2);
    if (snr < SNR_THRESHOLD_Q31) {
        BENCH_ERROR(riscv_cfft_radix4_q31);
        printf("riscv_cfft_radix4_q31 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix4_q31);
    // q15
    //generate_rand_q15(cfft_testinput_q15_50hz_200Hz, CFFTx4SIZE * 2);
    for (int i = 0; i < CFFTx4SIZE * 2; i++) {
        cfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(cfft_testinput_q15_50hz_200Hz_ref, cfft_testinput_q15_50hz_200Hz, CFFTx4SIZE * 2 * sizeof(q15_t));

    riscv_cfft_radix4_instance_q15 q15_S;
    riscv_cfft_radix4_init_q15(&q15_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_q15);
    riscv_cfft_radix4_q15(&q15_S, cfft_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_q15);
    riscv_cfft_radix4_init_q15(&q15_S, CFFTx4SIZE, ifftFlag, doBitReverse);
    ref_cfft_radix4_q15(&q15_S, cfft_testinput_q15_50hz_200Hz_ref);
    riscv_q15_to_float(cfft_testinput_q15_50hz_200Hz_ref, testOutput_f32_ref, CFFTx4SIZE * 2);
    riscv_q15_to_float(cfft_testinput_q15_50hz_200Hz, testOutput_f32, CFFTx4SIZE * 2);
    snr = riscv_snr_f32(testOutput_f32, testOutput_f32_ref, CFFTx4SIZE * 2);
    if (snr < SNR_THRESHOLD_Q15) {
        BENCH_ERROR(riscv_cfft_radix4_q15);
        printf("riscv_cfft_radix4_q15 failed with snr:%f\n", snr);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix4_q15);
}

int main(void)
{
    BENCH_INIT();

    DSP_cfft_test();
    DSP_cfftx2_test();
    DSP_cfftx4_test();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
