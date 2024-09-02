#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmin_f64/test_data.h"

BENCH_DECLARE_VAR();

void absmin_riscv_absmin_f64(void)
{
#ifdef F64
    float64_t f64_output;
    uint32_t index;

    generate_rand_f64(absmin_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_absmin_f64);
    riscv_absmin_f64(absmin_f64_input, ARRAY_SIZE, &f64_output, &index);
    BENCH_END(riscv_absmin_f64);
#endif
}
