#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/cfft_radix2_f16/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_radix2_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)

    generate_rand_f16(cfft_testinput_f16_50hz_200Hz, CFFTSIZE * 2);

    riscv_cfft_radix2_instance_f16 f16_S;
    riscv_status result = riscv_cfft_radix2_init_f16(&f16_S, CFFTSIZE, ifftFlag, doBitReverse);

    BENCH_START(riscv_cfft_radix2_f16);
    riscv_cfft_radix2_f16(&f16_S, cfft_testinput_f16_50hz_200Hz);
    BENCH_END(riscv_cfft_radix2_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

