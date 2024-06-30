#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/BasicMathFunctions/sub_f32/test_data.h"

BENCH_DECLARE_VAR();

void sub_riscv_sub_f32(void)
{
    float32_t sub_f32_output[ARRAY_SIZE_F32];
    generate_rand_f32(sub_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(sub_f32_input2, ARRAY_SIZE_F32);
    BENCH_START(riscv_sub_f32);
    riscv_sub_f32(sub_f32_input1, sub_f32_input2, sub_f32_output, ARRAY_SIZE_F32);
    BENCH_END(riscv_sub_f32);
}