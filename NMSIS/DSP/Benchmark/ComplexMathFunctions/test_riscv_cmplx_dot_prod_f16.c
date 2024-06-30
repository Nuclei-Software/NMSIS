#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ComplexMathFunctions/cmplx_dot_prod_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void cmplx_dot_prod_riscv_cmplx_dot_prod_f16(void)
{

    float16_t cmplx_dot_prod_f16_realOutput, cmplx_dot_prod_f16_imagOutput;

    generate_rand_f16(cmplx_dot_prod_f16_input1, 2 * ARRAY_SIZE_F16);
    generate_rand_f16(cmplx_dot_prod_f16_input2, 2 * ARRAY_SIZE_F16);

    BENCH_START(riscv_cmplx_dot_prod_f16);
    riscv_cmplx_dot_prod_f16(cmplx_dot_prod_f16_input1, cmplx_dot_prod_f16_input2, ARRAY_SIZE_F16, &cmplx_dot_prod_f16_realOutput,
                           &cmplx_dot_prod_f16_imagOutput);
    BENCH_END(riscv_cmplx_dot_prod_f16);

    return;
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
