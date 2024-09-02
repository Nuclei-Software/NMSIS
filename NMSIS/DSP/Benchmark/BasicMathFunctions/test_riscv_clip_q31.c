#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/clip_q31/test_data.h"

BENCH_DECLARE_VAR();

void clip_riscv_clip_q31(void)
{
    q31_t low_q31 = LOWER_BOUND_Q31;
    q31_t high_q31 = HIGHER_BOUND_Q31;
    q31_t clip_q31_output[ARRAY_SIZE_Q31];

    generate_rand_q31(clip_q31_input, ARRAY_SIZE_Q31);

    BENCH_START(riscv_clip_q31);
    riscv_clip_q31(clip_q31_input, clip_q31_output, low_q31, high_q31, ARRAY_SIZE_Q31);
    BENCH_END(riscv_clip_q31);
}