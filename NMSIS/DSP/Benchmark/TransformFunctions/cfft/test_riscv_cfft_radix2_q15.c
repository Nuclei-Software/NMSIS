#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/cfft_radix2_q15/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_radix2_q15(void)
{
    generate_rand_q15(cfft_testinput_q15_50hz_200Hz, CFFTSIZE * 2);

    riscv_cfft_radix2_instance_q15 q15_S;
    riscv_status result = riscv_cfft_radix2_init_q15(&q15_S, CFFTSIZE, ifftFlag, doBitReverse);

    BENCH_START(riscv_cfft_radix2_q15);
    riscv_cfft_radix2_q15(&q15_S, cfft_testinput_q15_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_q15);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}
