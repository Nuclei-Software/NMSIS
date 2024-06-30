#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/vlog_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void vlog_riscv_vlog_f16(void)
{
    
    float16_t vlog_f16_output[ARRAY_SIZE_F16];
    
    generate_rand_f16(vlog_f16_input, ARRAY_SIZE_F16);

    BENCH_START(riscv_vlog_f16);
    riscv_vlog_f16(vlog_f16_input, vlog_f16_output, ARRAY_SIZE_F16);
    BENCH_END(riscv_vlog_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */