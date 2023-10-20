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

#define ENABLE_ALL

#ifdef ENABLE_ALL
#define CLARKE
#define INV_CLARKE
#define PARK
#define INV_PARK
#define PID
#define SIN_COS
#endif

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

#define TEST_SIZE 100
float32_t output_f32[TEST_SIZE], output_f32_ref[TEST_SIZE];
q31_t output_q31[TEST_SIZE], output_q31_ref[TEST_SIZE];
q15_t output_q15[TEST_SIZE], output_q15_ref[TEST_SIZE];

float32_t Ia[TEST_SIZE];
float32_t Ib[TEST_SIZE];
float32_t pIalpha[TEST_SIZE];
float32_t pIbeta[TEST_SIZE];

float32_t sinVal[TEST_SIZE];
float32_t cosVal[TEST_SIZE];

static int DSP_clarke_f32_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;

    float32_t pIalpha_1[TEST_SIZE], pIalpha_ref[TEST_SIZE];
    float32_t pIbeta_1[TEST_SIZE], pIbeta_ref[TEST_SIZE];

    BENCH_START(riscv_clarke_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_clarke_f32(Ia[i], Ib[i], &pIalpha_1[i], &pIbeta_1[i]);
    }
    BENCH_END(riscv_clarke_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        pIalpha_ref[i] = Ia[i];
        pIbeta_ref[i] = (1 / sqrt(3)) * Ia[i] + (2 / sqrt(3)) * Ib[i];
    }

    s1 = verify_results_f32(pIalpha_1, pIalpha_ref, TEST_SIZE);
    s2 = verify_results_f32(pIbeta_1, pIbeta_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_clarke_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_clarke_f32);
}

static int DSP_clarke_q31_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;

    q31_t Ia1[TEST_SIZE];
    q31_t Ib1[TEST_SIZE];

    riscv_float_to_q31(Ia, Ia1, TEST_SIZE);
    riscv_float_to_q31(Ib, Ib1, TEST_SIZE);

    q31_t pIalpha_1[TEST_SIZE], pIalpha_ref[TEST_SIZE];
    q31_t pIbeta_1[TEST_SIZE], pIbeta_ref[TEST_SIZE];
    q63_t pIbeta_refTmp;

    BENCH_START(riscv_clarke_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_clarke_q31(Ia1[i], Ib1[i], &pIalpha_1[i], &pIbeta_1[i]);
    }
    BENCH_END(riscv_clarke_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        pIalpha_ref[i] = Ia1[i];
        pIbeta_refTmp = (q63_t)((q31_t)(((q63_t)Ia1[i] * 0x24F34E8B) >> 30)) + (q31_t)(((q63_t)Ib1[i] * 0x49E69D16) >> 30);
        pIbeta_ref[i] = (q31_t)(clip_q63_to_q31(pIbeta_refTmp));
    }
    s1 = verify_results_q31(pIalpha_1, pIalpha_ref, TEST_SIZE);
    s2 = verify_results_q31(pIbeta_1, pIbeta_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_clarke_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_clarke_q31);
}

static int DSP_inv_clarke_f32_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;

    float32_t Ia_1[TEST_SIZE], Ia_ref[TEST_SIZE];
    float32_t Ib_1[TEST_SIZE], Ib_ref[TEST_SIZE];

    BENCH_START(riscv_inv_clarke_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_inv_clarke_f32(pIalpha[i], pIbeta[i], &Ia_1[i], &Ib_1[i]);
    }
    BENCH_END(riscv_inv_clarke_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        Ia_ref[i] = pIalpha[i];
        Ib_ref[i] = -0.5f * pIalpha[i] + 0.8660254039f * pIbeta[i];
    }
    s1 = verify_results_f32(Ia_1, Ia_ref, TEST_SIZE);
    s2 = verify_results_f32(Ib_1, Ib_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_inv_clarke_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_inv_clarke_f32);
}

static int DSP_inv_clarke_q31_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;

    q31_t Ia_1[TEST_SIZE], Ia_ref[TEST_SIZE];
    q31_t Ib_1[TEST_SIZE], Ib_ref[TEST_SIZE];
    q31_t Ib_refTmp1, Ib_refTmp2;

    q31_t pIalpha1[TEST_SIZE];
    q31_t pIbeta1[TEST_SIZE];

    riscv_float_to_q31(pIalpha, pIalpha1, TEST_SIZE);
    riscv_float_to_q31(pIbeta, pIbeta1, TEST_SIZE);

    BENCH_START(riscv_inv_clarke_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_inv_clarke_q31(pIalpha1[i], pIbeta1[i], &Ia_1[i], &Ib_1[i]);
    }
    BENCH_END(riscv_inv_clarke_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        Ia_ref[i] = pIalpha1[i];
        Ib_refTmp1 = (q31_t)(((q63_t)(pIbeta1[i]) * (0x6ED9EBA1)) >> 31);
        Ib_refTmp2 = (q31_t)(((q63_t)(pIalpha1[i]) * (0x40000000)) >> 31);
        Ib_ref[i] = (q31_t)(clip_q63_to_q31((q63_t)Ib_refTmp1 - Ib_refTmp2));
    }
    s1 = verify_results_q31(Ia_1, Ia_ref, TEST_SIZE);
    s2 = verify_results_q31(Ib_1, Ib_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_inv_clarke_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_inv_clarke_q31);
}

static int DSP_park_f32_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;
    float32_t pId[TEST_SIZE], pId_ref[TEST_SIZE];
    float32_t pIq[TEST_SIZE], pIq_ref[TEST_SIZE];

    BENCH_START(riscv_park_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_park_f32(Ia[i], Ib[i], &pId[i], &pIq[i], sinVal[i], cosVal[i]);
    }
    BENCH_END(riscv_park_f32);

    for (i = 0; i < TEST_SIZE; i++) {
        pId_ref[i] = Ia[i] * cosVal[i] + Ib[i] * sinVal[i];
        pIq_ref[i] = -Ia[i] * sinVal[i] + Ib[i] * cosVal[i];
    }
    s1 = verify_results_f32(pId, pId_ref, TEST_SIZE);
    s2 = verify_results_f32(pIq, pIq_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_park_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_park_f32);
}

static int DSP_park_q31_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;
    q31_t Ia1[TEST_SIZE];
    q31_t Ib1[TEST_SIZE];

    riscv_float_to_q31(Ia, Ia1, TEST_SIZE);
    riscv_float_to_q31(Ib, Ib1, TEST_SIZE);

    q31_t pId[TEST_SIZE], pId_ref[TEST_SIZE];
    q31_t pIq[TEST_SIZE], pIq_ref[TEST_SIZE];

    q31_t sinVal1[TEST_SIZE], cosVal1[TEST_SIZE];
    q31_t refTmp1, refTmp2;

    riscv_float_to_q31(sinVal, sinVal1, TEST_SIZE);
    riscv_float_to_q31(cosVal, cosVal1, TEST_SIZE);

    BENCH_START(riscv_park_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_park_q31(Ia1[i], Ib1[i], &pId[i], &pIq[i], sinVal1[i], cosVal1[i]);
    }
    BENCH_END(riscv_park_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        refTmp1 = (q31_t)(((q63_t)(Ia1[i]) * (cosVal1[i])) >> 31);
        refTmp2 = (q31_t)(((q63_t)(Ib1[i]) * (sinVal1[i])) >> 31);
        pId_ref[i] = (q31_t)(clip_q63_to_q31((q63_t)refTmp1 + refTmp2));
        refTmp1 = (q31_t)(((q63_t)(Ib1[i]) * (cosVal1[i])) >> 31);
        refTmp2 = (q31_t)(((q63_t)(Ia1[i]) * (sinVal1[i])) >> 31);
        pIq_ref[i] = (q31_t)(clip_q63_to_q31((q63_t)refTmp1 - refTmp2));
    }
    s1 = verify_results_q31(pId, pId_ref, TEST_SIZE);
    s2 = verify_results_q31(pIq, pIq_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_park_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_park_q31);
}

static int DSP_inv_park_f32_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;
    float32_t pIalpha_1[TEST_SIZE], pIalpha_ref[TEST_SIZE];
    float32_t pIbeta_1[TEST_SIZE], pIbeta_ref[TEST_SIZE];

    BENCH_START(riscv_inv_park_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_inv_park_f32(Ia[i], Ib[i], &pIalpha_1[i], &pIbeta_1[i], sinVal[i], cosVal[i]);
    }
    BENCH_END(riscv_inv_park_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        pIalpha_ref[i] = Ia[i] * cosVal[i] - Ib[i] * sinVal[i];
        pIbeta_ref[i] = Ia[i] * sinVal[i] + Ib[i] * cosVal[i];
    }
    s1 = verify_results_f32(pIalpha_1, pIalpha_ref, TEST_SIZE);
    s2 = verify_results_f32(pIbeta_1, pIbeta_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_inv_park_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_inv_park_f32);
}

static int DSP_inv_park_q31_app(void)
{
    volatile int i = 0;
    int8_t s1, s2;
    q31_t Ia1[TEST_SIZE];
    q31_t Ib1[TEST_SIZE];

    q31_t pIalpha_1[TEST_SIZE], pIalpha_ref[TEST_SIZE];
    q31_t pIbeta_1[TEST_SIZE], pIbeta_ref[TEST_SIZE];
    q31_t sinValb[TEST_SIZE];
    q31_t cosValb[TEST_SIZE];
    q63_t pIalpha_refTmp, pIbeta_refTmp;

    for (i = 0; i < TEST_SIZE; i++) {
        Ia1[i] = (q31_t)(((float32_t)rand() / RAND_MAX) * pow(2, 31));
        Ib1[i] = (q31_t)(((float32_t)rand() / RAND_MAX) * pow(2, 31));
    }
    riscv_float_to_q31(sinVal, sinValb, TEST_SIZE);
    riscv_float_to_q31(cosVal, cosValb, TEST_SIZE);

    BENCH_START(riscv_inv_park_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_inv_park_q31(Ia1[i], Ib1[i], &pIalpha_1[i], &pIbeta_1[i], sinValb[i], cosValb[i]);
    }
    BENCH_END(riscv_inv_park_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        pIalpha_refTmp = (((q63_t)Ia1[i] * cosValb[i]) >> 31) - (((q63_t)Ib1[i] * sinValb[i]) >> 31);
        pIalpha_ref[i] = (q31_t)clip_q63_to_q31(pIalpha_refTmp);
        pIbeta_refTmp = (((q63_t)Ia1[i] * sinValb[i]) >> 31) + (((q63_t)Ib1[i] * cosValb[i]) >> 31);
        pIbeta_ref[i] = (q31_t)clip_q63_to_q31(pIbeta_refTmp);
    }
    s1 = verify_results_q31(pIalpha_1, pIalpha_ref, TEST_SIZE);
    s2 = verify_results_q31(pIbeta_1, pIbeta_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_inv_park_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_inv_park_q31);
}

static int DSP_pid_f32_app(void)
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
    target = (float32_t)rand() / 0x7fff;
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
    s = verify_results_f32(output_f32, output_f32_ref, TEST_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_pid_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_pid_f32);
}

static int DSP_pid_q15_app(void)
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
    s = verify_results_q15(output_q15, output_q15_ref, TEST_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_pid_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_pid_q15);
}

static int DSP_pid_q31_app(void)
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
    s = verify_results_q31(output_q31, output_q31_ref, TEST_SIZE);
    if (s != 0) {
        BENCH_ERROR(riscv_pid_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_pid_q31);
}

static int DSP_SIN_COS_F32(void)
{
    volatile int i;
    int8_t s1, s2;
    float32_t pSinVal[TEST_SIZE], pSinVal_ref[TEST_SIZE];
    float32_t pCosVal[TEST_SIZE], pCosVal_ref[TEST_SIZE];
    float32_t f32_pIN[TEST_SIZE];

    for (i = 0; i < TEST_SIZE; i++) {
        f32_pIN[i] = (float32_t)rand() / 0x7ff;
    }

    BENCH_START(riscv_sin_cos_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_sin_cos_f32(f32_pIN[i], &pSinVal[i], &pCosVal[i]);
    }
    BENCH_END(riscv_sin_cos_f32);
    for (i = 0; i < TEST_SIZE; i++) {
        ref_sin_cos_f32(f32_pIN[i], &pSinVal_ref[i], &pCosVal_ref[i]);
    }
    s1 = verify_results_f32(pSinVal, pSinVal_ref, TEST_SIZE);
    s2 = verify_results_f32(pCosVal, pCosVal_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_sin_cos_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sin_cos_f32);
}

static int DSP_SIN_COS_Q31(void)
{
    volatile int i;
    int8_t s1, s2;
    q31_t pSinVal[TEST_SIZE], pSinVal_ref[TEST_SIZE];
    q31_t pCosVal[TEST_SIZE], pCosVal_ref[TEST_SIZE];
    q31_t q31_pIN[TEST_SIZE];

    for (i = 0; i < TEST_SIZE; i++) {
        q31_pIN[i] = (q31_t)rand();
    }

    BENCH_START(riscv_sin_cos_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        riscv_sin_cos_q31(q31_pIN[i], &pSinVal[i], &pCosVal[i]);
    }
    BENCH_END(riscv_sin_cos_q31);
    for (i = 0; i < TEST_SIZE; i++) {
        ref_sin_cos_q31(q31_pIN[i], &pSinVal_ref[i], &pCosVal_ref[i]);
    }
    s1 = verify_results_q31(pSinVal, pSinVal_ref, TEST_SIZE);
    s2 = verify_results_q31(pCosVal, pCosVal_ref, TEST_SIZE);
    if (s1 != 0 || s2 != 0) {
        BENCH_ERROR(riscv_sin_cos_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_sin_cos_q31);
}


int main()
{
    BENCH_INIT();
    srand(__RV_CSR_READ(CSR_MCYCLE));

    generate_rand_f32(pIalpha, TEST_SIZE);
    generate_rand_f32(pIbeta, TEST_SIZE);
    generate_rand_f32(Ia, TEST_SIZE);
    generate_rand_f32(Ib, TEST_SIZE);
    generate_rand_f32(sinVal, TEST_SIZE);
    generate_rand_f32(cosVal, TEST_SIZE);

#if defined CLARKE
    DSP_clarke_f32_app();
    DSP_clarke_q31_app();
#endif
#if defined INV_CLARKE
    DSP_inv_clarke_f32_app();
    DSP_inv_clarke_q31_app();
#endif
#if defined PARK
    DSP_park_f32_app();
    DSP_park_q31_app();
#endif
#if defined INV_PARK
    DSP_inv_park_f32_app();
    DSP_inv_park_q31_app();
#endif
#if defined PID
    DSP_pid_f32_app();
    DSP_pid_q15_app();
    DSP_pid_q31_app();
#endif

#if defined SIN_COS
    DSP_SIN_COS_F32();
    DSP_SIN_COS_Q31();
#endif

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
