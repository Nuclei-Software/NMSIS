#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmin_no_idx_f64/test_data.h"

BENCH_DECLARE_VAR();

void absminNoIdx_riscv_absmin_no_idx_f64(void)
{
#ifdef F64
    float64_t f64_output;

    generate_rand_f64(absmin_no_idx_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_absmin_no_idx_f64);
    riscv_absmin_no_idx_f64(absmin_no_idx_f64_input, ARRAY_SIZE, &f64_output);
    BENCH_END(riscv_absmin_no_idx_f64);
#endif
}
