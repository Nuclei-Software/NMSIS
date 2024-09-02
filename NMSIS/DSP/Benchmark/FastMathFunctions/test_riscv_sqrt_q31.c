#include "riscv_math.h"
#include <stdio.h>
#include <stdlib.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/sqrt_q31/test_data.h"

BENCH_DECLARE_VAR();

void sqrt_riscv_sqrt_q31(void)
{
    q31_t sqrt_q31_output[ARRAY_SIZE_Q31];

    do_srand();
    for (int i = 0; i < ARRAY_SIZE_Q31; i++) {
        sqrt_q31_input[i] = (q31_t)(rand() % Q31_MAX - Q31_MAX / 2);
        sqrt_q31_input[i] = sqrt_q31_input[i] > 0 ? sqrt_q31_input[i] : 100;
    }

    riscv_status result;
    BENCH_START(riscv_sqrt_q31);
    for (int i = 0; i < ARRAY_SIZE_Q31; i++) {
        result = riscv_sqrt_q31(sqrt_q31_input[i], &sqrt_q31_output[i]);
    }
    BENCH_END(riscv_sqrt_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
