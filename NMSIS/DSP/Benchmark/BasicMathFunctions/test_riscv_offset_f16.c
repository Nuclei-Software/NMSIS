#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/offset_f16/test_data.h"

BENCH_DECLARE_VAR();

void offset_riscv_offset_f16(void)
{
#if defined(RISCV_FLOAT16_SUPPORTED)
    float16_t offset_f16_output[ARRAY_SIZE_F16];
    float16_t offset_f16;

    generate_rand_f16(offset_f16_input, ARRAY_SIZE_F16);

    // f16_offset
    generate_rand_f16(&offset_f16, 1);
    BENCH_START(riscv_offset_f16);
    riscv_offset_f16(offset_f16_input, offset_f16, offset_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_offset_f16);
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
}
