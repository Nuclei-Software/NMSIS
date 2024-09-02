#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/and_u16/test_data.h"

BENCH_DECLARE_VAR();

void and_riscv_and_u16(void)
{
    uint16_t and_u16_output[ARRAY_SIZE_U16];
    q15_t and_in1[ARRAY_SIZE_U16];
    q15_t and_in2[ARRAY_SIZE_U16];

    generate_rand_q15(and_in1, ARRAY_SIZE_U16);
    generate_rand_q15(and_in2, ARRAY_SIZE_U16);

    BENCH_START(riscv_and_u16);
    riscv_and_u16((uint16_t *)and_in1, (uint16_t *)and_in2, and_u16_output, ARRAY_SIZE_U16);
    BENCH_END(riscv_and_u16);
}
