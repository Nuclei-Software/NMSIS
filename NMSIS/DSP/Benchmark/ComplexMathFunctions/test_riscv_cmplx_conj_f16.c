#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ComplexMathFunctions/cmplx_conj_f16/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_conj_riscv_cmplx_conj_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t cmplx_conj_f16_output[2 * ARRAY_SIZE_F16];
    
    generate_rand_f16(cmplx_conj_f16_input, ARRAY_SIZE_F16 * 2);

    // riscv_cmplx_conj_f32.c
    BENCH_START(riscv_cmplx_conj_f16);
    riscv_cmplx_conj_f16(cmplx_conj_f16_input, cmplx_conj_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_cmplx_conj_f16);

#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
}
