#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/vexp_f32/test_data.h"

BENCH_DECLARE_VAR();

void vexp_riscv_vexp_f32(void)
{
    float32_t vexp_f32_output[ARRAY_SIZE_F32];
    
    generate_rand_f32(vexp_f32_input, ARRAY_SIZE_F32);

    BENCH_START(riscv_vexp_f32);
    riscv_vexp_f32(vexp_f32_input, vexp_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_vexp_f32);

    return;
}