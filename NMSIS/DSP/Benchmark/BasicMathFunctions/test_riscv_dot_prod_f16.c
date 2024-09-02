#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/dot_prod_f16/test_data.h"

BENCH_DECLARE_VAR();

void dot_prod_riscv_dot_prod_f16(void)
{
#if defined(RISCV_FLOAT16_SUPPORTED)
    float16_t dot_prod_f16_output;

    generate_rand_f16(dot_prod_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(dot_prod_f16_input2, ARRAY_SIZE_F16);

    // f16_dot
    BENCH_START(riscv_dot_prod_f16);
    riscv_dot_prod_f16(dot_prod_f16_input1, dot_prod_f16_input2, ARRAY_SIZE_F16, &dot_prod_f16_output);
    BENCH_END(riscv_dot_prod_f16);
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
}
