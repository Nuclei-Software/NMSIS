#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/shift_q15/test_data.h"

BENCH_DECLARE_VAR();

void shift_riscv_shift_q15(void)
{
    q15_t shift_q15_output[ARRAY_SIZE_Q15];
    int8_t shiftBits_q15 = -3;
    generate_rand_q15(shift_q15_input, ARRAY_SIZE_Q15);

    BENCH_START(riscv_shift_q15);
    riscv_shift_q15(shift_q15_input, shiftBits_q15, shift_q15_output, ARRAY_SIZE_Q15);
    BENCH_END(riscv_shift_q15);
}
