#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/fill_f32/test_data.h"

BENCH_DECLARE_VAR();

void fill_riscv_fill_f32(void)
{
    float32_t f32_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_f32);
    riscv_fill_f32(f32_fill_value, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_f32);
}