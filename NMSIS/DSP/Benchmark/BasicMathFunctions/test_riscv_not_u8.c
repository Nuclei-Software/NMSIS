#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/not_u8/test_data.h"

BENCH_DECLARE_VAR();

void not_riscv_not_u8(void)
{
    uint8_t not_u8_output[ARRAY_SIZE_U8];
    q7_t not_in[ARRAY_SIZE_U8];

    generate_rand_q7(not_in, ARRAY_SIZE_U8);

    BENCH_START(riscv_not_u8);
    riscv_not_u8((uint8_t *)not_in, not_u8_output, ARRAY_SIZE_U8);
    BENCH_END(riscv_not_u8);
}
