#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/mse_f16/test_data.h"

BENCH_DECLARE_VAR();

void mse_riscv_mse_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output;

    generate_rand_f16(mse_f16_input1, ARRAY_SIZE);
    generate_rand_f16(mse_f16_input2, ARRAY_SIZE);

    BENCH_START(riscv_mse_f16);
    riscv_mse_f16(mse_f16_input1, mse_f16_input2, ARRAY_SIZE, &f16_output);
    BENCH_END(riscv_mse_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
