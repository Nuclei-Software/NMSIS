#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/SupportFunctions/barycenter_f32/test_data.h"

BENCH_DECLARE_VAR();

void barycenter_riscv_barycenter_f32(void)
{
    float32_t f32_output[DIMENSION];

    generate_rand_f32(f32_barycenter_array, VEC_NUM * DIMENSION);

    for (int i = 0; i < VEC_NUM; i++) {
        f32_barycenter_weights_array[i] = (float32_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_barycenter_f32);
    riscv_barycenter_f32(f32_barycenter_array, f32_barycenter_weights_array, f32_output, VEC_NUM, DIMENSION);
    BENCH_END(riscv_barycenter_f32);

    return;
}
