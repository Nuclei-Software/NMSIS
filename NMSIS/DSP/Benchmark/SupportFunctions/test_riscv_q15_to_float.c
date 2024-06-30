#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/q15_to_float/test_data.h"

BENCH_DECLARE_VAR();

void q15ToF32_riscv_q15_to_f32(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_q15(q15_input, ARRAY_SIZE);

    BENCH_START(riscv_q15_to_f32);
    riscv_q15_to_float(q15_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_f32);

    return;
}