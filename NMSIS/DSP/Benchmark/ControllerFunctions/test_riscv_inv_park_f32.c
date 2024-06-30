#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/ControllerFunctions/inv_park_f32/test_data.h"

BENCH_DECLARE_VAR();

void inv_park_riscv_inv_park_f32(void)
{
    float32_t pIalpha_f32[ARRAY_SIZE_F32];
    float32_t pIbeta_f32[ARRAY_SIZE_F32];

    volatile int i = 0;

    generate_rand_f32(Ia_f32, ARRAY_SIZE_F32);
    generate_rand_f32(Ib_f32, ARRAY_SIZE_F32);
    for (i = 0; i < ARRAY_SIZE_F32; i++) { // sin/cos values are between [-1, 1]
        sinVal_f32[i] = (float32_t)rand() / RAND_MAX * (0.9999F - (-0.9999F)) + (-0.9999F);
        cosVal_f32[i] = (float32_t)rand() / RAND_MAX * (0.9999F - (-0.9999F)) + (-0.9999F);
    }

    BENCH_START(riscv_inv_park_f32);
    for (i = 0; i < ARRAY_SIZE_F32; i++) {
        riscv_inv_park_f32(Ia_f32[i], Ib_f32[i], &pIalpha_f32[i], &pIbeta_f32[i], sinVal_f32[i], cosVal_f32[i]);
    }
    BENCH_END(riscv_inv_park_f32);

    return;
}
