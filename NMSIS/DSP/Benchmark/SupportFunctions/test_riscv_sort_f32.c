#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/sort_f32/test_data.h"

BENCH_DECLARE_VAR();

void sort_riscv_sort_f32(void)
{
    float32_t f32_output[ARRAY_SIZE];

    generate_rand_f32(sort_f32_input, ARRAY_SIZE);

    riscv_sort_instance_f32 S_sort;
    riscv_sort_init_f32(&S_sort, RISCV_SORT_BUBBLE, RISCV_SORT_ASCENDING);

    BENCH_START(riscv_sort_f32);
    riscv_sort_f32(&S_sort, sort_f32_input, f32_output, ARRAY_SIZE);
    BENCH_END(riscv_sort_f32);

}