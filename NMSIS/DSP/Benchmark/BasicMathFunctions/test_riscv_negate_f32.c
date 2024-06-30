#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/negate_f32/test_data.h"

BENCH_DECLARE_VAR();

void negate_riscv_negate_f32(void)
{
    float32_t negate_f32_output[ARRAY_SIZE_F32];
    generate_rand_f32(negate_f32_input, ARRAY_SIZE_F32);

    // f32_mult
    BENCH_START(riscv_negate_f32);
    riscv_negate_f32(negate_f32_input, negate_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_negate_f32);
}