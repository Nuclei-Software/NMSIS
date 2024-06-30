#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/sub_q15/test_data.h"

BENCH_DECLARE_VAR();

void sub_riscv_sub_q15(void)
{
    q15_t sub_q15_output[ARRAY_SIZE_Q15];
    generate_rand_q15(sub_q15_input1, ARRAY_SIZE_Q15);
    generate_rand_q15(sub_q15_input2, ARRAY_SIZE_Q15);
    BENCH_START(riscv_sub_q15);
    riscv_sub_q15(sub_q15_input1, sub_q15_input2, sub_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_sub_q15);
}
