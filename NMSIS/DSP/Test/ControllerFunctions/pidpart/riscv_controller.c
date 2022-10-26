// This contains SIN_COS , clarke, inv_clarke, park, inv_park and pid
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "array.h"
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

#define ENABLE_ALL
#define CLARKE
#define INV_CLARKE
#define PARK
#define INV_PARK
#define PID

int test_flag_error = 0;

BENCH_DECLARE_VAR();

#define BLOCK_TESTSZ        100

static int DSP_clarke_f32_app()
{
    volatile int i = 0;

    float32_t Ia[BLOCK_TESTSZ];
    float32_t Ib[BLOCK_TESTSZ];

    float32_t pIalpha[BLOCK_TESTSZ], pIalpha_ref[BLOCK_TESTSZ];
    float32_t pIbeta[BLOCK_TESTSZ], pIbeta_ref[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia[i] = (float32_t)rand() / RAND_MAX;
        Ib[i] = (float32_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_clarke_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_clarke_f32(Ia[i], Ib[i], &pIalpha[i], &pIbeta[i]);
    }
    BENCH_END(riscv_clarke_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pIalpha_ref[i] = Ia[i];
        pIbeta_ref[i] = (1 / sqrt(3)) * Ia[i] + (2 / sqrt(3)) * Ib[i];
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((fabs(pIalpha[i] - pIalpha_ref[i]) > DELTAF32) ||
            (fabs(pIbeta[i] - pIbeta_ref[i]) > DELTAF32)) {
            BENCH_ERROR(riscv_clarke_f32);
            printf("pIalpha expect: %f, actual: %f\npIbeta expect: %f, actual: %f",
                    pIalpha_ref[i], pIalpha[i], pIbeta_ref[i], pIbeta[i]);
            test_flag_error = 1;
      }
    }
    BENCH_STATUS(riscv_clarke_f32);
}

static int DSP_clarke_q31_app()
{
    volatile int i = 0;

    float32_t Ia[BLOCK_TESTSZ];
    float32_t Ib[BLOCK_TESTSZ];

    q31_t Ia1[BLOCK_TESTSZ];
    q31_t Ib1[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia[i] = (float32_t)rand() / RAND_MAX;
        Ib[i] = (float32_t)rand() / RAND_MAX;
    }
    riscv_float_to_q31(Ia, Ia1, BLOCK_TESTSZ);
    riscv_float_to_q31(Ib, Ib1, BLOCK_TESTSZ);

    q31_t pIalpha[BLOCK_TESTSZ], pIalpha_ref[BLOCK_TESTSZ];
    q31_t pIbeta[BLOCK_TESTSZ], pIbeta_ref[BLOCK_TESTSZ];
    q63_t pIbeta_refTmp;

    BENCH_START(riscv_clarke_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_clarke_q31(Ia1[i], Ib1[i], &pIalpha[i], &pIbeta[i]);
    }
    BENCH_END(riscv_clarke_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pIalpha_ref[i] = Ia1[i];
        pIbeta_refTmp = (q63_t)((q31_t)(((q63_t)Ia1[i] * 0x24F34E8B) >> 30)) + (q31_t)(((q63_t)Ib1[i] * 0x49E69D16) >> 30);
        pIbeta_ref[i] = (q31_t)(clip_q63_to_q31(pIbeta_refTmp));
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((labs(pIalpha[i] - pIalpha_ref[i]) > DELTAQ31) ||
            (labs(pIbeta[i] - pIbeta_ref[i]) > DELTAQ31)) {
            BENCH_ERROR(riscv_clarke_q31);
            printf("pIalpha expect: %x, actual: %x\npIbeta expect: %x, actual: %x",
                   pIalpha_ref[i], pIalpha[i], pIbeta_ref[i], pIbeta[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_clarke_q31);
}

static int DSP_inv_clarke_f32_app()
{
    volatile int i = 0;

    float32_t Ia[BLOCK_TESTSZ], Ia_ref[BLOCK_TESTSZ];
    float32_t Ib[BLOCK_TESTSZ], Ib_ref[BLOCK_TESTSZ];

    float32_t pIalpha[BLOCK_TESTSZ];
    float32_t pIbeta[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pIalpha[i] = (float32_t)rand() / RAND_MAX;
        pIbeta[i] = (float32_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_inv_clarke_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_inv_clarke_f32(pIalpha[i], pIbeta[i], &Ia[i], &Ib[i]);
    }
    BENCH_END(riscv_inv_clarke_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia_ref[i] = pIalpha[i];
        Ib_ref[i] = -0.5f * pIalpha[i] + 0.8660254039f * pIbeta[i];
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((fabs(Ia[i] - Ia_ref[i]) > DELTAF32) || (fabs(Ib[i] - Ib_ref[i]) > DELTAF32)) {
            BENCH_ERROR(riscv_inv_clarke_f32);
            printf("Ia expect: %f, actual: %f\nIb expect: %f, actual: %f", Ia_ref[i],
                   Ia[i], Ib_ref[i], Ib[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_inv_clarke_f32);
}

static int DSP_inv_clarke_q31_app()
{
    volatile int i = 0;

    q31_t Ia[BLOCK_TESTSZ], Ia_ref[BLOCK_TESTSZ];
    q31_t Ib[BLOCK_TESTSZ], Ib_ref[BLOCK_TESTSZ];
    q31_t Ib_refTmp1, Ib_refTmp2;

    float32_t pIalpha[BLOCK_TESTSZ];
    float32_t pIbeta[BLOCK_TESTSZ];

    q31_t pIalpha1[BLOCK_TESTSZ];
    q31_t pIbeta1[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pIalpha[i] = (float32_t)rand() / RAND_MAX;
        pIbeta[i] = (float32_t)rand() / RAND_MAX;
    }
    riscv_float_to_q31(pIalpha, pIalpha1, BLOCK_TESTSZ);
    riscv_float_to_q31(pIbeta, pIbeta1, BLOCK_TESTSZ);

    BENCH_START(riscv_inv_clarke_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_inv_clarke_q31(pIalpha1[i], pIbeta1[i], &Ia[i], &Ib[i]);
    }
    BENCH_END(riscv_inv_clarke_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia_ref[i] = pIalpha1[i];
	Ib_refTmp1 = (q31_t)(((q63_t)(pIbeta1[i]) * (0x6ED9EBA1)) >> 31);
	Ib_refTmp2 = (q31_t)(((q63_t)(pIalpha1[i]) * (0x40000000)) >> 31);
	Ib_ref[i] = (q31_t)(clip_q63_to_q31((q63_t)Ib_refTmp1 - Ib_refTmp2));
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((labs(Ia[i] - Ia_ref[i]) > DELTAQ31) || (labs(Ib[i] - Ib_ref[i]) > DELTAQ31)) {
            BENCH_ERROR(riscv_inv_clarke_q31);
            printf("Ia expect: %x, actual: %x\nIb expect: %x, actual: %x", Ia_ref[i],
                   Ia[i], Ib_ref[i], Ib[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_inv_clarke_q31);
}

static int DSP_park_f32_app()
{
    volatile int i = 0;

    float32_t Ia[BLOCK_TESTSZ];
    float32_t Ib[BLOCK_TESTSZ];

    float32_t sinVal[BLOCK_TESTSZ];
    float32_t cosVal[BLOCK_TESTSZ];

    float32_t pId[BLOCK_TESTSZ], pId_ref[BLOCK_TESTSZ];
    float32_t pIq[BLOCK_TESTSZ], pIq_ref[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia[i] = (float32_t)rand() / RAND_MAX;
        Ib[i] = (float32_t)rand() / RAND_MAX;
    }

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        sinVal[i] = (float32_t)rand() / RAND_MAX;
        cosVal[i] = (float32_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_park_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_park_f32(Ia[i], Ib[i], &pId[i], &pIq[i], sinVal[i], cosVal[i]);
    }
    BENCH_END(riscv_park_f32);

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pId_ref[i] = Ia[i] * cosVal[i] + Ib[i] * sinVal[i];
        pIq_ref[i] = -Ia[i] * sinVal[i] + Ib[i] * cosVal[i];
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((fabs(pId[i] - pId_ref[i]) > DELTAF32) || (fabs(pIq[i] - pIq_ref[i]) > DELTAF32)) {
            BENCH_ERROR(riscv_park_f32);
            printf("pId expect: %f, actual: %f\npIq expect: %f, actual: %f",
                   pId_ref[i], pId[i], pIq_ref[i], pIq[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_park_f32);
}

static int DSP_park_q31_app()
{
    volatile int i = 0;

    float32_t Ia[BLOCK_TESTSZ];
    float32_t Ib[BLOCK_TESTSZ];

    q31_t Ia1[BLOCK_TESTSZ];
    q31_t Ib1[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia[i] = (float32_t)rand() / RAND_MAX;
        Ib[i] = (float32_t)rand() / RAND_MAX;
    }
    riscv_float_to_q31(Ia, Ia1, BLOCK_TESTSZ);
    riscv_float_to_q31(Ib, Ib1, BLOCK_TESTSZ);

    q31_t pId[BLOCK_TESTSZ], pId_ref[BLOCK_TESTSZ];
    q31_t pIq[BLOCK_TESTSZ], pIq_ref[BLOCK_TESTSZ];


    float32_t sinVal[BLOCK_TESTSZ], cosVal[BLOCK_TESTSZ];
    q31_t sinVal1[BLOCK_TESTSZ], cosVal1[BLOCK_TESTSZ];
    q31_t refTmp1, refTmp2;

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        sinVal[i] = (float32_t)rand() / RAND_MAX;
        cosVal[i] = (float32_t)rand() / RAND_MAX;
    }
    riscv_float_to_q31(sinVal, sinVal1, BLOCK_TESTSZ);
    riscv_float_to_q31(cosVal, cosVal1, BLOCK_TESTSZ);

    BENCH_START(riscv_park_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_park_q31(Ia1[i], Ib1[i], &pId[i], &pIq[i], sinVal1[i], cosVal1[i]);
    }
    BENCH_END(riscv_park_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        refTmp1 = (q31_t)(((q63_t)(Ia1[i]) * (cosVal1[i])) >> 31);
        refTmp2 = (q31_t)(((q63_t)(Ib1[i]) * (sinVal1[i])) >> 31);
        pId_ref[i] = (q31_t)(clip_q63_to_q31((q63_t)refTmp1 + refTmp2));
        refTmp1 = (q31_t)(((q63_t)(Ib1[i]) * (cosVal1[i])) >> 31);
        refTmp2 = (q31_t)(((q63_t)(Ia1[i]) * (sinVal1[i])) >> 31);
        pIq_ref[i] = (q31_t)(clip_q63_to_q31((q63_t)refTmp1 - refTmp2));
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((labs(pId[i] - pId_ref[i]) > DELTAQ31) || (labs(pIq[i] - pIq_ref[i]) > DELTAQ31)) {
            BENCH_ERROR(riscv_park_q31);
            printf("pId expect: %x, actual: %x\npIq expect: %x, actual: %x",
                   pId_ref[i], pId[i], pIq_ref[i], pIq[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_park_q31);
}

static int DSP_inv_park_f32_app()
{
    volatile int i = 0;

    float32_t Ia[BLOCK_TESTSZ];
    float32_t Ib[BLOCK_TESTSZ];

    float32_t pIalpha[BLOCK_TESTSZ], pIalpha_ref[BLOCK_TESTSZ];
    float32_t pIbeta[BLOCK_TESTSZ], pIbeta_ref[BLOCK_TESTSZ];

    float32_t sinVal[BLOCK_TESTSZ];
    float32_t cosVal[BLOCK_TESTSZ];

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia[i] = (float32_t)rand() / RAND_MAX;
        Ib[i] = (float32_t)rand() / RAND_MAX;
    }

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        sinVal[i] = (float32_t)rand() / RAND_MAX;
        cosVal[i] = (float32_t)rand() / RAND_MAX;
    }

    BENCH_START(riscv_inv_park_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_inv_park_f32(Ia[i], Ib[i], &pIalpha[i], &pIbeta[i], sinVal[i], cosVal[i]);
    }
    BENCH_END(riscv_inv_park_f32);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pIalpha_ref[i] = Ia[i] * cosVal[i] - Ib[i] * sinVal[i];
        pIbeta_ref[i] = Ia[i] * sinVal[i] + Ib[i] * cosVal[i];
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((fabs(pIalpha[i] - pIalpha_ref[i]) > DELTAF32) ||
            (fabs(pIbeta[i] - pIbeta_ref[i]) > DELTAF32)) {
            BENCH_ERROR(riscv_inv_park_f32);
            printf("pIalpha expect: %f, actual: %f\npIbeta expect: %f, actual: %f",
                    pIalpha_ref[i], pIalpha[i], pIbeta_ref[i], pIbeta[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_inv_park_f32);
}

static int DSP_inv_park_q31_app()
{
    volatile int i = 0;

    q31_t Ia1[BLOCK_TESTSZ];
    q31_t Ib1[BLOCK_TESTSZ];

    q31_t pIalpha[BLOCK_TESTSZ], pIalpha_ref[BLOCK_TESTSZ];
    q31_t pIbeta[BLOCK_TESTSZ], pIbeta_ref[BLOCK_TESTSZ];

    q31_t sinValb[BLOCK_TESTSZ];
    q31_t cosValb[BLOCK_TESTSZ];

    q63_t pIalpha_refTmp, pIbeta_refTmp;

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia1[i] = (q31_t)(((float32_t)rand() / RAND_MAX) * pow(2, 31));
        Ib1[i] = (q31_t)(((float32_t)rand() / RAND_MAX) * pow(2, 31));
    }

    for (i = 0; i < BLOCK_TESTSZ; i++) {
        Ia1[i] = (q31_t)(((float32_t)rand() / RAND_MAX) * pow(2, 31));
        Ib1[i] = (q31_t)(((float32_t)rand() / RAND_MAX) * pow(2, 31));
    }

    BENCH_START(riscv_inv_park_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        riscv_inv_park_q31(Ia1[i], Ib1[i], &pIalpha[i], &pIbeta[i], sinValb[i], cosValb[i]);
    }
    BENCH_END(riscv_inv_park_q31);
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        pIalpha_refTmp = (((q63_t)Ia1[i] * cosValb[i]) >> 31) - (((q63_t)Ib1[i] * sinValb[i]) >> 31);
        pIalpha_ref[i] = (q31_t)clip_q63_to_q31(pIalpha_refTmp);
        pIbeta_refTmp = (((q63_t)Ia1[i] * sinValb[i]) >> 31) + (((q63_t)Ib1[i] * cosValb[i]) >> 31);
        pIbeta_ref[i] = (q31_t)clip_q63_to_q31(pIbeta_refTmp);
    }
    for (i = 0; i < BLOCK_TESTSZ; i++) {
        if ((labs(pIalpha[i] - pIalpha_ref[i]) > DELTAQ31) ||
            (labs(pIbeta[i] - pIbeta_ref[i]) > DELTAQ31)) {
            BENCH_ERROR(riscv_inv_park_q31);
            printf("pIalpha expect: %x, actual: %x\npIbeta expect: %x, actual: %x\n",
                    pIalpha_ref[i], pIalpha[i], pIbeta_ref[i], pIbeta[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_inv_park_q31);
}

static int DSP_pid_f32_app()
{
    volatile int i = 0;

    riscv_pid_instance_f32 PIDS, PIDS_ref;
    PIDS.Kp = (float32_t)rand() / RAND_MAX;
    PIDS.Ki = (float32_t)rand() / RAND_MAX;
    PIDS.Kd = (float32_t)rand() / RAND_MAX;
    float32_t target, ival, ee = 0, ee_ref = 0;
    output_f32[0] = 0;
    output_f32_ref[0] = 0;
    /* Target value*/
    target = (float32_t)rand();
    /* Inital value */
    ival = 0;
    /* Initial value and target value error */
    ee = target - ival;
    ee_ref = ee;
    /* Initial DSP PID controller function*/
    riscv_pid_init_f32(&PIDS, 1);
    memcpy(&PIDS_ref, &PIDS, sizeof(riscv_pid_instance_f32));
    BENCH_START(riscv_pid_f32);
    for (i = 1; i < TEST_SIZE; i++) {
        output_f32[i] = riscv_pid_f32(&PIDS, ee);
        ee = target - output_f32[i - 1];
    }
    BENCH_END(riscv_pid_f32);
    for (i = 1; i < TEST_SIZE; i++) {
        output_f32_ref[i] = ref_pid_f32(&PIDS_ref, ee_ref);
        ee_ref = target - output_f32_ref[i - 1];
    }
    for (i = 0; i < TEST_SIZE; i++) {
        if (fabs(output_f32[i] - output_f32_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_pid_f32);
            printf("index: %d, expect: %f, actual: %f\n", i, output_f32_ref[i],
                   output_f32[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_pid_f32);
}

static int DSP_pid_q15_app()
{
    volatile int i = 0;

    riscv_pid_instance_f32 PIDS;
    riscv_pid_instance_q15 PIDS_q15, PIDS_q15_ref;
    PIDS.Kp = (float32_t)rand() / RAND_MAX;
    PIDS.Ki = (float32_t)rand() / RAND_MAX;
    PIDS.Kd = (float32_t)rand() / RAND_MAX;
    riscv_float_to_q15(&PIDS.Kp, &PIDS_q15.Kp, 1);
    riscv_float_to_q15(&PIDS.Ki, &PIDS_q15.Ki, 1);
    riscv_float_to_q15(&PIDS.Kd, &PIDS_q15.Kd, 1);
    q15_t ee_q15 = 0, ee_q15_ref = 0, target, ival;
    output_q15[0] = 0;
    output_q15_ref[0] = 0;
    /* Target value*/
    target = (q15_t)(rand() % 0x7fff);
    /* Inital value */
    ival = 0;
    /* Initial value and target value error */
    ee_q15 = target - ival;
    ee_q15_ref = ee_q15;
    /* Initial DSP PID controller function*/
    riscv_pid_init_q15(&PIDS_q15, 0);
    memcpy(&PIDS_q15_ref, &PIDS_q15, sizeof(riscv_pid_instance_q15));
    BENCH_START(riscv_pid_q15);
    for (i = 1; i < TEST_SIZE; i++) {
        output_q15[i] = riscv_pid_q15(&PIDS_q15, ee_q15);
        ee_q15 = target - output_q15[i - 1];
    }
    BENCH_END(riscv_pid_q15);
    for (i = 1; i < TEST_SIZE; i++) {
        output_q15_ref[i] = ref_pid_q15(&PIDS_q15_ref, ee_q15_ref);
        ee_q15_ref = target - output_q15_ref[i - 1];
    }
    for (i = 0; i < TEST_SIZE; i++) {
        if (abs(output_q15[i] - output_q15_ref[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_pid_q15);
            printf("index: %d, expect: %x, actual: %x\n", (i), output_q15_ref[i],
                   output_q15[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_pid_q15);
}

static int DSP_pid_q31_app()
{
    volatile int i = 0;

    riscv_pid_instance_f32 PIDS;
    riscv_pid_instance_q31 PIDS_q31, PIDS_q31_ref;
    PIDS.Kp = (float32_t)rand() / RAND_MAX;
    PIDS.Ki = (float32_t)rand() / RAND_MAX;
    PIDS.Kd = (float32_t)rand() / RAND_MAX;
    riscv_float_to_q31(&PIDS.Kp, &PIDS_q31.Kp, 1);
    riscv_float_to_q31(&PIDS.Ki, &PIDS_q31.Ki, 1);
    riscv_float_to_q31(&PIDS.Kd, &PIDS_q31.Kd, 1);
    q31_t ee_q31 = 0, ee_q31_ref = 0, target, ival;
    output_q31[0] = 0;
    output_q31_ref[0] = 0;
    /* Target value*/
    target = (q31_t)rand();
    /* Inital value */
    ival = 0;
    /* Initial value and target value error */
    ee_q31 = target - ival;
    ee_q31_ref = ee_q31;
    /* Initial DSP PID controller function*/
    riscv_pid_init_q31(&PIDS_q31, 1);
    memcpy(&PIDS_q31_ref, &PIDS_q31, sizeof(riscv_pid_instance_q31));
    BENCH_START(riscv_pid_q31);
    for (i = 1; i < TEST_SIZE; i++) {
        output_q31[i] = riscv_pid_q31(&PIDS_q31, ee_q31);
        ee_q31 = target - output_q31[i - 1];
    }
    BENCH_END(riscv_pid_q31);
    for (i = 1; i < TEST_SIZE; i++) {
        output_q31_ref[i] = ref_pid_q31(&PIDS_q31_ref, ee_q31_ref);
        ee_q31_ref = target - output_q31_ref[i - 1];
    }
    for (i = 0; i < TEST_SIZE; i++) {
        if (labs(output_q31[i] - output_q31_ref[i]) > DELTAQ31) {
	    BENCH_ERROR(riscv_pid_q31);
	    printf("index: %d, expect: %x, actual: %x\n", i, output_q31_ref[i],
                   output_q31[i]);
	    test_flag_error = 1;
	}
    }
    BENCH_STATUS(riscv_pid_q31);
}

int main()
{
    BENCH_INIT();
    srand(__RV_CSR_READ(mcycle));
#if defined CLARKE || defined ENABLE_ALL
    DSP_clarke_f32_app();
    DSP_clarke_q31_app();
#endif
#if defined INV_CLARKE || defined ENABLE_ALL
    DSP_inv_clarke_f32_app();
    DSP_inv_clarke_q31_app();
#endif
#if defined PARK || defined ENABLE_ALL
    DSP_park_f32_app();
    DSP_park_q31_app();
#endif
#if defined INV_PARK || defined ENABLE_ALL
    DSP_inv_park_f32_app();
    DSP_inv_park_q31_app();
#endif
#if defined PID || defined ENABLE_ALL
    DSP_pid_f32_app();
    DSP_pid_q15_app();
    DSP_pid_q31_app();
#endif

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
