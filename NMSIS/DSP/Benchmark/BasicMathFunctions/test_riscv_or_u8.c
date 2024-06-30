#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/or_u8/test_data.h"

BENCH_DECLARE_VAR();

void or_riscv_or_u8(void)
{
    uint8_t or_u8_output[ARRAY_SIZE_U8];
    q7_t or_in1[ARRAY_SIZE_U8];
    q7_t or_in2[ARRAY_SIZE_U8];

    generate_rand_q7(or_in1, ARRAY_SIZE_U8);
    generate_rand_q7(or_in2, ARRAY_SIZE_U8);

    BENCH_START(riscv_or_u8);
    riscv_or_u8((uint8_t *)or_in1, (uint8_t *)or_in2, or_u8_output, ARRAY_SIZE_U8);
    BENCH_END(riscv_or_u8);
}
