#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/float_to_f64/test_data.h"

BENCH_DECLARE_VAR();

void f32ToF64_riscv_float_to_f64(void)
{
    float64_t f64_output[ARRAY_SIZE];

    generate_rand_f32(f32_input, ARRAY_SIZE);

    BENCH_START(riscv_float_to_f64);
    riscv_float_to_f64(f32_input, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_float_to_f64);

    return;
}