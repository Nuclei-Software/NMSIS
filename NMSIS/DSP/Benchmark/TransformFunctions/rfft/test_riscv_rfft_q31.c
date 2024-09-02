#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "TestData/TransformFunctions/rfft_q31/test_data.h"

BENCH_DECLARE_VAR();

void rfft_riscv_rfft_q31(void)
{
    q31_t rfft_q31_output[2 * RFFTSIZE];
    riscv_rfft_instance_q31 SS;
    generate_rand_q31(rfft_testinput_q31_50hz_200Hz, RFFTSIZE);

    riscv_status result = riscv_rfft_init_q31(&SS, RFFTSIZE, ifftFlag, doBitReverse);
    BENCH_START(riscv_rfft_q31);
    riscv_rfft_q31(&SS, rfft_testinput_q31_50hz_200Hz, rfft_q31_output);
    BENCH_END(riscv_rfft_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}
