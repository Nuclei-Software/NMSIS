#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/sin_q15/test_data.h"

BENCH_DECLARE_VAR();

void sin_riscv_sin_q15(void)
{
    
    float32_t sin_q15_output[ARRAY_SIZE_Q15];
    
    generate_rand_q15(sin_q15_input, ARRAY_SIZE_Q15);

    BENCH_START(riscv_sin_q15);
    for (int i = 0; i < ARRAY_SIZE_Q15; i++) {
        sin_q15_output[i] = riscv_sin_q15(sin_q15_input[i]);
    }
    BENCH_END(riscv_sin_q15);

    return;
}