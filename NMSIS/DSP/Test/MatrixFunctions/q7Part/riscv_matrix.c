
//
// Created by lujun on 19-6-28.
//
// This contains f32, q31 and q7 versions of matrix
// each one has it's own function.
// All function can be found in main function.
// If you don't want to use it, then comment it.
#include "riscv_math.h"
#include "array.h"
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

int DSP_matrix_q7(void)
{
    riscv_matrix_instance_q7 q7_A;

    q7_t q7_ref_vec[ROWS];
    q7_t q7_dst_vec[ROWS];

    riscv_mat_init_q7(&q7_A, ROWS, COLUMNS, (q7_t *)q7_a_array);

    // ****************   q7   *********************
    // mat_vec_mult
    BENCH_START(riscv_mat_vec_mult_q7);
    riscv_mat_vec_mult_q7(&q7_A, q7_b_vec, q7_dst_vec);
    BENCH_END(riscv_mat_vec_mult_q7);
    ref_mat_vec_mult_q7(&q7_A, q7_b_vec, q7_ref_vec);
    for (int i = 0; i < ROWS; i++)
        if (labs(q7_ref_vec[i] - q7_dst_vec[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_mat_vec_mult_q7);
            printf("index: %d,expect: %f, actual: %f\n", i, q7_ref_vec[i],
                   q7_dst_vec[i]);
            test_flag_error = 1;
        }
    BENCH_STATUS(riscv_mat_vec_mult_q7);

    printf("all tests are passed,well done!\n");
}

int main()
{
    BENCH_INIT;
    DSP_matrix_q7();
    BENCH_FINISH;
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}

