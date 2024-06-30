#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/copy_f32/test_data.h"

BENCH_DECLARE_VAR();

void copy_riscv_copy_f32(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_f32(copy_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_copy_f32);
    riscv_copy_f32(copy_f32_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_copy_f32);

    return;
}