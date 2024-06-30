#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/sub_q7/test_data.h"

BENCH_DECLARE_VAR();

void sub_riscv_sub_q7(void)
{
    q7_t sub_q7_output[ARRAY_SIZE_Q7];
    generate_rand_q7(sub_q7_input1, ARRAY_SIZE_Q7);
    generate_rand_q7(sub_q7_input2, ARRAY_SIZE_Q7);
    BENCH_START(riscv_sub_q7);
    riscv_sub_q7(sub_q7_input1, sub_q7_input2, sub_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_sub_q7);
}
