#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/power_q7/test_data.h"

BENCH_DECLARE_VAR();

void power_riscv_power_q7(void)
{
    q31_t q31_output;

    generate_rand_q7(power_q7_input, ARRAY_SIZE);

    BENCH_START(riscv_power_q7);
    riscv_power_q7(power_q7_input, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_power_q7);
}
