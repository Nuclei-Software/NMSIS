#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/scale_q7/test_data.h"

BENCH_DECLARE_VAR();

void scale_riscv_scale_q7(void)
{
    q7_t scale_q7_output[ARRAY_SIZE_Q7];
    generate_rand_q7(scale_q7_input, ARRAY_SIZE_Q7);
    BENCH_START(riscv_scale_q7);
    riscv_scale_q7(scale_q7_input, scaleFract_q7, shift_q7, scale_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_scale_q7);
}
