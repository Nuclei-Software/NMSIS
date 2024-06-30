#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/logsumexp_f32/test_data.h"

BENCH_DECLARE_VAR();

void logsumexp_riscv_logsumexp_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(logsumexp_f32_input, ARRAY_SIZE);

    BENCH_START(riscv_logsumexp_f32);
    f32_output = riscv_logsumexp_f32(logsumexp_f32_input, ARRAY_SIZE);
    BENCH_END(riscv_logsumexp_f32);

    return;
}
