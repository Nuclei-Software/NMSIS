#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmax_q31/test_data.h"

BENCH_DECLARE_VAR();

void absmax_riscv_absmax_q31(void)
{
    q31_t q31_output;
    uint32_t index;

    generate_rand_q31(absmax_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_absmax_q31);
    riscv_absmax_q31(absmax_q31_input, ARRAY_SIZE, &q31_output, &index);
    BENCH_END(riscv_absmax_q31);
}