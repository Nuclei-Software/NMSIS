#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/kullback_leibler_f64/test_data.h"

BENCH_DECLARE_VAR();

void kullbackLeibler_riscv_kullback_leibler_f64(void)
{
    float64_t f64_output;

    generate_rand_f64(kullback_leibler_f64_input1, ARRAY_SIZE);
    generate_rand_f64(kullback_leibler_f64_input2, ARRAY_SIZE);

    BENCH_START(riscv_kullback_leibler_f64);
    f64_output = riscv_kullback_leibler_f64(kullback_leibler_f64_input1, kullback_leibler_f64_input2, ARRAY_SIZE);
    BENCH_END(riscv_kullback_leibler_f64);

    return;
}