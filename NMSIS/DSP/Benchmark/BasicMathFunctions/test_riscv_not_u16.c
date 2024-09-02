#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/not_u16/test_data.h"

BENCH_DECLARE_VAR();

void not_riscv_not_u16(void)
{
    uint16_t not_u16_output[ARRAY_SIZE_U16];
    q15_t not_in[ARRAY_SIZE_U16];

    generate_rand_q15(not_in, ARRAY_SIZE_U16);

    BENCH_START(riscv_not_u16);
    riscv_not_u16((uint16_t *)not_in, not_u16_output, ARRAY_SIZE_U16);
    BENCH_END(riscv_not_u16);
}
