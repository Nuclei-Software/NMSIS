#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/q15_to_f64/test_data.h"

BENCH_DECLARE_VAR();

void q15ToF64_riscv_q15_to_f64(void)
{
    float64_t f64_output[ARRAY_SIZE];

    generate_rand_q15(q15_input, ARRAY_SIZE);

    BENCH_START(riscv_q15_to_f64);
    riscv_q15_to_f64(q15_input, f64_output, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_f64);

    return;
}