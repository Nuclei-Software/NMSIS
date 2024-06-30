#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/q7_to_float/test_data.h"

BENCH_DECLARE_VAR();

void q7ToF32_riscv_q7_to_float(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_q7(q7_input, ARRAY_SIZE);

    BENCH_START(riscv_q7_to_float);
    riscv_q7_to_float(q7_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_float);

    return;
}