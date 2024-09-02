#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/DistanceFunctions/correlation_distance_f16/test_data.h"

BENCH_DECLARE_VAR();

void correlation_distance_riscv_correlation_distance_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t correlation_distance_f16_output;

    generate_rand_f16(correlation_distance_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(correlation_distance_f16_input2, ARRAY_SIZE_F16);

    BENCH_START(riscv_correlation_distance_f16);
    correlation_distance_f16_output = riscv_correlation_distance_f16(correlation_distance_f16_input1, correlation_distance_f16_input2, ARRAY_SIZE_F16);
    BENCH_END(riscv_correlation_distance_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
