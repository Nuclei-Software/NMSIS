#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/atan2_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void cos_riscv_atan2_f16(void)
{

    float16_t atan2_f16_output[ARRAY_SIZE_F16];

    generate_rand_f16(atan2_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(atan2_f16_input2, ARRAY_SIZE_F16);

    riscv_status result;
    BENCH_START(riscv_atan2_f16);
    for (int i = 0; i < ARRAY_SIZE_F16; i++) {
        result = riscv_atan2_f16(atan2_f16_input1[i], atan2_f16_input2[i], &atan2_f16_output[i]);
    }
    BENCH_END(riscv_atan2_f16);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
