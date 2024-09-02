#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/and_u32/test_data.h"

BENCH_DECLARE_VAR();

void and_riscv_and_u32(void)
{
    uint32_t and_u32_output[ARRAY_SIZE_U32];
    q31_t and_in1[ARRAY_SIZE_U32];
    q31_t and_in2[ARRAY_SIZE_U32];

    generate_rand_q31(and_in1, ARRAY_SIZE_U32);
    generate_rand_q31(and_in2, ARRAY_SIZE_U32);

    BENCH_START(riscv_and_u32);
    riscv_and_u32((uint32_t *)and_in1, (uint32_t *)and_in2, and_u32_output, ARRAY_SIZE_U32);
    BENCH_END(riscv_and_u32);
}
