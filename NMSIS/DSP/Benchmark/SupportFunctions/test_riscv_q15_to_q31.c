#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/q15_to_q31/test_data.h"

BENCH_DECLARE_VAR();

void q15ToQ31_riscv_q15_to_q31(void)
{
    q31_t q31_output[ARRAY_SIZE];

    generate_rand_q15(q15_input, ARRAY_SIZE);

    BENCH_START(riscv_q15_to_q31);
    riscv_q15_to_q31(q15_input, q31_output, ARRAY_SIZE);
    BENCH_END(riscv_q15_to_q31);
}