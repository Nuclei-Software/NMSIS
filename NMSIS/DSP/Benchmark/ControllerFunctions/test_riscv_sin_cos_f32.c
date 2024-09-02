#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/ControllerFunctions/sin_cos_f32/test_data.h"

BENCH_DECLARE_VAR();

void sin_cos_riscv_sin_cos_f32(void)
{
    volatile int i;
    float32_t pSinVal_f32[ARRAY_SIZE_F32];
    float32_t pCosVal_f32[ARRAY_SIZE_F32];

    for (i = 0; i < ARRAY_SIZE_F32; i++) {
        sin_cos_f32_input[i] = (float32_t)rand() / 0x7ff;
    }

    BENCH_START(riscv_sin_cos_f32);
    for (i = 0; i < ARRAY_SIZE_F32; i++) {
        riscv_sin_cos_f32(sin_cos_f32_input[i], &pSinVal_f32[i], &pCosVal_f32[i]);
    }
    BENCH_END(riscv_sin_cos_f32);
}