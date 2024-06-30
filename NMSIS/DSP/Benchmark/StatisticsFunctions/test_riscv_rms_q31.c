#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/rms_q31/test_data.h"

BENCH_DECLARE_VAR();

void rms_riscv_rms_q31(void)
{
    q31_t q31_output;

    generate_rand_q31(rms_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_rms_q31);
    riscv_rms_q31(rms_q31_input, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_rms_q31);

    return;
}