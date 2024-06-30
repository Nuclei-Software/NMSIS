#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/sub_f16/test_data.h"

#if defined(RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void sub_riscv_sub_f16(void)
{
    float16_t sub_f16_output[ARRAY_SIZE_F16];
    generate_rand_f16(sub_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(sub_f16_input2, ARRAY_SIZE_F16);
    BENCH_START(riscv_sub_f16);
    riscv_sub_f16(sub_f16_input1, sub_f16_input2, sub_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_sub_f16);
}
#endif /* #if defined(RISCV_FLOAT16_SUPPORTED) */