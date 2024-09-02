#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_trans_q15/test_data.h"

BENCH_DECLARE_VAR();

void matTrans_riscv_mat_trans_q15(void)
{
    q15_t q15_output[N * M];
    riscv_matrix_instance_q15 q15_A;
    riscv_matrix_instance_q15 q15_des;

    riscv_mat_init_q15(&q15_A, M, N, mat_trans_q15_array);
    riscv_mat_init_q15(&q15_des, N, M, q15_output);

    generate_rand_q15(mat_trans_q15_array, M * N);

    BENCH_START(riscv_mat_trans_q15);
    riscv_status result = riscv_mat_trans_q15(&q15_A, &q15_des);
    BENCH_END(riscv_mat_trans_q15);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);
}