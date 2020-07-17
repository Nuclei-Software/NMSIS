//
// Created by lujun on 19-6-28.
//
// This contains f32, q31 and q15 versions of matrix
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "../common.h"
#include "riscv_math.h"
#include "array.h"
#include <stdint.h>

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>
#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (1)
#define DELTAQ7 (1)

int test_flag_error = 0;

int DSP_matrix_f32(void)
{
    riscv_matrix_instance_f32 f32_A;
    riscv_matrix_instance_f32 f32_B;
    riscv_matrix_instance_f32 f32_C;
    riscv_matrix_instance_f32 f32_ref;
    riscv_matrix_instance_f32 f32_des;
    float32_t f32_output[ROWS * COLUMNS];
    float32_t f32_output_ref[ROWS * COLUMNS];
    float32_t f32_output_back[ROWS * COLUMNS];
    int i;
    riscv_mat_init_f32(&f32_A, ROWS, COLUMNS, (float32_t *)f32_a_array);
    riscv_mat_init_f32(&f32_B, ROWS, COLUMNS, (float32_t *)f32_b_array);
    riscv_mat_init_f32(&f32_des, ROWS, COLUMNS, f32_output);
    riscv_mat_init_f32(&f32_ref, ROWS, COLUMNS, f32_output_ref);

    BENCH_START(riscv_mat_add_f32);
    riscv_mat_add_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_add_f32);
    ref_mat_add_f32(&f32_A, &f32_B, &f32_ref);
    for (int i = 0; i < (ROWS * COLUMNS); i++)
        if (fabs(f32_output[i] - f32_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_add_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, f32_output_ref[i],
                   f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_add_f32);
    // inverse
    BENCH_START(riscv_mat_inverse_f32);
    float32_t f32_inv_output_ref[100] = {
        -0.00862429626639179, -0.00959854824422037,  0.0276167749043908,
        0.0108561123352599,   0.0219330933404954,    0.0361097345934595,
        0.0237285090347189,   0.0454092692778495,    0.0138371484972558,
        -0.0225754026431660,  0.0107344362016443,    0.00122979583992182,
        -0.0213552741123591,  -0.00743200065898201,  -0.00299948997507613,
        -0.00713079223019504, -0.0123823441857860,   -0.0200984993069169,
        -0.00503608372413689, 0.0155143879565654,    0.00583053739301843,
        0.00765554270862055,  -0.00343944039204994,  -0.00908688956508297,
        -0.0136529770004075,  -0.00341920340058213,  -0.00961140971207766,
        -0.0201116121923120,  -0.00438561194875933,  0.00997249718995338,
        -0.0293222711839281,  -0.0136459914172593,   0.0425696433663838,
        0.0209648863512806,   0.0340701727377447,    0.0357643787494830,
        0.0403642233830325,   0.0738440838510826,    0.0259793923156693,
        -0.0321812696806133,  -0.0121783037860931,   -0.0106248312361224,
        0.0243388527205345,   0.00823600351158623,   0.0150986813713879,
        0.0288376918378642,   0.0223018839612701,    0.0370713566414873,
        0.00937666971275350,  -0.0151354278219778,   -0.0107869919180683,
        -0.00133526043197877, -0.000460487058955662, 0.00506392055347465,
        0.00496541753596285,  -0.0108365696863311,   0.00310100531237189,
        0.00106620729312447,  -0.00267177046093381,  -0.00381950823531589,
        -0.00125988201158372, 0.00293383386906584,   -0.00338908223327635,
        0.00666934914595760,  0.00198546014854288,   -0.00345374185778855,
        0.00357331319361273,  0.00364319523513236,   0.00143733480632802,
        -0.00107606696568794, -0.00284404852170878,  -0.00931995675983785,
        0.0118818237771228,   0.00500828876426470,   0.0113242505306337,
        0.0139815916613188,   0.0133341815962530,    0.0160922940114372,
        0.00672399457731019,  -0.00781453110218049,  0.0105646437027108,
        0.00459652521028714,  0.00340440564330692,   -0.00919888068358254,
        -0.0124783843244020,  0.0108694023799228,    -0.000852727495570467,
        -0.00697204289638378, 0.000576430608542981,  -0.00262045974634447,
        0.00901418253947032,  0.000213766880640661,  0.0115163430233437,
        -0.00601202611917606, 0.00190698272103374,   0.0248008397449382,
        0.00809763125405313,  0.0141105893442632,    0.00469553745124477,
        -0.00601571239586183};
    riscv_mat_inverse_f32(&f32_A, &f32_des);
    BENCH_END(riscv_mat_inverse_f32);
    // ref_mat_inverse_f32(&f32_A, &f32_ref);
    for (int i = 0; i < (ROWS * COLUMNS); i++)
        if (fabs(f32_output[i] - f32_inv_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_inverse_f32);
            printf("index: %d,expect: %f, actual: %f\n", i,
                   f32_inv_output_ref[i], f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_inverse_f32);
    // mult
    BENCH_START(riscv_mat_mult_f32);
    riscv_mat_mult_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_mult_f32);
    ref_mat_mult_f32(&f32_A, &f32_B, &f32_ref);
    for (int i = 0; i < (ROWS * COLUMNS); i++)
        if (fabs(f32_output[i] - f32_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_mult_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, f32_output_ref[i],
                   f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_mult_f32);
    // scale
    BENCH_START(riscv_mat_scale_f32);
    riscv_mat_scale_f32(&f32_A, 8.73f, &f32_des);
    BENCH_END(riscv_mat_scale_f32);
    ref_mat_scale_f32(&f32_A, 8.73f, &f32_ref);
    for (int i = 0; i < (ROWS * COLUMNS); i++)
        if (fabs(f32_output[i] - f32_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_scale_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, f32_output_ref[i],
                   f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_scale_f32);
    // sub
    BENCH_START(riscv_mat_sub_f32);
    riscv_mat_sub_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_sub_f32);
    ref_mat_sub_f32(&f32_A, &f32_B, &f32_ref);
    for (int i = 0; i < (ROWS * COLUMNS); i++)
        if (fabs(f32_output[i] - f32_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_sub_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, f32_output_ref[i],
                   f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_sub_f32);
    // trans
    BENCH_START(riscv_mat_trans_f32);
    riscv_mat_trans_f32(&f32_A, &f32_des);
    BENCH_END(riscv_mat_trans_f32);
    ref_mat_trans_f32(&f32_A, &f32_ref);
    for (int i = 0; i < (ROWS * COLUMNS); i++)
        if (fabs(f32_output[i] - f32_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_trans_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, f32_output_ref[i],
                   f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_trans_f32);
    // cmplx_mult
    riscv_mat_init_f32(&f32_A, 5, 5, (float32_t *)f32_a_array);
    riscv_mat_init_f32(&f32_B, 5, 5, (float32_t *)f32_b_array);
    riscv_mat_init_f32(&f32_des, 5, 5, f32_output);
    riscv_mat_init_f32(&f32_ref, 5, 5, f32_output_ref);
    BENCH_START(riscv_mat_cmplx_mult_f32);
    riscv_mat_cmplx_mult_f32(&f32_A, &f32_B, &f32_des);
    BENCH_END(riscv_mat_cmplx_mult_f32);
    ref_mat_cmplx_mult_f32(&f32_A, &f32_B, &f32_ref);
    for (i = 0; i < (5 * 5); i++)
        if (fabs(f32_output[i] - f32_output_ref[i]) > DELTAF32) {
            BENCH_ERROR(riscv_mat_cmplx_mult_f32);
            printf("index: %d,expect: %f, actual: %f\n", i, f32_output_ref[i],
                   f32_output[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_cmplx_mult_f32);
    printf("all tests are passed,well done!\n");
}

int main()
{
    BENCH_INIT;
    DSP_matrix_f32();
    BENCH_FINISH;
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
