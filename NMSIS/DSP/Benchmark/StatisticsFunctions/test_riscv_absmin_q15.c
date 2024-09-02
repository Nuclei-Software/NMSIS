#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmin_q15/test_data.h"

BENCH_DECLARE_VAR();

void absmin_riscv_absmin_q15(void)
{
    q15_t q15_output;
    uint32_t index;

    generate_rand_q15(absmin_q15_input, ARRAY_SIZE);

    BENCH_START(riscv_absmin_q15);
    riscv_absmin_q15(absmin_q15_input, ARRAY_SIZE, &q15_output, &index);
    BENCH_END(riscv_absmin_q15);

}