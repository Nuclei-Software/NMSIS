#include "riscv_math.h"
#include <stdio.h>
#include "../validate.h"
#include "nmsis_bench.h"
#include "../TestData/MatrixFunctions/mat_mult_q7/test_data.h"

BENCH_DECLARE_VAR();

void matMult_riscv_mat_mult_q7(void)
{
    q7_t q7_output[M * N];
    q7_t q7_output_back[K * N];
    riscv_matrix_instance_q7 q7_A;
    riscv_matrix_instance_q7 q7_B;
    riscv_matrix_instance_q7 q7_des;
    riscv_mat_init_q7(&q7_A, M, K, (q7_t *)mat_mult_q7_arrayA);
    riscv_mat_init_q7(&q7_B, K, N, (q7_t *)mat_mult_q7_arrayB);
    riscv_mat_init_q7(&q7_des, M, N, q7_output);

    generate_rand_q7(mat_mult_q7_arrayA, M * K);
    generate_rand_q7(mat_mult_q7_arrayB, K * N);

    BENCH_START(riscv_mat_mult_q7);
    riscv_status result = riscv_mat_mult_q7(&q7_A, &q7_B, &q7_des, q7_output_back);
    BENCH_END(riscv_mat_mult_q7);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

    return;
}
