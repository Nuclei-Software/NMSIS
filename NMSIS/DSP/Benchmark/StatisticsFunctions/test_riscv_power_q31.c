#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/power_q31/test_data.h"

BENCH_DECLARE_VAR();

void power_riscv_power_q31(void)
{
    q63_t q63_output;

    generate_rand_q31(power_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_power_q31);
    riscv_power_q31(power_q31_input, ARRAY_SIZE, &q63_output);
    BENCH_END(riscv_power_q31);

    return;
}
