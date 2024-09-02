#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ControllerFunctions/clarke_f32/test_data.h"

BENCH_DECLARE_VAR();

void clarke_riscv_clarke_f32(void)
{
    float32_t pIalpha_f32[ARRAY_SIZE_F32];
    float32_t pIbeta_f32[ARRAY_SIZE_F32];

    generate_rand_f32(Ia_f32, ARRAY_SIZE_F32);
    generate_rand_f32(Ib_f32, ARRAY_SIZE_F32);

    volatile int i = 0;

    BENCH_START(riscv_clarke_f32);
    for (i = 0; i < ARRAY_SIZE_F32; i++) {
        riscv_clarke_f32(Ia_f32[i], Ib_f32[i], &pIalpha_f32[i], &pIbeta_f32[i]);
    }
    BENCH_END(riscv_clarke_f32);

    return;
}