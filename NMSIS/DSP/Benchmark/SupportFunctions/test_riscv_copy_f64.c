#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/copy_f64/test_data.h"

BENCH_DECLARE_VAR();

void copy_riscv_copy_f64(void)
{
#ifdef F64
    float64_t f64_output[ARRAY_SIZE];

    generate_rand_f64(copy_f64_input, ARRAY_SIZE);

    BENCH_START(riscv_copy_f64);
    riscv_copy_f64(copy_f64_input, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_copy_f64);
#endif
}