#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/absmax_f64/test_data.h"

BENCH_DECLARE_VAR();

void absmax_riscv_absmax_f64(void)
{
    float64_t f64_output;
    uint32_t index;

    generate_rand_f64(absmax_f64_input, ARRAY_SIZE);
    BENCH_START(riscv_absmax_f64);
    riscv_absmax_f64(absmax_f64_input, ARRAY_SIZE, &f64_output, &index);
    BENCH_END(riscv_absmax_f64);

    return;
}
