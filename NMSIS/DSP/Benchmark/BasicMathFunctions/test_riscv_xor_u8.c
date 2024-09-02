#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/xor_u8/test_data.h"

BENCH_DECLARE_VAR();

void xor_riscv_xor_u8(void)
{
    uint8_t xor_u8_output[ARRAY_SIZE_U8];
    q7_t xor_in1[ARRAY_SIZE_U8];
    q7_t xor_in2[ARRAY_SIZE_U8];

    generate_rand_q7(xor_in1, ARRAY_SIZE_U8);
    generate_rand_q7(xor_in2, ARRAY_SIZE_U8);

    BENCH_START(riscv_xor_u8);
    riscv_xor_u8((uint8_t *)xor_in1, (uint8_t *)xor_in2, xor_u8_output, ARRAY_SIZE_U8);
    BENCH_END(riscv_xor_u8);
}
