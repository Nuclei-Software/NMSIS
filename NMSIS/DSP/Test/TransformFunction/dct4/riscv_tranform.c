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

int test_flag_error = 0;

uint32_t fftSize = 1024;
uint8_t ifftFlag = 0;
uint8_t doBitReverse = 1;

// f32
float32_t f32_state[256] = {0};
float32_t dct4_testinput_f32_50hz_200Hz[256];
float32_t dct4_testinput_f32_50hz_200Hz_ref[256];
float32_t dct4_testinput_f32_50hz_200Hz_q31[256];
//q31_t
q31_t q31_state[256] = {0};

q31_t dct4_testinput_q31_50hz_200Hz[256];
q31_t dct4_testinput_q31_50hz_200Hz_ref[256];

//q15_t
q15_t q15_state[256] = {0};
q15_t dct4_testinput_q15_50hz_200Hz[256];
q15_t dct4_testinput_q15_50hz_200Hz_ref[256];

BENCH_DECLARE_VAR();
static int DSP_dct4_f32(void)
{
    generate_rand_f32(dct4_testinput_f32_50hz_200Hz, 256);
    memcpy(dct4_testinput_f32_50hz_200Hz_ref, dct4_testinput_f32_50hz_200Hz, sizeof(dct4_testinput_f32_50hz_200Hz));
    fftSize = 128;
    riscv_cfft_radix4_instance_f32 S;
    riscv_cfft_radix4_init_f32(&S, 64, ifftFlag, doBitReverse);
    riscv_rfft_instance_f32 SS;
    riscv_rfft_init_f32(&SS, &S, fftSize, ifftFlag, doBitReverse);
    riscv_dct4_instance_f32 SSS = {128, 64, 0.125, Weights_128, cos_factors_128,
                                 &SS, &S};
    BENCH_START(riscv_dct4_f32);
    riscv_dct4_f32(&SSS, f32_state, dct4_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_dct4_f32);
    riscv_rfft_init_f32(&SS, &S, fftSize, ifftFlag, doBitReverse);
    ref_dct4_f32(&SSS, f32_state, dct4_testinput_f32_50hz_200Hz_ref);
    float32_t resault, resault_ref;
    uint32_t index, index_ref;
    riscv_max_f32(dct4_testinput_f32_50hz_200Hz, 128, &resault, &index);
    riscv_max_f32(dct4_testinput_f32_50hz_200Hz_ref,128,&resault_ref,&index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_dct4_f32);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dct4_f32);
}
static int DSP_dct4_q31(void)
{
    generate_rand_q31(dct4_testinput_q31_50hz_200Hz, 256);
    memcpy(dct4_testinput_q31_50hz_200Hz_ref, dct4_testinput_q31_50hz_200Hz, sizeof(dct4_testinput_q31_50hz_200Hz));
    fftSize = 128;

    riscv_cfft_radix4_instance_q31 S;

    riscv_rfft_instance_q31 SS;
    riscv_dct4_instance_q31 SSS;
    riscv_dct4_init_q31(&SSS, &SS, &S, 128, 64, 0x10000000);
    BENCH_START(riscv_dct4_q31);
    riscv_dct4_q31(&SSS, q31_state, dct4_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_dct4_q31);
    riscv_dct4_init_q31(&SSS, &SS, &S, 128, 64, 0x10000000);
    ref_dct4_q31(&SSS, q31_state, dct4_testinput_q31_50hz_200Hz_ref);
    q31_t resault, resault_ref;
    uint32_t index, index_ref;
    riscv_shift_q31(dct4_testinput_q31_50hz_200Hz, 7,
                  dct4_testinput_q31_50hz_200Hz, fftSize);
    riscv_shift_q31(dct4_testinput_q31_50hz_200Hz_ref,7,dct4_testinput_q31_50hz_200Hz_ref,fftSize);
    riscv_max_q31(dct4_testinput_q31_50hz_200Hz, 128, &resault, &index);
    riscv_max_q31(dct4_testinput_q31_50hz_200Hz_ref,128,&resault_ref,&index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_dct4_q31);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dct4_q31);
}
static int DSP_dct4_q15(void)
{
    // generate_rand_q15(dct4_testinput_q15_50hz_200Hz, 256);
    // riscv_float_to_q15(dct4_testinput_f32_50hz_200Hz, dct4_testinput_q15_50hz_200Hz, 256);
    for (int i = 0; i < 256; i++) {
        dct4_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(dct4_testinput_q15_50hz_200Hz_ref, dct4_testinput_q15_50hz_200Hz, sizeof(dct4_testinput_q15_50hz_200Hz));
    fftSize = 128;

    riscv_cfft_radix4_instance_q15 S;
    riscv_rfft_instance_q15 SS;
    riscv_dct4_instance_q15 SSS;
    riscv_dct4_init_q15(&SSS, &SS, &S, fftSize, fftSize / 2, 0x1000);
    BENCH_START(riscv_dct4_q15);
    riscv_dct4_q15(&SSS, q15_state, dct4_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_dct4_q15);
    riscv_dct4_init_q15(&SSS, &SS, &S, fftSize, fftSize / 2, 0x1000);
    ref_dct4_q15(&SSS, q15_state, dct4_testinput_q15_50hz_200Hz_ref);
    q15_t resault, resault_ref;
    uint32_t index, index_ref;
    riscv_shift_q15(dct4_testinput_q15_50hz_200Hz, 6,
                  dct4_testinput_q15_50hz_200Hz, fftSize);
    riscv_max_q15(dct4_testinput_q15_50hz_200Hz, fftSize, &resault, &index);
    riscv_shift_q15(dct4_testinput_q15_50hz_200Hz_ref, 6, dct4_testinput_q15_50hz_200Hz_ref, fftSize);
    riscv_max_q15(dct4_testinput_q15_50hz_200Hz_ref,fftSize,&resault_ref,&index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_dct4_q15);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_dct4_q15);
}

int main()
{
    BENCH_INIT();

    DSP_dct4_f32();
    DSP_dct4_q31();
    DSP_dct4_q15();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
