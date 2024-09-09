#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/float_to_f16/test_data.h"

BENCH_DECLARE_VAR();

void f32ToF16_riscv_float_to_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t f16_output[ARRAY_SIZE];

    generate_rand_f32(f32_input, ARRAY_SIZE);

    BENCH_START(riscv_float_to_f16);
    riscv_float_to_f16(f32_input, f16_output, ARRAY_SIZE);
    BENCH_END(riscv_float_to_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
