#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/q7_to_f64/test_data.h"

BENCH_DECLARE_VAR();

void q7ToF64_riscv_q7_to_f64(void)
{
#ifdef F64
    float64_t f64_output[ARRAY_SIZE];

    generate_rand_q7(q7_input, ARRAY_SIZE);

    BENCH_START(riscv_q7_to_f64);
    riscv_q7_to_f64(q7_input, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_q7_to_f64);
#endif
}