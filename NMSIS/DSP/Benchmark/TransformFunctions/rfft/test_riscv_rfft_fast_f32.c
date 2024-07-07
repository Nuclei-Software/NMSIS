#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "riscv_common_tables.h"
#include "riscv_const_structs.h"
#include "../TestData/TransformFunctions/rfft_fast_f32/test_data.h"

BENCH_DECLARE_VAR();

void rfft_riscv_rfft_fast_f32(void)
{
    float32_t rfft_fast_f32_output[2 * RFFTSIZE];

    generate_rand_f32(rfft_testinput_f32_50hz_200Hz_fast, RFFTSIZE);
    riscv_rfft_fast_instance_f32 SS;

    riscv_status result = riscv_rfft_fast_init_f32(&SS, RFFTSIZE);
    BENCH_START(riscv_rfft_fast_f32);
    riscv_rfft_fast_f32(&SS, rfft_testinput_f32_50hz_200Hz_fast, rfft_fast_f32_output, ifftFlag);
    BENCH_END(riscv_rfft_fast_f32);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}