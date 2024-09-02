#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/or_u32/test_data.h"

BENCH_DECLARE_VAR();

void or_riscv_or_u32(void)
{
    uint32_t or_u32_output[ARRAY_SIZE_U32];
    q31_t or_in1[ARRAY_SIZE_U32];
    q31_t or_in2[ARRAY_SIZE_U32];

    generate_rand_q31(or_in1, ARRAY_SIZE_U32);
    generate_rand_q31(or_in2, ARRAY_SIZE_U32);

    BENCH_START(riscv_or_u32);
    riscv_or_u32((uint32_t *)or_in1, (uint32_t *)or_in2, or_u32_output, ARRAY_SIZE_U32);
    BENCH_END(riscv_or_u32);
}
