#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/mse_q15/test_data.h"

BENCH_DECLARE_VAR();

void mse_riscv_mse_q15(void)
{
    q15_t q15_output;

    generate_rand_q15(mse_q15_input1, ARRAY_SIZE);
    generate_rand_q15(mse_q15_input2, ARRAY_SIZE);

    BENCH_START(riscv_mse_q15);
    riscv_mse_q15(mse_q15_input1, mse_q15_input2, ARRAY_SIZE, &q15_output);
    BENCH_END(riscv_mse_q15);
}