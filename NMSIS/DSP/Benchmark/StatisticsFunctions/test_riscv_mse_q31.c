#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/mse_q31/test_data.h"

BENCH_DECLARE_VAR();

void mse_riscv_mse_q31(void)
{
    q31_t q31_output;

    generate_rand_q31(mse_q31_input1, ARRAY_SIZE);
    generate_rand_q31(mse_q31_input2, ARRAY_SIZE);

    BENCH_START(riscv_mse_q31);
    riscv_mse_q31(mse_q31_input1, mse_q31_input2, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_mse_q31);

    return;
}