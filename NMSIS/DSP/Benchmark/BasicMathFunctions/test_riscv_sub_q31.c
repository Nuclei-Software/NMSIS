#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/sub_q31/test_data.h"

BENCH_DECLARE_VAR();

void sub_riscv_sub_q31(void)
{
    q31_t sub_q31_output[ARRAY_SIZE_Q31];

    generate_rand_q31(sub_q31_input1, ARRAY_SIZE_Q31);
    generate_rand_q31(sub_q31_input2, ARRAY_SIZE_Q31);

    BENCH_START(riscv_sub_q31);
    riscv_sub_q31(sub_q31_input1, sub_q31_input2, sub_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_sub_q31);
}
