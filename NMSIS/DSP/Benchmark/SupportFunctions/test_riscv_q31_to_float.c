#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/q31_to_float/test_data.h"

BENCH_DECLARE_VAR();

void q31ToF32_riscv_q31_to_f32(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_q31(q31_input, ARRAY_SIZE);

    BENCH_START(riscv_q31_to_float);
    riscv_q31_to_float(q31_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_float);

    return;
}