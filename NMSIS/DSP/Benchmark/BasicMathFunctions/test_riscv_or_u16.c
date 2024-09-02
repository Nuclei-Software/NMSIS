#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/or_u16/test_data.h"

BENCH_DECLARE_VAR();

void or_riscv_or_u16(void)
{
    uint16_t or_u16_output[ARRAY_SIZE_U16];
    q15_t or_in1[ARRAY_SIZE_U16];
    q15_t or_in2[ARRAY_SIZE_U16];

    generate_rand_q15(or_in1, ARRAY_SIZE_U16);
    generate_rand_q15(or_in2, ARRAY_SIZE_U16);

    BENCH_START(riscv_or_u16);
    riscv_or_u16((uint16_t *)or_in1, (uint16_t *)or_in2, or_u16_output, ARRAY_SIZE_U16);
    BENCH_END(riscv_or_u16);
}
