#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/mse_f32/test_data.h"

BENCH_DECLARE_VAR();

void mse_riscv_mse_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(mse_f32_input1, ARRAY_SIZE);
    generate_rand_f32(mse_f32_input2, ARRAY_SIZE);

    BENCH_START(riscv_mse_f32);
    riscv_mse_f32(mse_f32_input1, mse_f32_input2, ARRAY_SIZE, &f32_output);
    BENCH_END(riscv_mse_f32);
}