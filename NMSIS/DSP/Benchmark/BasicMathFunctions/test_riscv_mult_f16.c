#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/mult_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void mul_riscv_mul_f16(void)
{
    float16_t mult_f16_output[ARRAY_SIZE_F16];
    generate_rand_f16(mult_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(mult_f16_input2, ARRAY_SIZE_F16);

    // f16_mult
    BENCH_START(riscv_mult_f16);
    riscv_mult_f16(mult_f16_input1, mult_f16_input2, mult_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_mult_f16);
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */
