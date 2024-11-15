#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/lgsumexp_f16/test_data.h"

BENCH_DECLARE_VAR();

void logsumexp_riscv_logsumexp_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output;

    generate_rand_f16(logsumexp_f16_input, ARRAY_SIZE);

    BENCH_START(riscv_logsumexp_f16);
    f16_output = riscv_logsumexp_f16(logsumexp_f16_input, ARRAY_SIZE);
    BENCH_END(riscv_logsumexp_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

