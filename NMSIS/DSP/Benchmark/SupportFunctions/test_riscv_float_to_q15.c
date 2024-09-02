#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/float_to_q15/test_data.h"

BENCH_DECLARE_VAR();

void f32ToQ15_riscv_float_to_q15(void)
{
    q15_t q15_output[ARRAY_SIZE];

    generate_rand_f32(f32_input, ARRAY_SIZE);

    BENCH_START(riscv_float_to_q15);
    riscv_float_to_q15(f32_input, q15_output, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q15);
}