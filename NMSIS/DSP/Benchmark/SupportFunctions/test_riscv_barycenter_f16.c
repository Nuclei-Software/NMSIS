#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/barycenter_f16/test_data.h"

#if defined (RISCV_FLOAT16_SUPPORTED)

BENCH_DECLARE_VAR();

void barycenter_riscv_barycenter_f16(void)
{
    float16_t f16_output[DIMENSION];

    generate_rand_f16(f16_barycenter_array, VEC_NUM * DIMENSION);

    for (int i = 0; i < VEC_NUM; i++) {
        f16_barycenter_weights_array[i] = (float16_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_barycenter_f16);
    riscv_barycenter_f16(f16_barycenter_array, f16_barycenter_weights_array, f16_output, VEC_NUM, DIMENSION);
    BENCH_END(riscv_barycenter_f16);

    return;
}
#endif /* #if defined (RISCV_FLOAT16_SUPPORTED) */
