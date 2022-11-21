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
float32_t cfft_testinput_f32_50hz_200Hz[1024] __attribute__((aligned(16)));
float32_t cfft_testinput_f32_50hz_200Hz_ref[1024] __attribute__((aligned(16)));
q31_t cfft_testinput_q31_50hz_200Hz[1024] __attribute__((aligned(16)));
q31_t cfft_testinput_q31_50hz_200Hz_ref[1024] __attribute__((aligned(16)));
q15_t cfft_testinput_q15_50hz_200Hz_ref[1024] __attribute__((aligned(16)));
q15_t cfft_testinput_q15_50hz_200Hz[1024] __attribute__((aligned(16)));

int test_flag_error = 0;

uint32_t fftSize = 512;
uint8_t ifftFlag = 0;
uint8_t doBitReverse = 1;

BENCH_DECLARE_VAR();

void DSP_cfft_test(void)
{
    // f32
    uint32_t index, index_ref;

    generate_rand_f32(cfft_testinput_f32_50hz_200Hz, 1024);
    memcpy(cfft_testinput_f32_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz, sizeof(cfft_testinput_f32_50hz_200Hz));
    BENCH_START(riscv_cfft_f32);
    riscv_cfft_f32(&riscv_cfft_sR_f32_len512, cfft_testinput_f32_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_f32);
    ref_cfft_f32(&riscv_cfft_sR_f32_len512, cfft_testinput_f32_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    float32_t f32_result, f32_result_ref;
    riscv_max_f32(cfft_testinput_f32_50hz_200Hz, 1024, &f32_result, &index);
    riscv_max_f32(cfft_testinput_f32_50hz_200Hz_ref, 1024, &f32_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_f32);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_f32);

    // q31
    generate_rand_q31(cfft_testinput_q31_50hz_200Hz, 1024);
    memcpy(cfft_testinput_q31_50hz_200Hz_ref, cfft_testinput_q31_50hz_200Hz, sizeof(cfft_testinput_q31_50hz_200Hz));

    BENCH_START(riscv_cfft_q31);
    riscv_cfft_q31(&riscv_cfft_sR_q31_len512, cfft_testinput_q31_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_q31);
    ref_cfft_q31(&riscv_cfft_sR_q31_len512, cfft_testinput_q31_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    q31_t q31_result, q31_result_ref;
    riscv_max_q31(cfft_testinput_q31_50hz_200Hz, 1024, &q31_result, &index);
    riscv_max_q31(cfft_testinput_q31_50hz_200Hz_ref, 1024, &q31_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_q31);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_q31);

    // q15
    // generate_rand_q15(cfft_testinput_q15_50hz_200Hz, 1024);
    for (int i = 0; i < 1024; i++) {
        cfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(cfft_testinput_q15_50hz_200Hz_ref, cfft_testinput_q15_50hz_200Hz, sizeof(cfft_testinput_q15_50hz_200Hz));
    BENCH_START(riscv_cfft_q15);
    riscv_cfft_q15(&riscv_cfft_sR_q15_len512, cfft_testinput_q15_50hz_200Hz,
                 ifftFlag, doBitReverse);
    BENCH_END(riscv_cfft_q15);
    riscv_cfft_q15(&riscv_cfft_sR_q15_len512, cfft_testinput_q15_50hz_200Hz_ref,
                 ifftFlag, doBitReverse);
    q15_t q15_result, q15_result_ref;
    riscv_max_q15(cfft_testinput_q15_50hz_200Hz, 1024, &q15_result, &index);
    riscv_max_q15(cfft_testinput_q15_50hz_200Hz_ref, 1024, &q15_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_q15);
        printf("expect: %d, actual: %d\n", index_ref, index);
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

#define FFT_DOT 1024
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

    // generate_rand_f32(cfft_testinput_f32_50hz_200Hz, 1024);
    memcpy(cfft_testinput_f32_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz, sizeof(cfft_testinput_f32_50hz_200Hz));
    fftSize = 512;
    riscv_cfft_radix2_instance_f32 f32_S;
    riscv_cfft_radix2_init_f32(&f32_S, fftSize, ifftFlag, doBitReverse);
    init_bitrev(fftSize);       //generate bit reverse index group

    BENCH_START(riscv_cfft_radix2_f32);
    riscv_cfft_radix2_f32(&f32_S, cfft_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_f32);
    riscv_cfft_radix2_init_f32(&f32_S, fftSize, ifftFlag, doBitReverse);
    ref_cfft_radix2_f32(&f32_S, cfft_testinput_f32_50hz_200Hz_ref);
    float32_t f32_result, f32_result_ref;
    riscv_max_f32(cfft_testinput_f32_50hz_200Hz, 1024, &f32_result, &index);
    riscv_max_f32(cfft_testinput_f32_50hz_200Hz_ref, 1024, &f32_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_radix2_f32);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_f32);
    // q31
    // generate_rand_q31(cfft_testinput_q31_50hz_200Hz, 1024);
    memcpy(cfft_testinput_q31_50hz_200Hz_ref, cfft_testinput_q31_50hz_200Hz, sizeof(cfft_testinput_q31_50hz_200Hz));
    fftSize = 512;
    riscv_cfft_radix2_instance_q31 q31_S;
    riscv_cfft_radix2_init_q31(&q31_S, 512, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix2_q31);
    riscv_cfft_radix2_q31(&q31_S, cfft_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_q31);
    riscv_cfft_radix2_init_q31(&q31_S, 512, ifftFlag, doBitReverse);
    ref_cfft_radix2_q31(&q31_S, cfft_testinput_q31_50hz_200Hz_ref);
    q31_t q31_result, q31_result_ref;
    riscv_max_q31(cfft_testinput_q31_50hz_200Hz, 1024, &q31_result, &index);
    riscv_max_q31(cfft_testinput_q31_50hz_200Hz_ref, 1024, &q31_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_radix2_q31);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_q31);

    // q15
    // generate_rand_q15(cfft_testinput_q15_50hz_200Hz, 1024);
    for (int i = 0; i < 1024; i++) {
        cfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(cfft_testinput_q15_50hz_200Hz_ref, cfft_testinput_q15_50hz_200Hz, sizeof(cfft_testinput_q15_50hz_200Hz));
    fftSize = 512;
    riscv_cfft_radix2_instance_q15 q15_S;
    riscv_cfft_radix2_init_q15(&q15_S, 512, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix2_q15);
    riscv_cfft_radix2_q15(&q15_S, cfft_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_q15);
    riscv_cfft_radix2_init_q15(&q15_S, 512, ifftFlag, doBitReverse);
    ref_cfft_radix2_q15(&q15_S, cfft_testinput_q15_50hz_200Hz_ref);
    q15_t q15_result, q15_result_ref;
    riscv_max_q15(cfft_testinput_q15_50hz_200Hz, 1024, &q15_result, &index);
        riscv_max_q15(cfft_testinput_q15_50hz_200Hz, 1024, &q15_result_ref, &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_radix2_q15);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix2_q15);
}

void DSP_cfftx4_test(void)
{
    uint32_t index, index_ref;
    // f32
    //  generate_rand_f32(cfft_testinput_f32_50hz_200Hz, 512);
    memcpy(cfft_testinput_f32_50hz_200Hz_ref, cfft_testinput_f32_50hz_200Hz, 512 * sizeof(float32_t));

    fftSize = 256;

    riscv_cfft_radix4_instance_f32 f32_S;
    riscv_cfft_radix4_init_f32(&f32_S, 256, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_f32);
    riscv_cfft_radix4_f32(&f32_S, cfft_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_f32);
    riscv_cfft_radix4_init_f32(&f32_S, 256, ifftFlag, doBitReverse);
    ref_cfft_radix4_f32(&f32_S, cfft_testinput_f32_50hz_200Hz_ref);
    float32_t f32_result, f32_result_ref;
    riscv_max_f32(cfft_testinput_f32_50hz_200Hz, 256, &f32_result, &index);
    riscv_max_f32(cfft_testinput_f32_50hz_200Hz_ref, 256, &f32_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_radix4_f32);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix4_f32);
    // q31
    fftSize = 256;
    // generate_rand_q31(cfft_testinput_q31_50hz_200Hz, 512);
    memcpy(cfft_testinput_q31_50hz_200Hz_ref, cfft_testinput_q31_50hz_200Hz, 512 * sizeof(q31_t));
    riscv_cfft_radix4_instance_q31 q31_S;
    riscv_cfft_radix4_init_q31(&q31_S, 256, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_q31);
    riscv_cfft_radix4_q31(&q31_S, cfft_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_q31);
    riscv_cfft_radix4_init_q31(&q31_S, 256, ifftFlag, doBitReverse);
    ref_cfft_radix4_q31(&q31_S, cfft_testinput_q31_50hz_200Hz_ref);
    q31_t q31_result, q31_result_ref;

    riscv_max_q31(cfft_testinput_q31_50hz_200Hz, 256, &q31_result, &index);
    riscv_max_q31(cfft_testinput_q31_50hz_200Hz_ref, 256, &q31_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_radix4_q31);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_cfft_radix4_q31);
    // q15
    fftSize = 256;
    //generate_rand_q15(cfft_testinput_q15_50hz_200Hz, 512);
    for (int i = 0; i < 512; i++) {
        cfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(cfft_testinput_q15_50hz_200Hz_ref, cfft_testinput_q15_50hz_200Hz, 512 * sizeof(q15_t));

    riscv_cfft_radix4_instance_q15 q15_S;
    riscv_cfft_radix4_init_q15(&q15_S, 256, ifftFlag, doBitReverse);
    BENCH_START(riscv_cfft_radix4_q15);
    riscv_cfft_radix4_q15(&q15_S, cfft_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_q15);
    riscv_cfft_radix4_init_q15(&q15_S, 256, ifftFlag, doBitReverse);
    ref_cfft_radix4_q15(&q15_S, cfft_testinput_q15_50hz_200Hz_ref);
    q15_t q15_result, q15_result_ref;
    riscv_max_q15(cfft_testinput_q15_50hz_200Hz, 256, &q15_result, &index);
    riscv_max_q15(cfft_testinput_q15_50hz_200Hz_ref, 256, &q15_result_ref,
                &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_cfft_radix4_q15);
        printf("expect: %d, actual: %d\n", index_ref, index);
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
