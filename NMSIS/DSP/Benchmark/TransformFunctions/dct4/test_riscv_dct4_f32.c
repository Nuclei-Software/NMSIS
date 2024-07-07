#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/dct4_f32/test_data.h"

BENCH_DECLARE_VAR();

void dct4_riscv_dct4_f32(void)
{
    riscv_cfft_radix4_instance_f32 S;
    riscv_rfft_instance_f32 SS;

    generate_rand_f32(dct4_testinput_f32_50hz_200Hz, DCT4SIZE);

    // the way to init the SSS not directly use riscv_dct4_init_f32 is to avoid the question:
    // `.data' will not fit in region `ram' when DLM_SIZE=512K 
    riscv_status result = riscv_cfft_radix4_init_f32(&S, DCT4SIZE / 2, 0, 1);
    result = riscv_rfft_init_f32(&SS, &S, DCT4SIZE, 0, 1);
    riscv_dct4_instance_f32 SSS = {DCT4SIZE, DCT4SIZE / 2, 0.125, Weights_128, cos_factors_128, &SS, &S};

    BENCH_START(riscv_dct4_f32);
    riscv_dct4_f32(&SSS, f32_state, dct4_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_dct4_f32);

//    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
