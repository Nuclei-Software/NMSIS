#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/SupportFunctions/merge_sort_f32/test_data.h"

BENCH_DECLARE_VAR();

void mergeSort_riscv_merge_sort_f32(void)
{
    
    float32_t f32_temp_array[ARRAY_SIZE];
    float32_t f32_out_array[ARRAY_SIZE];
    riscv_merge_sort_instance_f32 S_merge_sort;

    generate_rand_f32(merge_sort_f32_input, ARRAY_SIZE);
    riscv_merge_sort_init_f32(&S_merge_sort, RISCV_SORT_ASCENDING, f32_temp_array);

    BENCH_START(riscv_merge_sort_f32);
    riscv_merge_sort_f32(&S_merge_sort, merge_sort_f32_input, f32_out_array, ARRAY_SIZE);
    BENCH_END(riscv_merge_sort_f32);
}
