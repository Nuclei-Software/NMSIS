#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/cos_q15/test_data.h"

BENCH_DECLARE_VAR();

void cos_riscv_cos_q15(void)
{
    q15_t cos_q15_output[ARRAY_SIZE_Q15];
    
    generate_rand_q15(cos_q15_input, ARRAY_SIZE_Q15);

    BENCH_START(riscv_cos_q15);
    for (int i = 0; i < ARRAY_SIZE_Q15; i++) {
        cos_q15_output[i] = riscv_cos_q15(cos_q15_input[i]);
    }
    BENCH_END(riscv_cos_q15);

    return;
}