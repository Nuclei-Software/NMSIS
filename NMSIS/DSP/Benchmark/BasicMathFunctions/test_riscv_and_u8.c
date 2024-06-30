#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/and_u8/test_data.h"

BENCH_DECLARE_VAR();

void and_riscv_and_u8(void)
{
    uint8_t and_u8_output[ARRAY_SIZE_U8];
    q7_t and_in1[ARRAY_SIZE_U8];
    q7_t and_in2[ARRAY_SIZE_U8];

    generate_rand_q7(and_in1, ARRAY_SIZE_U8);
    generate_rand_q7(and_in2, ARRAY_SIZE_U8);

    BENCH_START(riscv_and_u8);
    riscv_and_u8((uint8_t *)and_in1, (uint8_t *)and_in2, and_u8_output, ARRAY_SIZE_U8);
    BENCH_END(riscv_and_u8);
}
