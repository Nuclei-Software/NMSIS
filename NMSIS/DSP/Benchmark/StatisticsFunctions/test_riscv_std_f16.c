#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/std_f16/test_data.h"

BENCH_DECLARE_VAR();

void std_riscv_std_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output;

    generate_rand_f16(std_f16_input, ARRAY_SIZE);

    BENCH_START(riscv_std_f16);
    riscv_std_f16(std_f16_input, ARRAY_SIZE, &f16_output);
    BENCH_END(riscv_std_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
