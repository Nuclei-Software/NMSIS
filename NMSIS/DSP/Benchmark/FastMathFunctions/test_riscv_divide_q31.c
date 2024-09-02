#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/divide_q31/test_data.h"

BENCH_DECLARE_VAR();

void divide_riscv_divide_q31(void)
{

    q31_t divide_q31_output[ARRAY_SIZE_Q31];
    int16_t shift_i16[ARRAY_SIZE_Q31];

    generate_rand_q31(divide_q31_input1, ARRAY_SIZE_Q31);
    generate_rand_q31(divide_q31_input2, ARRAY_SIZE_Q31);

    riscv_status result;
    BENCH_START(riscv_divide_q31);
    for (int i = 0; i < ARRAY_SIZE_Q31; i++) {
        result = riscv_divide_q31(divide_q31_input1[i], divide_q31_input2[i], &divide_q31_output[i], &shift_i16[i]);
    }
    BENCH_END(riscv_divide_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
