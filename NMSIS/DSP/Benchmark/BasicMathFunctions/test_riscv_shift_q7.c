#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/shift_q7/test_data.h"

BENCH_DECLARE_VAR();

void shift_riscv_shift_q7(void)
{
    q7_t shift_q7_output[ARRAY_SIZE_Q7];
    int8_t shiftBits_q7 = -1;
    generate_rand_q7(shift_q7_input, ARRAY_SIZE_Q7);

    BENCH_START(riscv_shift_q7);
    riscv_shift_q7(shift_q7_input, shiftBits_q7, shift_q7_output, ARRAY_SIZE_Q7);
    BENCH_END(riscv_shift_q7);
}
