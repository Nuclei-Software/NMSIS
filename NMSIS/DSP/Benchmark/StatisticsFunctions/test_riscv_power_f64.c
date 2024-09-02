#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/power_f64/test_data.h"

BENCH_DECLARE_VAR();

void power_riscv_power_f64(void)
{
#ifdef F64
    float64_t f64_output;

    generate_rand_f64(power_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_power_f64);
    riscv_power_f64(power_f64_input, ARRAY_SIZE, &f64_output);
    BENCH_END(riscv_power_f64);
#endif
}
