#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/weighted_sum_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void weightedSum_riscv_weighted_sum_f16(void)
{
    float16_t f16_weighted_output;

    generate_rand_f16(f16_weighted_sum_array, WEIGHT_NUM);
    for (int i = 0; i < WEIGHT_NUM; i++) {
        f16_weighted_array[i] = (float16_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_weighted_sum_f16);
    f16_weighted_output = riscv_weighted_sum_f16(f16_weighted_sum_array, f16_weighted_array, WEIGHT_NUM);
    BENCH_END(riscv_weighted_sum_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
