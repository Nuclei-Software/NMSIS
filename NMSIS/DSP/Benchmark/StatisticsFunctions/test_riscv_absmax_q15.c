#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmax_q15/test_data.h"

BENCH_DECLARE_VAR();

void absmax_riscv_absmax_q15(void)
{
    q15_t q15_output;
    uint32_t index;

    generate_rand_q15(absmax_q15_input, ARRAY_SIZE);

    BENCH_START(riscv_absmax_q15);
    riscv_absmax_q15(absmax_q15_input, ARRAY_SIZE, &q15_output, &index);
    BENCH_END(riscv_absmax_q15);

}