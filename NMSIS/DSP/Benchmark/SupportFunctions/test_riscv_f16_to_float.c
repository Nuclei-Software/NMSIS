#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/f16_to_float/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void f16ToF32_riscv_f16_to_float(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_f16(f16_input, ARRAY_SIZE);

    BENCH_START(riscv_f16_to_float);
    riscv_f16_to_float(f16_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_f16_to_float);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */