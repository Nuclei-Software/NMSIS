#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ComplexMathFunctions/cmplx_mult_real_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void cmplx_mult_real_riscv_cmplx_mult_real_f16(void)
{

    float16_t cmplx_mult_real_f16_output[2 * ARRAY_SIZE_F16];

    generate_rand_f16(cmplx_mult_real_f16_input1, 2 * ARRAY_SIZE_F16);
    generate_rand_f16(cmplx_mult_real_f16_input2, 2 * ARRAY_SIZE_F16);

    BENCH_START(riscv_cmplx_mult_real_f16);
    riscv_cmplx_mult_real_f16(cmplx_mult_real_f16_input1, cmplx_mult_real_f16_input2, cmplx_mult_real_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_cmplx_mult_real_f16);

    return;
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
