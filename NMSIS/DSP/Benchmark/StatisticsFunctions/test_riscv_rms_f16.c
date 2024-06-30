#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/rms_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void rms_riscv_rms_f16(void)
{
    float16_t f16_output;

    generate_rand_f16(rms_f16_input, ARRAY_SIZE);

    BENCH_START(riscv_rms_f16);
    riscv_rms_f16(rms_f16_input, ARRAY_SIZE, &f16_output);
    BENCH_END(riscv_rms_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */