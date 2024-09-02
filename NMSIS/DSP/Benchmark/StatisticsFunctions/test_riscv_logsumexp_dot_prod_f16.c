#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/StatisticsFunctions/lgsumexp_dot_prod_f16/test_data.h"

BENCH_DECLARE_VAR();

void logsumexpDotProd_riscv_logsumexp_dot_prod_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output;

    generate_rand_f16(logsumexp_dotProd_f16_input1, ARRAY_SIZE);
    generate_rand_f16(logsumexp_dotProd_f16_input2, ARRAY_SIZE);

    BENCH_START(riscv_logsumexp_dot_prod_f16);
    f16_output = riscv_logsumexp_dot_prod_f16(logsumexp_dotProd_f16_input1, logsumexp_dotProd_f16_input2, ARRAY_SIZE, f16_temp_array);
    BENCH_END(riscv_logsumexp_dot_prod_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}

