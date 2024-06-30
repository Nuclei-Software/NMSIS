#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/atan2_q15/test_data.h"

BENCH_DECLARE_VAR();

void cos_riscv_atan2_q15(void)
{

    q15_t atan2_q15_output[ARRAY_SIZE_Q15];

    generate_rand_q15(atan2_q15_input1, ARRAY_SIZE_Q15);
    generate_rand_q15(atan2_q15_input2, ARRAY_SIZE_Q15);

    riscv_status result;
    BENCH_START(riscv_atan2_q15);
    for (int i = 0; i < ARRAY_SIZE_Q15; i++) {
        result = riscv_atan2_q15(atan2_q15_input1[i], atan2_q15_input2[i], &atan2_q15_output[i]);
    }
    BENCH_END(riscv_atan2_q15);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
