#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/clip_q7/test_data.h"

BENCH_DECLARE_VAR();

void clip_riscv_clip_q7(void)
{
    q7_t low_q7 = LOWER_BOUND_Q7;
    q7_t high_q7 = HIGHER_BOUND_Q7;
    q7_t clip_q7_output[ARRAY_SIZE_Q7];

    generate_rand_q7(clip_q7_input, ARRAY_SIZE_Q7);

    BENCH_START(riscv_clip_q7);
    riscv_clip_q7(clip_q7_input, clip_q7_output, low_q7, high_q7, ARRAY_SIZE_Q7);
    BENCH_END(riscv_clip_q7);
}