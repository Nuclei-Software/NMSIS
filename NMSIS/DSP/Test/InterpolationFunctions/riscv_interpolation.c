#include "riscv_math.h"
#include <stdint.h>
#include <stdlib.h>
#include "array.h"
#include "../common.h"

#include "../HelperFunctions/math_helper.c"
#include "../HelperFunctions/ref_helper.c"

#include <stdio.h>

int test_flag_error = 0;
int8_t s;
BENCH_DECLARE_VAR();

void linear_test(void)
{
    //simulate y=pi*x + pi/2
    riscv_linear_interp_instance_f32 S_f32 = {
        ARRAY_SIZE,       /*number of values*/
        0,                /*start value of x*/
        0.1,              /*x's spacing*/
        f32_y_array,      /*value of y*/
    };
    float32_t x_f32_input = 5.12, y_f32_output, y_f32_output_ref = 17.6557507131746;
    BENCH_START(riscv_linear_interp_f32);
    y_f32_output = riscv_linear_interp_f32(&S_f32, x_f32_input);
    BENCH_END(riscv_linear_interp_f32);
    s = verify_results_f32(&y_f32_output, &y_f32_output_ref, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_linear_interp_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_linear_interp_f32);

    //simulate y=0.00000125x+0.00000625
    //in q31 is y=2684x+13421
    //the x_input is 3972.
    //x_q31_input is in 12.20 format. 12 for start position, 20 for format(calculate before).
    //first calculate the format, then shift right 12 bits. At last move expected index to the high 12 bits
    //this is 39 and 0.72
    q31_t x_q31_input = 41271951, y_q31_output, y_q31_output_ref = 10674269;
    BENCH_START(riscv_linear_interp_q31);
    y_q31_output = riscv_linear_interp_q31(q31_y_array, x_q31_input, ARRAY_SIZE);
    BENCH_END(riscv_linear_interp_q31);
    //change result to float32_t to compare
    riscv_q31_to_float(&y_q31_output, &y_f32_output, 1);
    riscv_q31_to_float(&y_q31_output_ref, &y_f32_output_ref, 1);
    s = verify_results_f32(&y_f32_output, &y_f32_output_ref, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_linear_interp_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_linear_interp_q31);

    //simulate y=0.000125x+0.000625
    //in q15 is y=4x+20
    //the x_input is 3972.
    //x_q15_input is in 12.20 format. 12 for start position, 20 for format(calculate before).
    //first calculate the format, then shift right 12 bits. At last move expected index to the high 12 bits
    //this is 39 and 0.72
    q31_t x_q15_input = 41271951;
    q15_t y_q15_output, y_q15_output_ref = 15908;
    BENCH_START(riscv_linear_interp_q15);
    y_q15_output = riscv_linear_interp_q15(q15_y_array, x_q15_input, ARRAY_SIZE);
    BENCH_END(riscv_linear_interp_q15);
    //change result to float32_t to compare
    riscv_q15_to_float(&y_q15_output, &y_f32_output, 1);
    riscv_q15_to_float(&y_q15_output_ref, &y_f32_output_ref, 1);
    s = verify_results_f32(&y_f32_output, &y_f32_output_ref, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_linear_interp_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_linear_interp_q15);

    //simulate y=0.02x+0.03
    //in q7 is y=2x+3
    //the x_input is 39.
    //x_q7_input is in 12.20 format. 12 for start position, 20 for format(calculate before).
    //first calculate the format, then shift right 12 bits. At last move expected index to the high 12 bits
    //this is 35 and 0.8
    q31_t x_q7_input = 37119590;
    q7_t y_q7_output, y_q7_output_ref = 81;
    BENCH_START(riscv_linear_interp_q7);
    y_q7_output = riscv_linear_interp_q7(q7_y_array, x_q7_input, 9);
    BENCH_END(riscv_linear_interp_q7);
    //change result to float32_t to compare
    riscv_q7_to_float(&y_q7_output, &y_f32_output, 1);
    riscv_q7_to_float(&y_q7_output_ref, &y_f32_output_ref, 1);
    s = verify_results_f32(&y_f32_output, &y_f32_output_ref, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_linear_interp_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_linear_interp_q7);
}

void bilinear_test(void)
{
    //simulate z=3.1*x + 7.2*y + 1.25
    riscv_bilinear_interp_instance_f32 S_f32 = {
        20,                 /*number of Rows*/
        20,                 /*number of Columns*/
        f32_z_array,        /*value of y*/
    };
    float32_t x_f32_input = 5.12, y_f32_input = 6.24, z_f32_output, z_f32_output_ref = 62.0500000000000;
    BENCH_START(riscv_bilinear_interp_f32);
    z_f32_output = riscv_bilinear_interp_f32(&S_f32, x_f32_input, y_f32_input);
    BENCH_END(riscv_bilinear_interp_f32);
    s = verify_results_f32(&z_f32_output_ref, &z_f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_bilinear_interp_f32);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_bilinear_interp_f32);

    //simulate z=47120*x + 39675*y + 96200
    //stands for z=0.000022*x + 0.000018*y + 0.000045
    //test:x:5.12 y:6.24
    riscv_bilinear_interp_instance_q31 S_q31 = {
        20,                  /*number of Rows*/
        20,                  /*number of Columns*/
        q31_z_array,         /*value of y*/
    };
    q31_t x_q31_input = 5305794, y_q31_input = 6417285, z_q31_output, z_q31_output_ref = 579735;
    BENCH_START(riscv_bilinear_interp_q31);
    z_q31_output = riscv_bilinear_interp_q31(&S_q31, x_q31_input, y_q31_input);
    BENCH_END(riscv_bilinear_interp_q31);
    riscv_q31_to_float(&z_q31_output, &z_f32_output, 1);
    riscv_q31_to_float(&z_q31_output_ref, &z_f32_output_ref, 1);
    s = verify_results_f32(&z_f32_output_ref, &z_f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_bilinear_interp_q31);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_bilinear_interp_q31);

    //simulate z=41*x + 205*y + 328
    //stands for z=0.00125*x + 0.00625*y + 0.01
    //test:x:5.12 y:6.24
    riscv_bilinear_interp_instance_q15 S_q15 = {
        20,                   /*number of Rows*/
        20,                   /*number of Columns*/
        q15_z_array,          /*value of y*/
    };
    q31_t x_q15_input = 5305794, y_q15_input = 6417285;
    q15_t z_q15_output, z_q15_output_ref = 1815;
    BENCH_START(riscv_bilinear_interp_q15);
    z_q15_output = riscv_bilinear_interp_q15(&S_q15, x_q15_input, y_q15_input);
    BENCH_END(riscv_bilinear_interp_q15);
    riscv_q15_to_float(&z_q15_output, &z_f32_output, 1);
    riscv_q15_to_float(&z_q15_output_ref, &z_f32_output_ref, 1);
    s = verify_results_f32(&z_f32_output_ref, &z_f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_bilinear_interp_q15);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_bilinear_interp_q15);

    //simulate z=16*x + 3*y + 1
    //stands for z=0.125*x + 0.025*y + 0.01
    //test:x:5.12 y:6.24
    riscv_bilinear_interp_instance_q7 S_q7 = {
        20,                   /*number of Rows*/
        20,                   /*number of Columns*/
        q7_z_array,           /*value of y*/
    };
    q31_t x_q7_input = 5305794, y_q7_input = 6417285;
    q7_t z_q7_output, z_q7_output_ref = 103;
    BENCH_START(riscv_bilinear_interp_q7);
    z_q7_output = riscv_bilinear_interp_q7(&S_q7, x_q7_input, y_q7_input);
    BENCH_END(riscv_bilinear_interp_q7);
    riscv_q7_to_float(&z_q7_output, &z_f32_output, 1);
    riscv_q7_to_float(&z_q7_output_ref, &z_f32_output_ref, 1);
    s = verify_results_f32(&z_f32_output_ref, &z_f32_output, 1);
    if (s != 0) {
        BENCH_ERROR(riscv_bilinear_interp_q7);
        test_flag_error = 1;
    }
    BENCH_STATUS(riscv_bilinear_interp_q7);
}

int main(void)
{
    int i;

    BENCH_INIT();

    linear_test();
    bilinear_test();

    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
