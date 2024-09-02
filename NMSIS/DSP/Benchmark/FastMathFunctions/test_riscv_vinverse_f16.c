#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/FastMathFunctions/vinverse_f16/test_data.h"

BENCH_DECLARE_VAR();

void vinverse_riscv_vinverse_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED) 
    float16_t vinverse_f16_output[ARRAY_SIZE_F16];
    
    generate_rand_f16(vinverse_f16_input, ARRAY_SIZE_F16);

    BENCH_START(riscv_vinverse_f16);
    riscv_vinverse_f16(vinverse_f16_input, vinverse_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_vinverse_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
