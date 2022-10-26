// This contains SIN_COS , clarke, inv_clarke, park, inv_park and pid
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

int test_flag_error = 0;

BENCH_DECLARE_VAR();

#define BLOCK_TESTSZ        16

static int DSP_SIN_COS_F32(void)
{
    volatile int i;
    float32_t pSinVal[BLOCK_TESTSZ], pSinVal_ref[BLOCK_TESTSZ];
    float32_t pCosVal[BLOCK_TESTSZ], pCosVal_ref[BLOCK_TESTSZ];
    float32_t f32_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        f32_pIN[i] = (float32_t)rand() / 0x7ff;
    }

    BENCH_START(riscv_sin_cos_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_sin_cos_f32(f32_pIN[i], &pSinVal[i], &pCosVal[i]);
    }
    BENCH_END(riscv_sin_cos_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        ref_sin_cos_f32(f32_pIN[i], &pSinVal_ref[i], &pCosVal_ref[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((fabs(pSinVal[i] - pSinVal_ref[i]) > DELTAF32) ||
            (fabs(pCosVal[i] - pCosVal_ref[i]) > DELTAF32)) {
            BENCH_ERROR(riscv_sin_cos_f32);
            printf("sin expect: %f, actual: %f\ncos expect: %f, actual: %f",
                   pSinVal_ref[i], pSinVal[i], pCosVal_ref[i], pCosVal[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_sin_cos_f32);
}

static int DSP_SIN_COS_Q31(void)
{
    volatile int i;
    q31_t pSinVal[BLOCK_TESTSZ], pSinVal_ref[BLOCK_TESTSZ];
    q31_t pCosVal[BLOCK_TESTSZ], pCosVal_ref[BLOCK_TESTSZ];
    q31_t q31_pIN[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        q31_pIN[i] = (q31_t)rand();
    }

    BENCH_START(riscv_sin_cos_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_sin_cos_q31(q31_pIN[i], &pSinVal[i], &pCosVal[i]);
    }
    BENCH_END(riscv_sin_cos_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        ref_sin_cos_q31(q31_pIN[i], &pSinVal_ref[i], &pCosVal_ref[i]);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((labs(pSinVal[i] - pSinVal_ref[i]) > DELTAQ31) ||
            (labs(pCosVal[i] - pCosVal_ref[i]) > DELTAQ31)) {
            BENCH_ERROR(riscv_sin_cos_q31);
            printf("sin expect: %x, actual: %x\ncos expect: %x, actual: %x",
                   pSinVal_ref[i], pSinVal[i], pCosVal_ref[i], pCosVal[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_sin_cos_q31);
}

int main()
{
    BENCH_INIT();

    srand(__RV_CSR_READ(mcycle));

    DSP_SIN_COS_F32();
    DSP_SIN_COS_Q31();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
