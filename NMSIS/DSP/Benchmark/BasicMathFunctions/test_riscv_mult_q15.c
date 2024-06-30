#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/mult_q15/test_data.h"

BENCH_DECLARE_VAR();

void mul_riscv_mul_q15(void)
{
    q15_t mult_q15_output[ARRAY_SIZE_Q15];
    generate_rand_q15(mult_q15_input1, ARRAY_SIZE_Q15);
    generate_rand_q15(mult_q15_input2, ARRAY_SIZE_Q15);

    // q15_mult
    BENCH_START(riscv_mult_q15);
    riscv_mult_q15(mult_q15_input1, mult_q15_input2, mult_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_mult_q15);
}