#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/StatisticsFunctions/lgsumexp_dot_prod_f32/test_data.h"

BENCH_DECLARE_VAR();

void logsumexpDotProd_riscv_logsumexp_dot_prod_f32(void)
{
    float32_t f32_output;

    generate_rand_f32(logsumexp_dotProd_f32_input1, ARRAY_SIZE);
    generate_rand_f32(logsumexp_dotProd_f32_input2, ARRAY_SIZE);

    BENCH_START(riscv_logsumexp_dot_prod_f32);
    f32_output = riscv_logsumexp_dot_prod_f32(logsumexp_dotProd_f32_input1, logsumexp_dotProd_f32_input2, ARRAY_SIZE, f32_temp_array);
    BENCH_END(riscv_logsumexp_dot_prod_f32);

    return;
}
