#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/mse_q7/test_data.h"

BENCH_DECLARE_VAR();

void mse_riscv_mse_q7(void)
{
    q7_t q7_output;

    generate_rand_q7(mse_q7_input1, ARRAY_SIZE);
    generate_rand_q7(mse_q7_input2, ARRAY_SIZE);

    BENCH_START(riscv_mse_q7);
    riscv_mse_q7(mse_q7_input1, mse_q7_input2, ARRAY_SIZE, &q7_output);
    BENCH_END(riscv_mse_q7);

    return;
}