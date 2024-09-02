#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/xor_u32/test_data.h"

BENCH_DECLARE_VAR();

void xor_riscv_xor_u32(void)
{
    uint32_t xor_u32_output[ARRAY_SIZE_U32];
    q31_t xor_in1[ARRAY_SIZE_U32];
    q31_t xor_in2[ARRAY_SIZE_U32];

    generate_rand_q31(xor_in1, ARRAY_SIZE_U32);
    generate_rand_q31(xor_in2, ARRAY_SIZE_U32);

    BENCH_START(riscv_xor_u32);
    riscv_xor_u32((uint32_t *)xor_in1, (uint32_t *)xor_in2, xor_u32_output, ARRAY_SIZE_U32);
    BENCH_END(riscv_xor_u32);
}
