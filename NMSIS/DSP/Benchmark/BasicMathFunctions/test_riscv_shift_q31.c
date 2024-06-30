#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/shift_q31/test_data.h"

BENCH_DECLARE_VAR();

void shift_riscv_shift_q31(void)
{
    q31_t shift_q31_output[ARRAY_SIZE_Q31];
    int8_t shiftBits_q31 = -3;
    generate_rand_q31(shift_q31_input, ARRAY_SIZE_Q31);

    BENCH_START(riscv_shift_q31);
    riscv_shift_q31(shift_q31_input, shiftBits_q31, shift_q31_output, ARRAY_SIZE_Q31);
    BENCH_END(riscv_shift_q31);
}
