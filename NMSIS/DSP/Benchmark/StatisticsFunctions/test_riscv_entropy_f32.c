#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/entropy_f32/test_data.h"

BENCH_DECLARE_VAR();

void entropy_riscv_entropy_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(entropy_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_entropy_f32);
    f32_output = riscv_entropy_f32(entropy_f32_input, ARRAY_SIZE);
    BENCH_END(riscv_entropy_f32);
}