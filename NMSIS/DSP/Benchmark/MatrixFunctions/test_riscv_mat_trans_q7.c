#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_trans_q7/test_data.h"

BENCH_DECLARE_VAR();

void matTrans_riscv_mat_trans_q7(void)
{
    q7_t q7_output[N * M];
    riscv_matrix_instance_q7 q7_A;
    riscv_matrix_instance_q7 q7_des;

    riscv_mat_init_q7(&q7_A, M, N, mat_trans_q7_array);
    riscv_mat_init_q7(&q7_des, N, M, q7_output);

    generate_rand_q7(mat_trans_q7_array, M * N);

    BENCH_START(riscv_mat_trans_q7);
    riscv_status result = riscv_mat_trans_q7(&q7_A, &q7_des);
    BENCH_END(riscv_mat_trans_q7);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}