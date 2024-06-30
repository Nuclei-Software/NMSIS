#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/f64_to_q31/test_data.h"

BENCH_DECLARE_VAR();

void f64Toq31_riscv_f64_to_q31(void)
{
    q31_t q31_output[ARRAY_SIZE];

    generate_rand_f64(f64_input, ARRAY_SIZE);

    BENCH_START(riscv_f64_to_q31);
    riscv_f64_to_q31(f64_input, q31_output, ARRAY_SIZE);
    BENCH_END(riscv_f64_to_q31);

    return;
}