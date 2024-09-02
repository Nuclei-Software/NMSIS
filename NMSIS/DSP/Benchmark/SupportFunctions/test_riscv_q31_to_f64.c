#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/q31_to_f64/test_data.h"

BENCH_DECLARE_VAR();

void q31ToF64_riscv_q31_to_f64(void)
{
#ifdef F64
    float64_t f64_output[ARRAY_SIZE];

    generate_rand_q31(q31_input, ARRAY_SIZE);

    BENCH_START(riscv_q31_to_f64);
    riscv_q31_to_f64(q31_input, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_q31_to_f64);
#endif
}