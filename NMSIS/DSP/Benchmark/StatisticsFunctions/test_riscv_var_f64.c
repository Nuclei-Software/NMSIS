#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/var_f64/test_data.h"

BENCH_DECLARE_VAR();

void var_riscv_var_f64(void)
{
    float64_t f64_output;

    generate_rand_f64(var_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_var_f64);
    riscv_var_f64(var_f64_input, ARRAY_SIZE, &f64_output);
    BENCH_END(riscv_var_f64);

    return;
}