#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/rfft_fast_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void rfft_riscv_rfft_fast_f16(void)
{
    float16_t rfft_fast_f16_output[2 * RFFTSIZE];

    generate_rand_f16(rfft_testinput_f16_50hz_200Hz_fast, RFFTSIZE);
    riscv_rfft_fast_instance_f16 SS;

    riscv_status result = riscv_rfft_fast_init_f16(&SS, RFFTSIZE);
    BENCH_START(riscv_rfft_fast_f16);
    riscv_rfft_fast_f16(&SS, rfft_testinput_f16_50hz_200Hz_fast, rfft_fast_f16_output, ifftFlag);
    BENCH_END(riscv_rfft_fast_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
