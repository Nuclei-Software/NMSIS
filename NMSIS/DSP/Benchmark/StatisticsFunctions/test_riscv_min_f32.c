#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/min_f32/test_data.h"

BENCH_DECLARE_VAR();

void min_riscv_min_f32(void)
{
    float32_t f32_output;
    uint32_t index;

    generate_rand_f32(min_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_min_f32);
    riscv_min_f32(min_f32_input, ARRAY_SIZE, &f32_output, &index);
    BENCH_END(riscv_min_f32);
}