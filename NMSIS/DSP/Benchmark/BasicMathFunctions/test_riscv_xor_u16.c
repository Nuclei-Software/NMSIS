#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/xor_u16/test_data.h"

BENCH_DECLARE_VAR();

void xor_riscv_xor_u16(void)
{
    uint16_t xor_u16_output[ARRAY_SIZE_U16];
    q15_t xor_in1[ARRAY_SIZE_U16];
    q15_t xor_in2[ARRAY_SIZE_U16];

    generate_rand_q15(xor_in1, ARRAY_SIZE_U16);
    generate_rand_q15(xor_in2, ARRAY_SIZE_U16);

    BENCH_START(riscv_xor_u16);
    riscv_xor_u16((uint16_t *)xor_in1, (uint16_t *)xor_in2, xor_u16_output, ARRAY_SIZE_U16);
    BENCH_END(riscv_xor_u16);
}
