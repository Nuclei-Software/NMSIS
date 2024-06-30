#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/copy_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void copy_riscv_copy_f16(void)
{
    float16_t f16_output[ARRAY_SIZE];

    generate_rand_f16(copy_f16_input, ARRAY_SIZE);

    BENCH_START(riscv_copy_f16);
    riscv_copy_f16(copy_f16_input, f16_output, ARRAY_SIZE);
    BENCH_END(riscv_copy_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */