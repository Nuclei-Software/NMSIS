#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/mse_f64/test_data.h"

BENCH_DECLARE_VAR();

void mse_riscv_mse_f64(void)
{
    float64_t f64_output;

    generate_rand_f64(mse_f64_input1, ARRAY_SIZE);
    generate_rand_f64(mse_f64_input2, ARRAY_SIZE);

    BENCH_START(riscv_mse_f64);
    riscv_mse_f64(mse_f64_input1, mse_f64_input2, ARRAY_SIZE, &f64_output);
    BENCH_END(riscv_mse_f64);

    return;
}