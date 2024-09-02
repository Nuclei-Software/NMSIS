#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmin_f32/test_data.h"

BENCH_DECLARE_VAR();

void absmin_riscv_absmin_f32(void)
{
    float32_t f32_output;
    uint32_t index;

    generate_rand_f32(absmin_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_absmin_f32);
    riscv_absmin_f32(absmin_f32_input, ARRAY_SIZE, &f32_output, &index);
    BENCH_END(riscv_absmin_f32);

    return;
}