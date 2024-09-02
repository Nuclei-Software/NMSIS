#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/entropy_f64/test_data.h"

BENCH_DECLARE_VAR();

void entropy_riscv_entropy_f64(void)
{
#ifdef F64
    float64_t f64_output;

    generate_rand_f64(entropy_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_entropy_f64);
    f64_output = riscv_entropy_f64(entropy_f64_input, ARRAY_SIZE);
    BENCH_END(riscv_entropy_f64);
#endif
}