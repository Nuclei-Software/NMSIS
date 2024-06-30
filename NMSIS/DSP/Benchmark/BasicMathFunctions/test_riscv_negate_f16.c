#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/negate_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void negate_riscv_negate_f16(void)
{
    float16_t negate_f16_output[ARRAY_SIZE_F16];
    generate_rand_f16(negate_f16_input, ARRAY_SIZE_F16);

    // f16_negate
    BENCH_START(riscv_negate_f16);
    riscv_negate_f16(negate_f16_input, negate_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_negate_f16);
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */