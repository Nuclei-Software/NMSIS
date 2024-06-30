#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/abs_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void abs_riscv_abs_f16(void)
{

    float16_t abs_f16_output[ARRAY_SIZE_F16];
    
    generate_rand_f16(abs_f16_input, ARRAY_SIZE_F16);

    // riscv_abs_f32.c
    BENCH_START(riscv_abs_f16);
    riscv_abs_f16(abs_f16_input, abs_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_abs_f16);

    return;
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
