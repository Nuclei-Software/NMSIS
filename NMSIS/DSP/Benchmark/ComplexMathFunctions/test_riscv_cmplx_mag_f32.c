#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ComplexMathFunctions/cmplx_mag_f32/test_data.h"

BENCH_DECLARE_VAR();

void cmplx_mag_riscv_cmplx_mag_f32(void)
{
    float32_t cmplx_mag_f32_output[2 * ARRAY_SIZE_F32];

    generate_rand_f32(cmplx_mag_f32_input, 2 * ARRAY_SIZE_F32);

    BENCH_START(riscv_cmplx_mag_f32);
    riscv_cmplx_mag_f32(cmplx_mag_f32_input, cmplx_mag_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_cmplx_mag_f32);

    return;
}
