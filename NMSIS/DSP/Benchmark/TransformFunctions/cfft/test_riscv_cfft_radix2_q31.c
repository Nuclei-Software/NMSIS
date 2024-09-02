#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/cfft_radix2_q31/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_radix2_q31(void)
{
    generate_rand_q31(cfft_testinput_q31_50hz_200Hz, CFFTSIZE * 2);

    riscv_cfft_radix2_instance_q31 q31_S;
    riscv_status result = riscv_cfft_radix2_init_q31(&q31_S, CFFTSIZE, ifftFlag, doBitReverse);

    BENCH_START(riscv_cfft_radix2_q31);
    riscv_cfft_radix2_q31(&q31_S, cfft_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}
