#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/not_u32/test_data.h"

BENCH_DECLARE_VAR();

void not_riscv_not_u32(void)
{
    uint32_t not_u32_output[ARRAY_SIZE_U32];
    q31_t not_in[ARRAY_SIZE_U32];

    generate_rand_q31(not_in, ARRAY_SIZE_U32);

    BENCH_START(riscv_not_u32);
    riscv_not_u32((uint32_t *)not_in, not_u32_output, ARRAY_SIZE_U32);
    BENCH_END(riscv_not_u32);
}
