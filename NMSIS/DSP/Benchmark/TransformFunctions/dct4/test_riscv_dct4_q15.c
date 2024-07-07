#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/dct4_q15/test_data.h"

BENCH_DECLARE_VAR();

void dct4_riscv_dct4_q15(void)
{
  //  riscv_cfft_radix4_instance_q15 S;
  //  riscv_rfft_instance_q15 SS;
  //  riscv_dct4_instance_q15 SSS;

    generate_rand_q15(dct4_testinput_q15_50hz_200Hz, DCT4SIZE);

    riscv_cfft_radix4_instance_q15 S;
    riscv_status result = riscv_cfft_radix4_init_q15(&S, DCT4SIZE / 2, 0, 1);
    riscv_rfft_instance_q15 SS;
    riscv_rfft_init_q15(&SS, DCT4SIZE, 0, 1);
    riscv_dct4_instance_q15 SSS = {DCT4SIZE, DCT4SIZE / 2, normalize, WeightsQ15_128, cos_factorsQ15_128,
                                 &SS, &S};

//    riscv_status result = riscv_dct4_init_q15(&SSS, &SS, &S, DCT4SIZE, DCT4SIZE / 2, normalize);
    BENCH_START(riscv_dct4_q15);
    riscv_dct4_q15(&SSS, q15_state, dct4_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_dct4_q15);

//    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
