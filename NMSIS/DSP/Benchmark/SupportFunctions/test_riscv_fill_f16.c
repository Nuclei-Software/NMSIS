#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/fill_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void fill_riscv_fill_f16(void)
{
    float16_t f16_output[ARRAY_SIZE];

    BENCH_START(riscv_fill_f16);
    riscv_fill_f16(f16_fill_value, f16_output, ARRAY_SIZE);
    BENCH_END(riscv_fill_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */