#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/DistanceFunctions/chebyshev_distance_f32/test_data.h"

BENCH_DECLARE_VAR();

void chebyshev_distance_riscv_chebyshev_distance_f32(void)
{
    float32_t chebyshev_distance_f32_output;

    generate_rand_f32(chebyshev_distance_f32_input1, ARRAY_SIZE_F32);
    generate_rand_f32(chebyshev_distance_f32_input2, ARRAY_SIZE_F32);

    BENCH_START(riscv_chebyshev_distance_f32);
    chebyshev_distance_f32_output = riscv_chebyshev_distance_f32(chebyshev_distance_f32_input1, chebyshev_distance_f32_input2, ARRAY_SIZE_F32);
    BENCH_END(riscv_chebyshev_distance_f32);

    return;
}