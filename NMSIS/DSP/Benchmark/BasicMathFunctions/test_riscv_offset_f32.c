#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/BasicMathFunctions/offset_f32/test_data.h"

BENCH_DECLARE_VAR();

void offset_riscv_offset_f32(void)
{
    float32_t offset_f32_output[ARRAY_SIZE_F32];
    float32_t offset_f32;

    generate_rand_f32(offset_f32_input, ARRAY_SIZE_F32);

    // f32_offset
    generate_rand_f32(&offset_f32, 1);
    BENCH_START(riscv_offset_f32);
    riscv_offset_f32(offset_f32_input, offset_f32, offset_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_offset_f32);
}