#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/fill_f64/test_data.h"

BENCH_DECLARE_VAR();

void fill_riscv_fill_f64(void)
{
    float64_t f64_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_f64);
    riscv_fill_f64(f64_fill_value, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_f64);

    return;
}