#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/absmax_f16/test_data.h"

BENCH_DECLARE_VAR();

void absmax_riscv_absmax_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output;
    uint32_t index;

    generate_rand_f16(absmax_f16_input, ARRAY_SIZE);

    BENCH_START(riscv_absmax_f16);
    riscv_absmax_f16(absmax_f16_input, ARRAY_SIZE, &f16_output, &index);
    BENCH_END(riscv_absmax_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

