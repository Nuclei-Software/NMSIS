#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/clip_q15/test_data.h"

BENCH_DECLARE_VAR();

void clip_riscv_clip_q15(void)
{
    q15_t low_q15 = LOWER_BOUND_Q15;
    q15_t high_q15 = HIGHER_BOUND_Q15;
    q15_t clip_q15_output[ARRAY_SIZE_Q15];
    generate_rand_q15(clip_q15_input, ARRAY_SIZE_Q15);

    BENCH_START(riscv_clip_q15);
    riscv_clip_q15(clip_q15_input, clip_q15_output, low_q15, high_q15, ARRAY_SIZE_Q15);
    BENCH_END(riscv_clip_q15);
}