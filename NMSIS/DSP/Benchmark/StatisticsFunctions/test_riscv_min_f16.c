#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/min_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void min_riscv_min_f16(void)
{
    float16_t f16_output;
    uint32_t index;

    generate_rand_f16(min_f16_input, ARRAY_SIZE);

    BENCH_START(riscv_min_f16);
    riscv_min_f16(min_f16_input, ARRAY_SIZE, &f16_output, &index);
    BENCH_END(riscv_min_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
