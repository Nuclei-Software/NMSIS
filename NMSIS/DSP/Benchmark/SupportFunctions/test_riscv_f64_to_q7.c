#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/f64_to_q7/test_data.h"

BENCH_DECLARE_VAR();

void f64Toq7_riscv_f64_to_q7(void)
{
#ifdef F64
    q7_t q7_output[ARRAY_SIZE];

    generate_rand_f64(f64_input, ARRAY_SIZE);

    BENCH_START(riscv_f64_to_q7);
    riscv_f64_to_q7(f64_input, q7_output, ARRAY_SIZE);
    BENCH_END(riscv_f64_to_q7);
#endif
}