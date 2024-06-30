#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/DistanceFunctions/minkowski_distance_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void minkowski_distance_riscv_minkowski_distance_f16(void)
{
    float16_t minkowski_distance_f16_output;

    generate_rand_f16(minkowski_distance_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(minkowski_distance_f16_input2, ARRAY_SIZE_F16);

    BENCH_START(riscv_minkowski_distance_f16);
    minkowski_distance_f16_output = riscv_minkowski_distance_f16(minkowski_distance_f16_input1, minkowski_distance_f16_input2, 3, ARRAY_SIZE_F16);
    BENCH_END(riscv_minkowski_distance_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */