#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmin_no_idx_q31/test_data.h"

BENCH_DECLARE_VAR();

void absminNoIdx_riscv_absmin_no_idx_q31(void)
{
    q31_t q31_output;

    generate_rand_q31(absmin_no_idx_q31_input, ARRAY_SIZE);

    BENCH_START(riscv_absmin_no_idx_q31);
    riscv_absmin_no_idx_q31(absmin_no_idx_q31_input, ARRAY_SIZE, &q31_output);
    BENCH_END(riscv_absmin_no_idx_q31);
}