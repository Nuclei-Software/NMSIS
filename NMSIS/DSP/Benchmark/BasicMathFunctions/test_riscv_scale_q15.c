#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/scale_q15/test_data.h"

BENCH_DECLARE_VAR();

void scale_riscv_scale_q15(void)
{
    q15_t scale_q15_output[ARRAY_SIZE_Q15];
    generate_rand_q15(scale_q15_input, ARRAY_SIZE_Q15);
    BENCH_START(riscv_scale_q15);
    riscv_scale_q15(scale_q15_input, scaleFract_q15, shift_q15, scale_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_scale_q15);
}
