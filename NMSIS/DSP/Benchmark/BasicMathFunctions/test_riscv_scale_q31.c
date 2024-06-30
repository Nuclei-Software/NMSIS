#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/scale_q31/test_data.h"

BENCH_DECLARE_VAR();

void scale_riscv_scale_q31(void)
{
    q31_t scale_q31_output[ARRAY_SIZE_Q31];
    generate_rand_q31(scale_q31_input, ARRAY_SIZE_Q31);
    BENCH_START(riscv_scale_q31);
    riscv_scale_q31(scale_q31_input, scaleFract_q31, shift_q31, scale_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_scale_q31);
}
