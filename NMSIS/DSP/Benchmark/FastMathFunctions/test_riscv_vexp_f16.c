#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/vexp_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void vexp_riscv_vexp_f16(void)
{

    float16_t vexp_f16_output[ARRAY_SIZE_F16];

    generate_rand_f16(vexp_f16_input, ARRAY_SIZE_F16);

    BENCH_START(riscv_vexp_f16);
    riscv_vexp_f16(vexp_f16_input, vexp_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_vexp_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */