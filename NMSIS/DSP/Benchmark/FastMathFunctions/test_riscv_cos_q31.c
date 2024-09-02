#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/cos_q31/test_data.h"

BENCH_DECLARE_VAR();

void cos_riscv_cos_q31(void)
{
    q31_t cos_q31_output[ARRAY_SIZE_Q31];
    
    generate_rand_q31(cos_q31_input, ARRAY_SIZE_Q31);

    BENCH_START(riscv_cos_q31);
    for (int i = 0; i < ARRAY_SIZE_Q31; i++) {
        cos_q31_output[i] = riscv_cos_q31(cos_q31_input[i]);
    }
    BENCH_END(riscv_cos_q31);

    return;
}