#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/DistanceFunctions/jensenshannon_distance_f16/test_data.h"

BENCH_DECLARE_VAR();

void jensenshannon_distance_riscv_jensenshannon_distance_f16(void)
{
#if defined (RISCV_FLOAT16_SUPPORTED)
    float16_t jensenshannon_distance_f16_output;

    generate_rand_f16(jensenshannon_distance_f16_input1, ARRAY_SIZE_F16);
    generate_rand_f16(jensenshannon_distance_f16_input2, ARRAY_SIZE_F16);

    BENCH_START(riscv_jensenshannon_distance_f16);
    jensenshannon_distance_f16_output = riscv_jensenshannon_distance_f16(jensenshannon_distance_f16_input1, jensenshannon_distance_f16_input2, ARRAY_SIZE_F16);
    BENCH_END(riscv_jensenshannon_distance_f16);

#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
}
