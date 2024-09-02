#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/negate_q31/test_data.h"

BENCH_DECLARE_VAR();

void negate_riscv_negate_q31(void)
{
    q31_t negate_q31_output[ARRAY_SIZE_Q31];

    generate_rand_q31(negate_q31_input, ARRAY_SIZE_Q31);

    // q31_mult
    BENCH_START(riscv_negate_q31);
    riscv_negate_q31(negate_q31_input, negate_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_negate_q31);
}