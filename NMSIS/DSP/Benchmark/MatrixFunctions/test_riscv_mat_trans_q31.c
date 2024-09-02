#include "riscv_math.h"
#include <stdio.h>
#include "validate.h"
#include "nmsis_bench.h"
#include "TestData/MatrixFunctions/mat_trans_q31/test_data.h"

BENCH_DECLARE_VAR();

void matTrans_riscv_mat_trans_q31(void)
{
    q31_t q31_output[N * M];
    riscv_matrix_instance_q31 q31_A;
    riscv_matrix_instance_q31 q31_des;

    riscv_mat_init_q31(&q31_A, M, N, mat_trans_q31_array);
    riscv_mat_init_q31(&q31_des, N, M, q31_output);

    generate_rand_q31(mat_trans_q31_array, M * N);

    BENCH_START(riscv_mat_trans_q31);
    riscv_status result = riscv_mat_trans_q31(&q31_A, &q31_des);
    BENCH_END(riscv_mat_trans_q31);

    TEST_ASSERT_EQUAL(RISCV_MATH_SUCCESS, result);

}