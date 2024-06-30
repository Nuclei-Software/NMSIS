#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/cfft_radix4_q31/test_data.h"

BENCH_DECLARE_VAR();

void cfft_riscv_cfft_radix4_q31(void)
{
    generate_rand_q31(cfft_testinput_q31_50hz_200Hz, CFFTx4SIZE * 2);

    riscv_cfft_radix4_instance_q31 q31_S;
    riscv_status result = riscv_cfft_radix4_init_q31(&q31_S, CFFTx4SIZE, ifftFlag, doBitReverse);

    BENCH_START(riscv_cfft_radix4_q31);
    riscv_cfft_radix4_q31(&q31_S, cfft_testinput_q31_50hz_200Hz);
    BENCH_END(riscv_cfft_radix4_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}