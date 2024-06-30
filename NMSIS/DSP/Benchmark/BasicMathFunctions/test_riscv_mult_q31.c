#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/mult_q31/test_data.h"

BENCH_DECLARE_VAR();

void mul_riscv_mul_q31(void)
{
    q31_t mult_q31_output[ARRAY_SIZE_Q31];
    generate_rand_q31(mult_q31_input1, ARRAY_SIZE_Q31);
    generate_rand_q31(mult_q31_input2, ARRAY_SIZE_Q31);

    // q31_mult
    BENCH_START(riscv_mult_q31);
    riscv_mult_q31(mult_q31_input1, mult_q31_input2, mult_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_mult_q31);
}