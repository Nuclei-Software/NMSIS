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
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;

//q31_t
q31_t rfft_testinput_q31_50hz_200Hz[1024];
q31_t rfft_testinput_q31_50hz_200Hz_ref[1024];
q31_t q31_testOutput[1024];
q31_t q31_testOutput_ref[1024];

//q15_t
q15_t rfft_testinput_q15_50hz_200Hz[1024];
q15_t rfft_testinput_q15_50hz_200Hz_ref[1024] = {0};
q15_t q15_testOutput[1024];
q15_t q15_testOutput_ref[1024];

// f32
float32_t rfft_testinput_f32_50hz_200Hz[512];
float32_t rfft_testinput_f32_50hz_200Hz_ref[512];
float32_t f32_testOutput[512];
float32_t f32_testOutput_ref[512];
float32_t rfft_testinput_f32_50hz_200Hz_fast[512];
float32_t rfft_testinput_f32_50hz_200Hz_fast_ref[512];

BENCH_DECLARE_VAR();
static int DSP_rfft_q31(void)
{
    generate_rand_q31(rfft_testinput_q31_50hz_200Hz, 1024);
    memcpy(rfft_testinput_q31_50hz_200Hz_ref, rfft_testinput_q31_50hz_200Hz, sizeof(rfft_testinput_q31_50hz_200Hz));
    riscv_rfft_instance_q31 SS;
    fftSize = 512;
    ifftFlag = 0;
    doBitReverse = 1;
    riscv_rfft_init_q31(&SS, fftSize, ifftFlag, doBitReverse);
    BENCH_START(riscv_rfft_q31);
    riscv_rfft_q31(&SS, rfft_testinput_q31_50hz_200Hz, q31_testOutput);
    BENCH_END(riscv_rfft_q31);
    riscv_rfft_init_q31(&SS, fftSize, ifftFlag, doBitReverse);
    ref_rfft_q31(&SS, rfft_testinput_q31_50hz_200Hz_ref, q31_testOutput_ref);
    q31_t resault, resault_ref;
    uint32_t index, index_ref;
    riscv_shift_q31(q31_testOutput, 8, q31_testOutput, fftSize);
    riscv_shift_q31(q31_testOutput_ref, 8, q31_testOutput_ref, fftSize);
    riscv_max_q31(q31_testOutput, 512, &resault, &index);
    riscv_max_q31(q31_testOutput_ref, 512, &resault_ref, &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_rfft_q31);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rfft_q31);
}
static int DSP_rfft_q15(void)
{
    // generate_rand_q15(rfft_testinput_q15_50hz_200Hz, 1024);
    for (int i = 0; i < 256; i++) {
        rfft_testinput_q15_50hz_200Hz[i] = (q15_t)(rand() % Q15_MAX);
    }
    memcpy(rfft_testinput_q15_50hz_200Hz_ref, rfft_testinput_q15_50hz_200Hz, sizeof(rfft_testinput_q15_50hz_200Hz));
    riscv_rfft_instance_q15 SS;
    fftSize = 512;
    ifftFlag = 0;
    doBitReverse = 1;
    riscv_rfft_init_q15(&SS, fftSize, ifftFlag, doBitReverse);
    BENCH_START(riscv_rfft_q15);
    riscv_rfft_q15(&SS, rfft_testinput_q15_50hz_200Hz, q15_testOutput);
    BENCH_END(riscv_rfft_q15);
    riscv_rfft_init_q15(&SS, fftSize, ifftFlag, doBitReverse);
    ref_rfft_q15(&SS, rfft_testinput_q15_50hz_200Hz_ref, q15_testOutput_ref);
    q15_t resault, resault_ref;
    riscv_shift_q15(q15_testOutput, 6, q15_testOutput, fftSize);
    riscv_shift_q15(q15_testOutput_ref, 6, q15_testOutput_ref, fftSize);
    uint32_t index, index_ref;
    riscv_max_q15(q15_testOutput, fftSize, &resault, &index);
    riscv_max_q15(q15_testOutput_ref, fftSize, &resault_ref, &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_rfft_q15);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rfft_q15);
}

static int DSP_rfft_fast_f32(void)
{
    generate_rand_f32(rfft_testinput_f32_50hz_200Hz_fast, 512);
    memcpy(rfft_testinput_f32_50hz_200Hz_fast_ref, rfft_testinput_f32_50hz_200Hz_fast, sizeof(rfft_testinput_f32_50hz_200Hz_fast));
    riscv_rfft_fast_instance_f32 SS;

    riscv_rfft_fast_init_f32(&SS, 512);
    BENCH_START(riscv_rfft_fast_f32);
    riscv_rfft_fast_f32(&SS, rfft_testinput_f32_50hz_200Hz_fast, f32_testOutput, 0);
    BENCH_END(riscv_rfft_fast_f32);
    riscv_rfft_fast_init_f32(&SS, 512);
    ref_rfft_fast_f32(&SS, rfft_testinput_f32_50hz_200Hz_fast_ref,
                      f32_testOutput_ref, 0);
    float32_t resault, resault_ref;
    uint32_t index, index_ref;
    riscv_max_f32(f32_testOutput, 512, &resault, &index);
    riscv_max_f32(f32_testOutput_ref, 512, &resault_ref, &index_ref);

    if (index != index_ref) {
        BENCH_ERROR(riscv_rfft_fast_f32);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rfft_fast_f32);
}
static int DSP_rfft_f32(void)
{
    riscv_rfft_instance_f32 SS;
    riscv_cfft_radix4_instance_f32 S_CFFT;
    generate_rand_f32(rfft_testinput_f32_50hz_200Hz, 512);
    memcpy(rfft_testinput_f32_50hz_200Hz_ref, rfft_testinput_f32_50hz_200Hz, sizeof(rfft_testinput_f32_50hz_200Hz));

    fftSize = 512;
    ifftFlag = 0;
    doBitReverse = 1;
    riscv_cfft_radix4_init_f32(&S_CFFT, 256, ifftFlag, doBitReverse);
    riscv_rfft_init_f32(&SS, &S_CFFT, fftSize, ifftFlag, doBitReverse);
    BENCH_START(riscv_rfft_f32);
    riscv_rfft_f32(&SS, rfft_testinput_f32_50hz_200Hz, f32_testOutput);
    BENCH_END(riscv_rfft_f32);
    riscv_cfft_radix4_init_f32(&S_CFFT, 256, ifftFlag, doBitReverse);
    riscv_rfft_init_f32(&SS, &S_CFFT, fftSize, ifftFlag, doBitReverse);
    riscv_cfft_radix4_init_f32(&S_CFFT, 256, ifftFlag, doBitReverse);
    ref_rfft_f32(&SS, rfft_testinput_f32_50hz_200Hz_ref, f32_testOutput_ref);
    float32_t resault, resault_ref;
    uint32_t index, index_ref;
    riscv_max_f32(f32_testOutput, fftSize, &resault, &index);
    riscv_max_f32(f32_testOutput_ref, fftSize, &resault_ref, &index_ref);
    if (index != index_ref) {
        BENCH_ERROR(riscv_rfft_f32);
        printf("expect: %d, actual: %d\n", index_ref, index);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_rfft_f32);
}

int main()
{
    BENCH_INIT();

    DSP_rfft_q31();
    DSP_rfft_q15();
    DSP_rfft_f32();
    DSP_rfft_fast_f32();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
