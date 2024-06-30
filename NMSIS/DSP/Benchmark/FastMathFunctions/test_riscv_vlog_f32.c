#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/FastMathFunctions/vlog_f32/test_data.h"

BENCH_DECLARE_VAR();

void vlog_riscv_vlog_f32(void)
{

    float32_t vlog_f32_output[ARRAY_SIZE_F32];

    generate_rand_f32(vlog_f32_input, ARRAY_SIZE_F32);

    BENCH_START(riscv_vlog_f32);
    riscv_vlog_f32(vlog_f32_input, vlog_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_vlog_f32);

    return;
}