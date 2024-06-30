#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/float_to_q7/test_data.h"

BENCH_DECLARE_VAR();

void f32ToQ7_riscv_float_to_q7(void)
{
    q7_t q7_output[ARRAY_SIZE];

    generate_rand_f32(f32_input, ARRAY_SIZE);

    BENCH_START(riscv_float_to_q7);
    riscv_float_to_q7(f32_input, q7_output, ARRAY_SIZE);
    BENCH_END(riscv_float_to_q7);

    return;
}