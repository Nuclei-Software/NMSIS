#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/cfft_radix4_f32/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_radix4_f32(void)
{
    generate_rand_f32(cfft_testinput_f32_50hz_200Hz, CFFTx4SIZE * 2);

    riscv_cfft_radix4_instance_f32 f32_S;
    riscv_status result = riscv_cfft_radix4_init_f32(&f32_S, CFFTx4SIZE, ifftFlag, doBitReverse);

    BENCH_START(riscv_cfft_radix4_f32);
    riscv_cfft_radix4_f32(&f32_S, cfft_testinput_f32_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}