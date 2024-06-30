#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/DistanceFunctions/euclidean_distance_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void euclidean_distance_riscv_euclidean_distance_f16(void)
{
    float16_t euclidean_distance_f16_output;

    generate_rand_f16(euclidean_distance_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(euclidean_distance_f16_input2, ARRAY_SIZE_F16);

    BENCH_START(riscv_euclidean_distance_f16);
    euclidean_distance_f16_output = riscv_euclidean_distance_f16(euclidean_distance_f16_input1, euclidean_distance_f16_input2, ARRAY_SIZE_F16);
    BENCH_END(riscv_euclidean_distance_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */