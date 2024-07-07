#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/dct4_q31/test_data.h"

BENCH_DECLARE_VAR();

void dct4_riscv_dct4_q31(void)
{
 
    generate_rand_q31(dct4_testinput_q31_50hz_200Hz, DCT4SIZE);

    riscv_cfft_radix4_instance_q31 S;

    riscv_rfft_instance_q31 SS;
    riscv_dct4_instance_q31 SSS;
    riscv_status result = riscv_dct4_init_q31(&SSS, &SS, &S, DCT4SIZE, DCT4SIZE / 2, normalize);
    BENCH_START(riscv_dct4_q31);
    riscv_dct4_q31(&SSS, q31_state, dct4_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_dct4_q31);	
	
    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
