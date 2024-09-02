#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/fill_f16/test_data.h"

BENCH_DECLARE_VAR();

void fill_riscv_fill_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_f16);
    riscv_fill_f16(f16_fill_value, f16_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
