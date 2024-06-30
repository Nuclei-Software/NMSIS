#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/kullback_leibler_f32/test_data.h"

BENCH_DECLARE_VAR();

void kullbackLeibler_riscv_kullback_leibler_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(kullback_leibler_f32_input1, ARRAY_SIZE);
    generate_rand_f32(kullback_leibler_f32_input2, ARRAY_SIZE);

    BENCH_START(riscv_kullback_leibler_f32);
    f32_output = riscv_kullback_leibler_f32(kullback_leibler_f32_input1, kullback_leibler_f32_input2, ARRAY_SIZE);
    BENCH_END(riscv_kullback_leibler_f32);

    return;
}